/*  Copyright (C) 2013  Nithin Nellikunnu, nithin.nn@gmail.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */  

#include <list>
#include <vector>
#include <exception>

#include "pugixml.hpp"
using namespace pugi;

#include "types.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "message.hpp"
#include "xml_parser.hpp"

/**
 * @brief
 *    Parses the xml scenario file using pugi xml
 *
 * @throw ErrCodeEn
 */
XmlParser::XmlParser(const S8 *pXmlFile) throw (ErrCodeEn)
{
   xml_parse_result res = m_xmlDoc.load_file(pXmlFile);
   if (res.status != status_ok)
   {
      LOG_FATAL("%s", res.description());
      throw ERR_XML_PARSING;
   }
}

MsgVec* XmlParser::parseXmlDoc()
{
   xml_node root = m_xmlDoc.first_child();
   LOG_INFO("%s", root.name());
   LOG_INFO("%s", root.attribute("name").value());

   MsgVec *pMsgVec = NULL;

   try
   {
      pMsgVec = new MsgVec;
      for (xml_node node = root.first_child(); node;\
            node = node.next_sibling())
      {
         MsgTask  *pMsgTask = NULL;

         if (0 == strcmp(node.name(), "send"))
         {
            pMsgTask = procSend(&node);
            pMsgVec->push_back(pMsgTask);
         }
         else if (0 == strcmp(node.name(), "recv"))
         {
            pMsgTask = procRecv(&node);
            pMsgVec->push_back(pMsgTask);
         }
         else if (0 == strcmp(node.name(), "wait"))
         {
            pMsgTask = procWait(&node);
            pMsgVec->push_back(pMsgTask);
         }
         else
         {
            LOG_ERROR("Unknown Tag: %s", node.name());
         }
      }
   }
   catch (std::exception &m)
   {
      throw ERR_MEMORY_ALLOC;
   }
   catch (ErrCodeEn &e)
   {
      throw ERR_XML_PROCESSING;
   }

   LOG_EXITFN(pMsgVec);
}
 
/**
 *  Class destructor frees memory used to hold the XML tag and
 *  attribute definitions. It als terminates use of the xerces-C
 *  framework.
 */
XmlParser::~XmlParser()
{
}
 

/**
 * @brief
 *    Processes the <send> element tag
 *
 * @param node
 *    node contianing entire <send> element tag
 *
 * @return
 *    ROK if processed successfully
 *    RFAILED otherwise
 */
MsgTask* XmlParser::procSend(xml_node *pSend)
{
   LOG_ENTERFN();

   GtpIeLst         ieLst;
   MsgTask           *pMsgTask = NULL;

   try
   {
      const S8 *pMsgName = NULL;

      pMsgName = pSend->attribute("request").value();
      if (STRLEN(pMsgName) == 0)
      {
         pMsgName = pSend->attribute("response").value();
      }

      LOG_DEBUG("Processing <%s>", pMsgName);

      for (xml_node node = pSend->first_child(); node;\
            node = node.next_sibling())
      {
         if (0 == strcmp(node.name(), "ie"))
         {
            procIe(&node, &ieLst);
         }
         else
         {
            LOG_ERROR("Unknown Tag: %s", node.name());
         }
      }

      GtpMsg *pGtpMsg = new GtpMsg(gtpGetMsgType(pMsgName));
      pGtpMsg->encode(&ieLst);

      pMsgTask = new MsgTask(pGtpMsg, MSG_TASK_SEND);
   }
   catch (std::exception &m)
   {
      throw ERR_MEMORY_ALLOC;
   }
   catch (ErrCodeEn &e)
   {
      throw ERR_XML_PROCESSING;
   }

   LOG_EXITFN(pMsgTask);
}

/**
 * @brief
 *    Processes the <recv> element tag
 *
 * @param node
 *    node contianing entire <recv> element tag
 *
 * @return
 *    ROK if processed successfully
 *    RFAILED otherwise
 */
MsgTask* XmlParser::procRecv(xml_node *pRecv)
{
   LOG_ENTERFN();

   MsgTask     *pMsgTask = NULL;

   try
   {
      LOG_DEBUG("Processing <%s>", pRecv->name());
      const S8 *pMsgName = NULL;
      pMsgName = pRecv->attribute("request").value();
      if (STRLEN(pMsgName) == 0)
      {
         pMsgName = pRecv->attribute("response").value();
      }

      for (xml_node node = pRecv->first_child(); node;\
            node = node.next_sibling())
      {
         if (0 == strcmp(node.name(), "store"))
         {
            procStore(&node);
         }
         else if (0 == strcmp(node.name(), "validate"))
         {
            procValidate(&node);
         }
         else
         {
            LOG_ERROR("Unknown Tag: %s", node.name());
         }
      }

      GtpMsg *pGtpMsg = new GtpMsg(gtpGetMsgType(pMsgName));
      pMsgTask = new MsgTask(pGtpMsg, MSG_TASK_RECV);
   }
   catch (std::exception &m)
   {
      throw ERR_MEMORY_ALLOC;
   }
   catch (ErrCodeEn &e)
   {
      throw ERR_XML_PROCESSING;
   }

   LOG_EXITFN(pMsgTask);
}

MsgTask* XmlParser::procWait(xml_node *pWait)
{
   LOG_ENTERFN();

   MsgTask* pMsgTask = NULL;

   LOG_EXITFN(pMsgTask);
}

