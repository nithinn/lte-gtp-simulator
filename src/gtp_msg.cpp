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
using std::list;

#include "types.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "gtp_types.hpp"
#include "gtp_macro.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "gtp_util.hpp"

/**
 * @brief
 *    constructor
 *
 * @param msgType
 */
GtpMsg::GtpMsg(GtpMsgType_E msgType)
{
   MEMSET(m_gtpMsgBuf, 0, GTP_MSG_BUF_LEN);
   m_msgHdr.msgType = msgType;
}

GtpMsg::GtpMsg(Buffer *pBuf)
{
   decodeHdr(pBuf->pVal);

   if (pBuf->len <= GTP_MSG_BUF_LEN)
   {
      if (GSIM_CHK_MASK(m_msgHdr.pres, GTP_MSG_T_BIT_PRES))
      {
         MEMCPY(m_gtpMsgBuf, pBuf->pVal + GTP_MSG_HDR_LEN,\
               (pBuf->len - GTP_MSG_HDR_LEN));
      }
      else
      {
         MEMCPY(m_gtpMsgBuf, pBuf->pVal + GTP_MSG_HDR_LEN_WITHOUT_TEID,\
               (pBuf->len - GTP_MSG_HDR_LEN_WITHOUT_TEID));
      }
   }
   else
   {
      throw ERR_GTP_MSG_BUF_OVERFLOW;
   }
}

/**
 * @brief
 *    destructor
 */
GtpMsg::~GtpMsg()
{
   for (GtpIeLstItr itr = m_ieLst.begin(); itr != m_ieLst.end(); itr++)
   {
      delete *itr;
   }
}

/**
 * @brief
 *    Encodes GTP message
 *
 * @param pIeLst
 *
 * @return 
 */
RETVAL GtpMsg::encode(GtpIeLst  *pIeLst)
{
   LOG_ENTERFN();

   RETVAL         ret = ROK;

   for (GtpIeLstItr ie = pIeLst->begin(); ie != pIeLst->end(); ie++)
   {
      m_ieLst.push_back(*ie);
   }

   LOG_EXITFN(ret);
}

/**
 * @brief sets the FTEID ie in Gtp Message IE
 *
 * @param teid
 * @param pIp
 *
 * @return 
 */
RETVAL GtpMsg::setSenderFteid(GtpTeid_t teid, const IpAddr *pIp)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;

   GtpIe *pGtpIe = getIe(GTP_IE_FTEID, 0, 1);
   if (NULL != pGtpIe)
   {
      GtpFteid *pFteid = dynamic_cast<GtpFteid *>(pGtpIe);
      pFteid->setTeid(teid);
      pFteid->setIpAddr(pIp);
   }
   else
   {
      ret = ERR_IE_NOT_FOUND;
      LOG_ERROR("Sender F-TEID missing");
   }

   LOG_EXITFN(ret);
}

VOID GtpMsg::setMsgHdr(const GtpMsgHdr* pHdr)
{
   LOG_ENTERFN();

   if (GSIM_CHK_MASK(pHdr->pres, GTP_MSG_HDR_TEID_PRES))
   {
      m_msgHdr.teid = pHdr->teid;
   }

   if (GSIM_CHK_MASK(pHdr->pres, GTP_MSG_HDR_VER_PRES))
   {
      m_msgHdr.ver = pHdr->ver;
   }

   if (GSIM_CHK_MASK(pHdr->pres, GTP_MSG_HDR_MSGTYPE_PRES))
   {
      m_msgHdr.msgType = pHdr->msgType;
   }

   if (GSIM_CHK_MASK(pHdr->pres, GTP_MSG_HDR_SEQ_PRES))
   {
      m_msgHdr.seqN = pHdr->seqN;
   }

   LOG_EXITVOID();
}

