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
#include "procedure.hpp"
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

JobSequence* XmlParser::parseXmlDoc()
{
   xml_node root = m_xmlDoc.first_child();
   LOG_INFO("%s", root.name());
   LOG_INFO("%s", root.attribute("name").value());

   JobSequence *jobSeq = NULL;

   try
   {
      jobSeq = new JobSequence;
      for (xml_node node = root.first_child(); node;\
            node = node.next_sibling())
      {
         Job  *job = NULL;

         if (0 == strcmp(node.name(), "send"))
         {
            job = procSend(&node);
            jobSeq->push_back(job);
         }
         else if (0 == strcmp(node.name(), "recv"))
         {
            job = procRecv(&node);
            jobSeq->push_back(job);
         }
         else if (0 == strcmp(node.name(), "wait"))
         {
            job = procWait(&node);
            jobSeq->push_back(job);
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

   LOG_EXITFN(jobSeq);
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
Job* XmlParser::procSend(xml_node *pSend)
{
   LOG_ENTERFN();

   GtpIeLst ieLst;
   Job  *job = NULL;

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

      job = new Job(pGtpMsg, JOB_TYPE_SEND);
   }
   catch (std::exception &m)
   {
      throw ERR_MEMORY_ALLOC;
   }
   catch (ErrCodeEn &e)
   {
      throw ERR_XML_PROCESSING;
   }

   LOG_EXITFN(job);
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
Job* XmlParser::procRecv(xml_node *pRecv)
{
   LOG_ENTERFN();

   Job     *job = NULL;

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
      job = new Job(pGtpMsg, JOB_TYPE_RECV);
   }
   catch (std::exception &m)
   {
      throw ERR_MEMORY_ALLOC;
   }
   catch (ErrCodeEn &e)
   {
      throw ERR_XML_PROCESSING;
   }

   LOG_EXITFN(job);
}

Job* XmlParser::procWait(xml_node *pWait)
{
   LOG_ENTERFN();

   Job *job = NULL;

   auto child = pWait->first_child();
   Time_t wait = static_cast<Time_t>(std::stol(child.value()));
   job = new Job(wait);

   LOG_EXITFN(job);
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
   GtpIeType_t    ieType     = GTP_IE_MAX;
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
            ret = procComplexIe(pIe, pXmlIe);
            if (ret != ROK)
            {
               LOG_ERROR("Processing composite IE");
            }
         }
      }
      else
      {
         const S8 *value = pXmlIe->attribute("value").value();
         if (XML_HEX_VAL(value))
         {
            /* skipping '0x' part of the hex buffer */
            HexString hexStr = (value + 2);
            ret = pIe->buildIe(&hexStr);
         }
         else
         {
            ret = pIe->buildIe(value);
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

/**
 * @brief 
 * Complex IEs are not grouped IEs. They are IEs with more than one 
 * types of data within it. For example ULI is a Complex IE, but Bearer
 * is a Grouped IE. ULI has multiple types of data present - sai, tai etc.
 *
 * @param pIe
 * @param pXmlIe
 *
 * @return 
 */
RETVAL XmlParser::procComplexIe(GtpIe *pIe, xml_node *pXmlIe)
{
   S16            ret      = ROK;
   IeParamLst   *pBufLst = NULL;

   LOG_ENTERFN();

   pBufLst = new IeParamLst;
   for (xml_node node = pXmlIe->first_child(); node;\
         node = node.next_sibling())
   {
      const S8* pVal = node.attribute("value").value();

      IeParam   *pParam = new IeParam;
      STRCPY(pParam->paramName, node.attribute("type").value());

      if (XML_HEX_VAL(pVal))
      {
         /* Hex buffer, skipt first 2 bytes '0x' */
        BUFFER_CPY(&pParam->buf, pVal + 2, strlen(pVal) - 2);
      }
      else
      {
         BUFFER_CPY(&pParam->buf, pVal, strlen(pVal));
      }

      pBufLst->push_back(pParam);
   }

   pIe->buildIe(pBufLst);

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

   pIe->buildIe(&ieLst);

   LOG_EXITFN(ret);
}

PUBLIC VOID parseXmlScenario(const S8 *pScnFile, JobSequence *jobSeq) \
      throw (ErrCodeEn)
{
   LOG_ENTERFN();

   try
   {
      XmlParser parser(pScnFile);
      
      JobSequence *tmpJobSeq = parser.parseXmlDoc();
      if (NULL == jobSeq)
      {
         throw ERR_XML_PARSING;
      }
      else
      {
         for (U32 i = 0; i < tmpJobSeq->size(); i++)
         {
            Job *job = (*tmpJobSeq)[i];
            jobSeq->push_back(job);
         }

         delete tmpJobSeq;
      }
   }
   catch (ErrCodeEn &e)
   {
      throw ERR_XML_INIT;
   }

   LOG_EXITVOID();
}