/**
 * @brief 
 *
 * @param pIe
 *    Pointer to <ie> tag xml node
 *
 * @return 
 *    ROK if processed successfully
 *    RFAILED otherwise
 */
RETVAL XmlParser::procIe(xml_node *pXmlIe, GtpIeLst *pIeLst)
{
   RETVAL         ret        = ROK;
   GtpIeType_E    ieType     = GTP_IE_MAX;
   GtpInstance_t  ieInstance = 0;

   LOG_DEBUG("Processing IE <%s>",\
         pXmlIe->attribute("type").value());

   ieType = gtpGetIeType(pXmlIe->attribute("type").value());
   ieInstance = pXmlIe->attribute("instance").as_uint();

   GtpIe          *pIe = GtpIe::createGtpIe(ieType, ieInstance);

   if (NULL != pIe)
   {
      if (pXmlIe->first_child())
      {
         const S8 *pChildName = pXmlIe->first_child().name();
         if (0 == strcasecmp("ie", pChildName))
         {
            LOG_DEBUG("Processing Grouped IE");
            ret = procGroupedIe(pIe, pXmlIe);
            if (ROK != ret)
            {
               LOG_ERROR("Processing Grouped IE");
            }
         }
         else
         {
            LOG_DEBUG("Processing composite IE");
            ret = procCompositeIe(pIe, pXmlIe);
            if (ret != ROK)
            {
               LOG_ERROR("Processing composite IE");
            }
         }
      }
      else
      {
         const S8* pVal = pXmlIe->attribute("value").value();
         if (XML_HEX_VAL(pVal))
         {
            XmlBuffer   *pBuf = new XmlBuffer;
            /* dont copy '0x' part of the hex buffer */
            XML_BUFFER_CPY(pBuf, pVal + 2, strlen(pVal) - 2);
            ret = pIe->encode(pBuf);
         }
         else
         {
            ret = pIe->encode((U8 *)pVal);
         }
      }

      if (ROK == ret)
      {
         pIeLst->push_back(pIe);
      }
   }
   else
   {
      LOG_ERROR("Creating IE <%s>",\
            pXmlIe->attribute("type").value());
   }
   
   return ret;
}

/**
 * @brief 
 *
 * @param pStore
 *    Pointer to <store> tag xml node
 *
 * @return 
 *    ROK if processed successfully
 *    RFAILED otherwise
 */
RETVAL XmlParser::procStore(xml_node *pStore)
{
   RETVAL ret = ROK;

   LOG_DEBUG("Processing Storing <%s>", pStore->name());

   return ret;
}


/**
 * @brief
 *
 * @param pValidate
 *    Pointer to <validate> tag xml node
 *
 * @return 
 *    ROK if processed successfully
 *    RFAILED otherwise
 */
RETVAL XmlParser::procValidate(xml_node *pValidate)
{
   RETVAL ret = ROK;

   LOG_ENTERFN();

   LOG_DEBUG("Processing Validate <%s>", pValidate->name());

   LOG_EXITFN(ret);
}


RETVAL XmlParser::procCompositeIe(GtpIe *pIe, xml_node *pXmlIe)
{
   S16            ret = ROK;
   XmlBufferLst   *pBufLst = new XmlBufferLst;

   LOG_ENTERFN();

   for (xml_node node = pXmlIe->first_child(); node;\
         node = node.next_sibling())
   {
      const S8* pVal = node.attribute("value").value();

      XmlBuffer   *pParam = new XmlBuffer;
      STRCPY(pParam->paramName, node.attribute("type").value());

      if (XML_HEX_VAL(pVal))
      {
         /* Hex buffer, skipt first 2 bytes '0x' */
         XML_BUFFER_CPY(pParam, pVal + 2, strlen(pVal) - 2);
      }
      else
      {
         XML_BUFFER_CPY(pParam, pVal, strlen(pVal));
      }

      pBufLst->push_back(pParam);
   }

   pIe->encode(pBufLst);

   LOG_EXITFN(ret);
}


RETVAL XmlParser::procGroupedIe(GtpIe *pIe, xml_node *pXmlIe)
{
   RETVAL         ret = ROK;
   GtpIeLst      ieLst;

   LOG_ENTERFN();

   LOG_DEBUG("Processing Grouped IE <%s>",\
         pXmlIe->attribute("type").value());

   for (xml_node node = pXmlIe->first_child(); node;\
        node = node.next_sibling())
   {
      ret = procIe(&node, &ieLst);
      if (ret != ROK)
      {
         LOG_WARN("Processing of IE [%s] failed",\
               node.attribute("type").value());
      }
   }

   pIe->encode(&ieLst);

   LOG_EXITFN(ret);
}


PUBLIC VOID parseXmlScenario(const S8 *pScnFile, MsgVec *pMsgVec) \
      throw (ErrCodeEn)
{
   LOG_ENTERFN();

   try
   {
      XmlParser parser(pScnFile);
      
      MsgVec *pTmpMskVec = parser.parseXmlDoc();
      if (NULL == pMsgVec)
      {
         throw ERR_XML_PARSING;
      }
      else
      {
         for (U32 i = 0; i < pTmpMskVec->size(); i++)
         {
            MsgTask *pMsgTask = (*pTmpMskVec)[i];
            pMsgVec->push_back(pMsgTask);
         }

         delete pTmpMskVec;
      }
   }
   catch (ErrCodeEn &e)
   {
      throw ERR_XML_INIT;
   }

   LOG_EXITVOID();
}