RETVAL GtpMsg::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   U8       *pTmpBuf = pBuf;
   U32      len = 0;
   RETVAL   ret = ROK;

   /* Message header will be encoded once all the ies are encoded.
    * So reserve the length of message header in the message buffer by
    * 12 bytes or 8 bytes (if teid not present
    */
   *pLen = 0;
   if (GSIM_CHK_MASK(m_msgHdr.pres, GTP_MSG_T_BIT_PRES))
   {
      pTmpBuf += GTP_MSG_HDR_LEN;
   }
   else
   {
      /* TEID not present */
      pTmpBuf += GTP_MSG_HDR_LEN_WITHOUT_TEID;
   }

   /* encode all the IEs */
   for (GtpIeLst::iterator ie = m_ieLst.begin(); ie != m_ieLst.end(); ie++)
   {
      ret = (*ie)->encode(pTmpBuf, &len);
      if (ROK == ret)
      {
         pTmpBuf += len;
         *pLen += len;
         len = 0;
      }
      else
      {
         LOG_ERROR("Encoding IE failed IE [%ld]", (*ie)->type());
      }
   }

   /* encode the message header */
   m_msgHdr.len = *pLen;
   *pLen += encodeHdr(pBuf);

   LOG_EXITFN(ret);
}

RETVAL GtpMsg::decode()
{
   LOG_ENTERFN();

   GtpLength_t len = m_msgHdr.len;
   U8          *pMsgBuf = m_gtpMsgBuf;
   while (len > 0)
   {
      GtpIeType_E    ieType = GTP_IE_RESERVED;
      GtpInstance_t  ieInst = 0;
      GtpLength_t    ieLen = 0;

      GTP_GET_IE_TYPE(pMsgBuf, ieType);
      GTP_GET_IE_INSTANCE(pMsgBuf, ieInst);
      GTP_GET_IE_LEN(pMsgBuf, ieLen);

      GtpIe    *pIe = GtpIe::createGtpIe(ieType, ieInst);

      Buffer  buf = {0, NULL};
      buf.len = ieLen;
      buf.pVal = new U8 [ieLen];
      MEMCPY(buf.pVal, (pMsgBuf + GTP_IE_HDR_LEN), ieLen);
      pIe->decode(&buf);

      m_ieLst.push_back(pIe);
      delete[] buf.pVal;

      pMsgBuf += (GTP_IE_HDR_LEN + ieLen);
      len -= (GTP_IE_HDR_LEN + ieLen);
   }

   LOG_EXITFN(ROK);
}

U32 GtpMsg::encodeHdr(U8 *pBuf)
{
   LOG_ENTERFN();

   U32   len = 0;
   U8    *pTmpBuf = pBuf;

   /* 1 byte of (3 bits ver, P and T bits, and 3 bits spare */  
   pTmpBuf[0] |= (m_msgHdr.ver << 5);
   if (GSIM_CHK_MASK(m_msgHdr.pres, GTP_MSG_P_BIT_PRES))
   {
      pTmpBuf[0] |= (1 << 4);
   }

   if (GSIM_CHK_MASK(m_msgHdr.pres, GTP_MSG_T_BIT_PRES))
   {
      pTmpBuf[0] |= (1 << 3);
   }
   pTmpBuf += 1;
   len += 1;                                                                           
   /* 1 byte of msg type */
   GTP_ENC_MSG_TYPE(pTmpBuf, m_msgHdr.msgType);  
   pTmpBuf += 1;
   len += 1;                                                                           
   /* 2 bytes of msg length */
   GTP_ENC_LEN(pTmpBuf, m_msgHdr.len); 
   pTmpBuf += 2;
   len += 2;

   /* 4 bytes of teid */
   if (GSIM_CHK_MASK(m_msgHdr.pres, GTP_MSG_T_BIT_PRES))
   {
      GTP_ENC_TEID(pTmpBuf, m_msgHdr.teid);
      pTmpBuf += 4;
      len += 4;/* 4 bytes of teid */                                         
   }
                                                                           
   /* 3 bytes of sequence number */
   GTP_ENC_SEQN(pTmpBuf, m_msgHdr.seqN);
   pTmpBuf += 3;
   len += 3; /* 3 bytes of seq number */                                  
                                                                           
   /* spare byte */                                                        
   pTmpBuf += 1;                                                              
   len += 1; /* 1 byte spare */                                           

   LOG_EXITFN(len);
}

VOID GtpMsg::decodeHdr(U8 *pBuf)
{
   LOG_ENTERFN();

   if (GTP_CHK_T_BIT_PRESENT(pBuf))
   {
      GSIM_SET_MASK(m_msgHdr.pres, GTP_MSG_T_BIT_PRES);
   }

   if (GTP_CHK_P_BIT_PRESENT(pBuf))
   {
      GSIM_SET_MASK(m_msgHdr.pres, GTP_MSG_P_BIT_PRES);
   }

   if (GSIM_CHK_MASK(m_msgHdr.pres, GTP_MSG_T_BIT_PRES))
   {
      GTP_MSG_DEC_TEID(pBuf, m_msgHdr.teid);
   }

   GTP_MSG_GET_TYPE(pBuf, m_msgHdr.msgType);
   GTP_MSG_GET_LEN(pBuf, m_msgHdr.len);
   GTP_MSG_GET_SEQN(pBuf, m_msgHdr.seqN);

   LOG_EXITVOID();
}

U32 GtpMsg::getIeCount(GtpIeType_E ieType, GtpInstance_t inst)
{
   LOG_ENTERFN();

   U32      cnt = 0;
   for (GtpIeLstItr ie = m_ieLst.begin(); ie != m_ieLst.end(); ie++)
   {
      if ((*ie)->type() == ieType && (*ie)->instance() == inst)
      {
         cnt++;
      }
   }

   LOG_EXITFN(cnt);
}

GtpIe* GtpMsg::getIe(GtpIeType_E  ieType, GtpInstance_t  inst, U32 occurance)
{
   LOG_ENTERFN();

   GtpIe    *pIe = NULL;
   U32      cnt = 0;
   for (GtpIeLstItr ie = m_ieLst.begin(); ie != m_ieLst.end(); ie++)
   {
      if ((*ie)->type() == ieType && (*ie)->instance() == inst)
      {
         cnt++;
         if (cnt == occurance)
         {
            pIe = *ie;
            break;
         }
      }
   }

   LOG_EXITFN(pIe);
}

/**
 * @brief
 *    Returns the buffer pointer at which the IE indicated by ietype, instance
 *    and occurance count exists in Gtp Message buffer
 *
 * @param ieType
 * @param inst
 * @param occr
 *
 * @return
 *    NULL if ie does not exist, otherwise the buffer pointer is returned
 */
U8* GtpMsg::getIeBufPtr
(
GtpIeType_E       ieType,\
GtpInstance_t     inst,
U32               occr
)
{
   LOG_ENTERFN();

   U8          *pIeBufPtr = NULL;
   GtpIeHdr    ieHdr;
   U32         cnt = 0;
   GtpLength_t len = m_msgHdr.len;
   U8          *pBuf = m_gtpMsgBuf;

   while (len)
   {
      decIeHdr(pBuf, &ieHdr);
      if (ieHdr.ieType == ieType && ieHdr.instance == inst)
      {
         cnt++;
         if (cnt == occr)
         {
            pIeBufPtr = pBuf;
            break;
         }
      }

      len -= (ieHdr.len + GTP_IE_HDR_LEN);
      pBuf += (ieHdr.len + GTP_IE_HDR_LEN);
   }

   LOG_EXITFN(pIeBufPtr);
}

VOID GtpMsg::setImsi(GtpImsiKey *pImsiKey)
{
   LOG_ENTERFN();

   GtpIe    *pGtpIe = getIe(GTP_IE_IMSI, 0, 1);
   GtpImsi  *pImsi = dynamic_cast<GtpImsi *>(pGtpIe);

   pImsi->setImsi(pImsiKey);

   LOG_EXITVOID();
}
