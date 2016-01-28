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

#include "types.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "macros.hpp"

#include "pugixml.hpp"
using namespace pugi;

#include <iostream>
using namespace std;

#include "gtp_types.hpp"
#include "gtp_macro.hpp"
#include "gtp_util.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"

BOOL gtpIeDict[GTP_IE_MAX] = \
{
   TRUE,
   FALSE
};

GtpIe* GtpIe::createGtpIe(GtpIeType_E  ieType, GtpInstance_t instance)
{
   switch (ieType)
   {
      case GTP_IE_IMSI:
         return new GtpImsi(instance);
      case GTP_IE_MSISDN:
         return new GtpMsisdn(instance);
      case GTP_IE_ULI:
         return new GtpUli(instance);
      case GTP_IE_BEARER_CNTXT:
         return new GtpBearerContext(instance);
      case GTP_IE_FTEID:
         return new GtpFteid(instance);
      case GTP_IE_EBI:
         return new GtpEbi(instance);
      case GTP_IE_MEI:
         return new GtpMei(instance);
      case GTP_IE_RAT_TYPE:
         return new GtpRatType(instance);
      case GTP_IE_SERVING_NW:
         return new GtpServingNw(instance);
      case GTP_IE_APN:
         return new GtpApn(instance);
      case GTP_IE_AMBR:
         return new GtpAmbr(instance);
      case GTP_IE_INDICATION:
         return new GtpIndication(instance);
      case GTP_IE_SELECTION_MODE:
         return new GtpSelectionMode(instance);
      case GTP_IE_PDN_TYPE:
         return new GtpPdnType(instance);
      case GTP_IE_PAA:
         return new GtpPaa(instance);
      default:
         return NULL;
   }
}

/**
 * @brief
 *    Encodes imsi from a 15 digit buffer to GTPv2-c format
 *
 * @param pVal
 *    string containing imsi digits
 *
 * @return
 *    ROK if imsi is encoded successfully
 *    RFAILED otherwise
 */
RETVAL GtpImsi::buildIe(const S8 *pVal)
{
   U32      imsiStrLen = 0;
   U32      indx       = 0;
   U32      len        = 0;

   LOG_ENTERFN(); 

   imsiStrLen = STRLEN(pVal);
   if (imsiStrLen > GTP_IMSI_MAX_DIGITS)
   {
      LOG_ERROR("Invalid no.of IMSI digits [%d]", imsiStrLen);
      LOG_EXITFN(RFAILED);
   }

   /* Enocde even number of digits into hex buffer */
   for (indx = 0; indx < (imsiStrLen - 1); indx += 2)
   {
      m_val[len++] = GSIM_CHAR_TO_DIGIT(pVal[indx]) | \
            GSIM_CHAR_TO_DIGIT(pVal[indx + 1]) << 4;
   }

   /* Encode the last digit into hex buffer */
   if (GSIM_IS_ODD(imsiStrLen))
   {
      m_val[len] = GSIM_CHAR_TO_DIGIT(pVal[imsiStrLen - 1]) | 0xf0;
      len++;
   }

   this->hdr.len = len;
   LOG_EXITFN(ROK);
}

RETVAL GtpImsi::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);
   this->hdr.len = pBuf->len;

   LOG_EXITFN(ROK);
}

/**
 * @brief 
 *
 * @param pBuf
 *    Buffer containing hex representation of imsi
 *
 * @return
 *    ROK if imsi is encoded successfully
 *    RFAILED otherwise
 */
RETVAL GtpImsi::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_IMSI_MAX_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid IMSI Hex Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief
 *    Enocdes the IMSI into U8 buffer
 *
 * @param pBuf
 * @param pLen
 *
 * @return 
 */
RETVAL GtpImsi::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}


VOID GtpImsi::setImsi(GtpImsiKey *pImsi)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pImsi->val, pImsi->len);
   this->hdr.len = pImsi->len;

   LOG_EXITVOID();
}

/**
 * @brief
 *    Encodes msisdn from a 15 digit buffer to GTPv2-c format
 *
 * @param pVal
 *    string containing msisdn digits
 *
 * @return
 *    ROK if msisdn is encoded successfully
 *    RFAILED otherwise
 */
RETVAL GtpMsisdn::buildIe(const S8 *pVal)
{
   LOG_ENTERFN(); 

   U32      msisdnStrLen = 0;
   U32      indx         = 0;
   U32      len          = 0;

   msisdnStrLen = STRLEN(pVal);
   if (msisdnStrLen > GTP_MSISDN_MAX_DIGITS)
   {
      LOG_ERROR("Invalid no.of MSISDN digits [%d]", msisdnStrLen);
      LOG_EXITFN(RFAILED);
   }

   /* Enocde even number of digits into hex buffer */
   for (indx = 0; indx < (msisdnStrLen - 1); indx += 2)
   {
      m_val[len++] = GSIM_CHAR_TO_DIGIT(pVal[indx]) | \
            GSIM_CHAR_TO_DIGIT(pVal[indx + 1]) << 4;
   }

   /* Encode the last digit into hex buffer */
   if (GSIM_IS_ODD(msisdnStrLen))
   {
      m_val[len] = GSIM_CHAR_TO_DIGIT(pVal[msisdnStrLen - 1]) | 0xf0;
      len++;
   }

   this->hdr.len = len;

   LOG_EXITFN(ROK);
}

RETVAL GtpMsisdn::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);

   LOG_EXITFN(ROK);
}

/**
 * @brief 
 *
 * @param pBuf
 *    Buffer containing hex representation of msisidn
 *
 * @return
 *    ROK if msisdn is encoded successfully
 *    RFAILED otherwise
 */
RETVAL GtpMsisdn::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_MSISDN_MAX_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid MSISDN Hex Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief
 *    Enocdes the MSISDN into U8 buffer
 *
 * @param pBuf
 * @param pLen
 *
 * @return 
 */
RETVAL GtpMsisdn::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}


RETVAL GtpUli::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);
   this->hdr.len = pBuf->len;

   LOG_EXITFN(ROK);
}

/**
 * @brief 
 *
 * @param pBuf
 *    Buffer containing hex representation of uli 
 *
 * @return
 *    ROK if uli is encoded successfully
 *    RFAILED otherwise
 */
RETVAL GtpUli::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL  ret = ROK;

   if (GTP_ULI_MAX_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid ULI Hex Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

RETVAL GtpUli::buildIe(XmlBufferLst *pBufLst)
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   U8    *pBuf = m_val + 1;   /* skip the first byte for ULI parameter flags */
   for (XmlBufferLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      XmlBuffer   *pXmlBuf = *b;

      if (STRCASECMP(pXmlBuf->paramName, "cgi") == 0)
      {
         GSIM_SET_MASK(m_pres, GTP_ULI_CGI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "sai") == 0)
      {
         GSIM_SET_MASK(m_pres, GTP_ULI_SAI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "rai") == 0)
      {
         GSIM_SET_MASK(m_pres, GTP_ULI_RAI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "tai") == 0)
      {
         GSIM_SET_MASK(m_pres, GTP_ULI_TAI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "ecgi") == 0)
      {
         GSIM_SET_MASK(m_pres, GTP_ULI_ECGI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "lai") == 0)
      {
         GSIM_SET_MASK(m_pres, GTP_ULI_LAI_PRESENT);
      }
      else
      {
         LOG_ERROR("Invalid ULI Parameter type [%s]",\
               pXmlBuf->paramName);
      }

      U32 len = gtpConvStrToHex(&pXmlBuf->buf, pBuf);
      pBuf += len;
      this->hdr.len += len;

      delete *b;
   }

   /* encoding 1 byte of ULI flags */
   GSIM_ENC_U8(m_val, m_pres);
   this->hdr.len += 1;

   delete pBufLst;
   LOG_EXITFN(ret);
}

RETVAL GtpUli::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}

GtpBearerContext::~GtpBearerContext()
{
   for (GtpIeLstItr itr = m_ieLst.begin(); itr != m_ieLst.end(); itr++)
   {
      delete *itr;
   }

   delete[] m_pVal;
}

RETVAL GtpBearerContext::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   m_pVal = new U8 [pBuf->len];
   MEMCPY(m_pVal, pBuf->pVal, pBuf->len);
   this->hdr.len = pBuf->len;

   LOG_EXITFN(ROK);
}

VOID GtpBearerContext::setGtpuTeid(GtpTeid_t teid, GtpInstance_t inst)
{
   LOG_ENTERFN();

   GtpIeHdr    ieHdr;
   U8          *pBuf = m_pVal;
   GtpLength_t ieLen = this->hdr.len;

   while (ieLen)
   {
      decIeHdr(pBuf, &ieHdr);
      if (ieHdr.ieType == GTP_IE_FTEID && ieHdr.instance == inst)
      {
         pBuf += GTP_IE_HDR_LEN;
         GTP_ENC_TEID((pBuf + 1), teid);
         break;
      }

      ieLen -= (ieHdr.len + GTP_IE_HDR_LEN);
      pBuf += (ieHdr.len + GTP_IE_HDR_LEN);
   }

   LOG_EXITVOID();
}

GtpEbi_t GtpBearerContext::getEbi()
{
   LOG_ENTERFN();

   GtpEbi_t ebi = 0;

   U8 *pBuf = getIeBufPtr(m_pVal, this->hdr.len, GTP_IE_EBI, 0, 1);
   if (NULL != pBuf)
   {
      GTP_DEC_EBI(pBuf, ebi);
   }
   else
   {
      LOG_ERROR("EBI Ie not found, Instance [0]");
   }

   LOG_EXITFN(ebi);
}

RETVAL GtpBearerContext::buildIe(const HexString *value)
{
   RETVAL  ret = ROK;

   LOG_ENTERFN();

   try
   {
      U32 hexBufLen = GSIM_CEIL_DIVISION(value->size(), 2);
      m_pVal = new U8 [hexBufLen];
      this->hdr.len = gtpConvStrToHex(value, m_pVal);
   }
   catch (exception &e)
   {
      ret = ERR_MEMORY_ALLOC;
   }

   LOG_EXITFN(ret);
}

RETVAL GtpBearerContext::buildIe(const GtpIeLst *pIeLst)
{
   LOG_ENTERFN();

   RETVAL                     ret = ROK;
   U32                        len = 0;
   U8                         buf[GTP_BEARER_CNTXT_MAX_LEN];
   U8                         *pBuf = &buf[0];
   GtpIeLst::const_iterator  ie;

   LOG_DEBUG("[%d] IEs to encode", pIeLst->size());
   MEMSET(buf, 0, GTP_BEARER_CNTXT_MAX_LEN); 
   for (ie = pIeLst->begin(); ie != pIeLst->end(); ie++)
   {
      (*ie)->encode(pBuf, &len);
      pBuf += len;
      this->hdr.len += len;
      len = 0;

      delete *ie;
   }

   m_pVal = new U8 [this->hdr.len];
   MEMCPY(m_pVal, buf, this->hdr.len);

   LOG_EXITFN(ret);
}

RETVAL GtpBearerContext::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_pVal, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}

RETVAL GtpFteid::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);
   this->hdr.len = pBuf->len;
   GTP_DEC_TEID((m_val + 1), m_teid);

   LOG_EXITFN(ROK);
}

RETVAL GtpFteid::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (GTP_FTEID_MAX_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid FTEID Hex length");
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

RETVAL GtpFteid::buildIe(XmlBufferLst *pBufLst)
{
   LOG_ENTERFN();

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (XmlBufferLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      XmlBuffer   *pXmlBuf = *b;

      if (STRCASECMP(pXmlBuf->paramName, "iftype") == 0)
      {
         GtpIfType_E ifType = gtpConvStrToIfType((const S8*)pXmlBuf->buf.pVal,\
               pXmlBuf->buf.len);
         GSIM_ENC_U8(m_val, ifType);
         this->hdr.len += 1;
      }
      else if (STRCASECMP(pXmlBuf->paramName, "ipv4") == 0)
      {
         GSIM_SET_MASK(m_val[0], GTP_FTEID_IPV4_ADDR_PRESENT);
         IpAddr ipAddr = convIpStrToIpAddr((const S8*)pXmlBuf->buf.pVal,\
               pXmlBuf->buf.len);
         GTP_ENC_IPV4_ADDR((m_val + 5), ipAddr.u.ipv4Addr.addr);
         this->hdr.len += IPV4_ADDR_MAX_LEN;
      }
      else if (STRCASECMP(pXmlBuf->paramName, "ipv6") == 0)
      {
         GSIM_SET_MASK(m_val[0], GTP_FTEID_IPV6_ADDR_PRESENT);
         IpAddr ipAddr = convIpStrToIpAddr((const S8*)pXmlBuf->buf.pVal,\
               pXmlBuf->buf.len);
         GTP_ENC_IPV6_ADDR((m_val + 9), ipAddr.u.ipv6Addr.addr);
         this->hdr.len += IPV6_ADDR_MAX_LEN;
      }
      else if (STRCASECMP(pXmlBuf->paramName, "teid") == 0)
      {
         GtpTeid_t teid = (GtpTeid_t)gtpConvStrToU32(\
               (const S8*)pXmlBuf->buf.pVal, pXmlBuf->buf.len);
         GTP_ENC_TEID((m_val + 1), teid);
         this->hdr.len += 4;
      }
      else if (STRCASECMP(pXmlBuf->paramName, "grekey") == 0)
      {
         GtpTeid_t greKey = (GtpTeid_t)gtpConvStrToU32(\
               (const S8*)pXmlBuf->buf.pVal, pXmlBuf->buf.len);
         GTP_ENC_TEID((m_val + 1), greKey);
         this->hdr.len += 4;
      }
      else
      {
         LOG_ERROR("Invalid ULI Parameter type [%s]", pXmlBuf->paramName);
      }

      delete *b;
   }

   delete pBufLst;
   LOG_EXITFN(ROK);
}

RETVAL GtpFteid::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}

VOID GtpFteid::setTeid(GtpTeid_t teid)
{
   LOG_ENTERFN();

   U8 *pBuf = &m_val[1];
   GTP_ENC_TEID(pBuf, teid);

   LOG_EXITVOID();
}

VOID GtpFteid::setIpAddr(const IpAddr *pIp)
{
   LOG_ENTERFN();

   U8 *pBuf = &m_val[5];
   GTP_ENC_IPV4_ADDR(pBuf, pIp->u.ipv4Addr.addr);

   LOG_EXITVOID();
}

RETVAL GtpEbi::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();
   
   m_ebi = (U8)gtpConvStrToU32((const S8*)pVal, STRLEN(pVal));
   this->hdr.len = STRLEN(pVal);

   LOG_EXITFN(ROK);
}

RETVAL GtpEbi::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   GTP_DEC_EBI(pBuf->pVal, m_ebi);
   this->hdr.len = 1;

   LOG_EXITFN(ROK);
}

RETVAL GtpEbi::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;

   GTP_ENC_EBI(pTmpBuf, m_ebi);
   *pLen = this->hdr.len + GTP_IE_HDR_LEN;

   LOG_EXITFN(ROK);
}

/**
 * @brief
 *    Returns the buffer pointer at which the IE indicated by ietype, instance
 *    and occurance count exists in Gtp Grouped IE buffer
 *
 * @param ieType
 * @param inst
 * @param occr
 *
 * @return
 *    NULL if ie does not exist, otherwise the buffer pointer is returned
 */
U8* GtpIe::getIeBufPtr
(
U8                *pBuf,
GtpLength_t       len,
GtpIeType_E       ieType,\
GtpInstance_t     inst,
U32               occr
)
{
   LOG_ENTERFN();

   U8          *pIeBufPtr = NULL;
   GtpIeHdr    ieHdr;
   U32         cnt = 0;

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


/**
 * @brief
 *    Encodes mei from a 15/16 digit buffer to GTPv2-c format
 *
 * @param pVal
 *    string containing mei digits
 *
 * @return
 *    ROK if mei is encoded successfully
 *    RFAILED otherwise
 */
RETVAL GtpMei::buildIe(const S8 *pVal)
{
   LOG_ENTERFN(); 

   U32      meiStrLen = 0;
   U32      indx         = 0;
   U32      len          = 0;

   meiStrLen = STRLEN(pVal);
   if (meiStrLen > GTP_MEI_MAX_DIGITS)
   {
      LOG_ERROR("Invalid no.of MEI digits [%d]", meiStrLen);
      LOG_EXITFN(RFAILED);
   }

   /* Enocde even number of digits into hex buffer */
   for (indx = 0; indx < (meiStrLen - 1); indx += 2)
   {
      m_val[len++] = GSIM_CHAR_TO_DIGIT(pVal[indx]) | \
            GSIM_CHAR_TO_DIGIT(pVal[indx + 1]) << 4;
   }

   /* Encode the last digit into hex buffer */
   if (GSIM_IS_ODD(meiStrLen))
   {
      m_val[len] = GSIM_CHAR_TO_DIGIT(pVal[meiStrLen - 1]) | 0xf0;
      len++;
   }

   this->hdr.len = len;

   LOG_EXITFN(ROK);
}

RETVAL GtpMei::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);

   LOG_EXITFN(ROK);
}

/**
 * @brief 
 *
 * @param pBuf
 *    Buffer containing hex representation of msisidn
 *
 * @return
 *    ROK if mei is encoded successfully
 *    RFAILED otherwise
 */
RETVAL GtpMei::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_MEI_MAX_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid MEI Hex Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief
 *    Enocdes the MEI into U8 buffer
 *
 * @param pBuf
 * @param pLen
 *
 * @return 
 */
RETVAL GtpMei::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}


/**
 * @brief encodes RAT type from type of rat represented in integer string
 *
 * @param pVal
 *
 * @return 
 */
RETVAL GtpRatType::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();
   
   m_ratType = (GtpRatType_E)gtpConvStrToU32((const S8*)pVal, STRLEN(pVal));
   this->hdr.len = STRLEN(pVal);

   LOG_EXITFN(ROK);
}

RETVAL GtpRatType::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   GTP_DEC_RAT_TYPE(pBuf->pVal, m_ratType);
   this->hdr.len = 1;

   LOG_EXITFN(ROK);
}

RETVAL GtpRatType::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;

   GTP_ENC_RAT_TYPE(pTmpBuf, m_ratType);
   *pLen = this->hdr.len + GTP_IE_HDR_LEN;

   LOG_EXITFN(ROK);
}


RETVAL GtpServingNw::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();
   
   if (NULL == pVal || (0 == STRLEN(pVal)))
   {
      LOG_ERROR("NULL function parameter")
   }

   if (STRLEN(pVal) > GTP_SERVING_NW_MAX_STR_LEN)
   {
      LOG_ERROR("Invalid Serving Network")
   }

   m_plmnId.mcc[0] = GSIM_CHAR_TO_DIGIT(pVal[0]);
   m_plmnId.mcc[1] = GSIM_CHAR_TO_DIGIT(pVal[1]);
   m_plmnId.mcc[2] = GSIM_CHAR_TO_DIGIT(pVal[2]);

   m_plmnId.mnc[0] = GSIM_CHAR_TO_DIGIT(pVal[3]);
   m_plmnId.mnc[1] = GSIM_CHAR_TO_DIGIT(pVal[4]);

   if (STRLEN(pVal) == GTP_SERVING_NW_MAX_STR_LEN)
   {
      m_plmnId.mnc[2] = GSIM_CHAR_TO_DIGIT(pVal[5]);
      m_plmnId.numMncDigits = 3; 
   }
   else
   {
      m_plmnId.numMncDigits = 2;
   }

   gtpUtlEncPlmnId(&m_plmnId, m_val);
   hdr.len = GTP_SERVING_NW_MAX_BUF_LEN;

   LOG_EXITFN(ROK);
}

RETVAL GtpServingNw::buildIe(const HexString *value)
{
   LOG_ENTERFN();
   
   RETVAL  ret = ROK;

   if (GTP_SERVING_NW_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Serving Network Hex Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

RETVAL GtpServingNw::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}

RETVAL GtpServingNw::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);
   this->hdr.len = pBuf->len;

   LOG_EXITFN(ROK);
}

RETVAL GtpApn::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();
   
   if (NULL == pVal || (0 == STRLEN(pVal)))
   {
      LOG_ERROR("NULL function parameter")
   }

   if (STRLEN(pVal) > GTP_APN_MAX_LEN)
   {
      LOG_ERROR("Invalid APN length")
   }

   hdr.len = STRLEN(pVal);
   MEMCPY(m_val, pVal, hdr.len);

   LOG_EXITFN(ROK);
}

RETVAL GtpApn::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}

RETVAL GtpApn::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);
   this->hdr.len = pBuf->len;

   LOG_EXITFN(ROK);
}

RETVAL GtpAmbr::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_IMSI_MAX_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Ambr Hex Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


RETVAL GtpAmbr::buildIe(XmlBufferLst *pBufLst)
{
   LOG_ENTERFN();

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (XmlBufferLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      XmlBuffer   *pXmlBuf = *b;

     if (STRCASECMP(pXmlBuf->paramName, "ul") == 0)
      {
         GtpApnAmbr_t ambrul = (GtpApnAmbr_t)gtpConvStrToU32(\
               (const S8*)pXmlBuf->buf.pVal, pXmlBuf->buf.len);
         GTP_ENC_APN_AMBR(m_val, ambrul);
         this->hdr.len += 4;
      }
      else if (STRCASECMP(pXmlBuf->paramName, "dl") == 0)
      {
         GtpApnAmbr_t ambrdl = (GtpApnAmbr_t)gtpConvStrToU32(\
               (const S8*)pXmlBuf->buf.pVal, pXmlBuf->buf.len);
         GTP_ENC_APN_AMBR((m_val + 4), ambrdl);
         this->hdr.len += 4;
      }
      else
      {
         LOG_ERROR("Invalid Ambr Parameter type [%s]", pXmlBuf->paramName);
      }

      delete *b;
   }

   delete pBufLst;
   LOG_EXITFN(ROK);
}

RETVAL GtpAmbr::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}

RETVAL GtpIndication::buildIe(XmlBufferLst *pBufLst)
{
   LOG_ENTERFN();

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }


   for (XmlBufferLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      XmlBuffer   *pXmlBuf = *b;

      if (STRCASECMP(pXmlBuf->paramName, "daf") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_DAF_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "dtf") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_DTF_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "dfi") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_DFI_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "oi") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_OI_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "isrsi") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_ISRSI_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "israi") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_ISRAI_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "sgwci") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_SGWCI_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "sqci") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_SQCI_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "uimsi") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_UIMSI_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "cfsi") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_CFSI_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "p") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_P_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "pt") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_PT_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "si") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_SI_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "msv") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_MSV_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "israu") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_ISRAU_PRES);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "ccrsi") == 0)
      {
         GSIM_SET_MASK(bitmask, GTP_INDICATION_CCRSI_PRES);
      }
      else
      {
         LOG_ERROR("Invalid indication Parameter type [%s]",\
               pXmlBuf->paramName);
      }

      delete *b;
   }

   delete pBufLst;
   LOG_EXITFN(ROK);
}


RETVAL GtpAmbr::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);
   this->hdr.len = pBuf->len;

   LOG_EXITFN(ROK);
}


RETVAL GtpIndication::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_INDICATION_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Indicaiton IE Hex Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


RETVAL GtpIndication::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}

RETVAL GtpIndication::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);
   this->hdr.len = pBuf->len;

   LOG_EXITFN(ROK);
}

RETVAL GtpSelectionMode::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();

   m_selMode = (U8)gtpConvStrToU32((const S8*)pVal, STRLEN(pVal));
   this->hdr.len = STRLEN(pVal);

   LOG_EXITFN(ROK);
}

RETVAL GtpSelectionMode::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;

   GTP_ENC_SEL_MODE(pTmpBuf, m_selMode);
   *pLen = this->hdr.len + GTP_IE_HDR_LEN;

   LOG_EXITFN(ROK);
}

RETVAL GtpSelectionMode::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   m_selMode = pBuf->pVal[0];
   this->hdr.len = GTP_SEL_MODE_BUF_MAX_LEN;

   LOG_EXITFN(ROK);
}

RETVAL GtpPdnType::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();

   if (NULL == pVal || STRLEN(pVal) == 0)
   {
      LOG_DEBUG("Pdn Type string is NULL or 0 size");
      return RFAILED;
   }

   if (STRCASECMP(pVal, "ipv4"))
   {
      m_pdnType = GTP_PDN_TYPE_IPV4;
   }
   else if (STRCASECMP(pVal, "ipv6"))
   {
      m_pdnType = GTP_PDN_TYPE_IPV6;
   }
   else if (STRCASECMP(pVal, "ipv4v6"))
   {
      m_pdnType = GTP_PDN_TYPE_IPV4V6;
   }
   else
   {
      LOG_DEBUG("Invalid PDN Type [%s]", pVal);
      LOG_EXITFN(RFAILED);
   }

   this->hdr.len = GTP_PDN_TYPE_MAX_BUF_LEN;

   LOG_EXITFN(ROK);
}

RETVAL GtpPdnType::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   m_pdnType = (GtpPdnType_E)(pBuf->pVal[0] & 0x0007);
   this->hdr.len = GTP_SEL_MODE_BUF_MAX_LEN;

   LOG_EXITFN(ROK);
}

RETVAL GtpPdnType::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;

   GTP_ENC_PDN_TYPE(pTmpBuf, m_pdnType);
   *pLen = this->hdr.len + GTP_IE_HDR_LEN;

   LOG_EXITFN(ROK);
}


RETVAL GtpPaa::decode(const Buffer *pBuf)
{
   LOG_ENTERFN();

   MEMCPY(m_val, pBuf->pVal, pBuf->len);
   this->hdr.len = pBuf->len;

   LOG_EXITFN(ROK);
}

RETVAL GtpPaa::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_PAA_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid PAA Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


RETVAL GtpPaa::buildIe(XmlBufferLst *pBufLst)
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (XmlBufferLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      XmlBuffer   *pXmlBuf = *b;

      if (STRCASECMP(pXmlBuf->paramName, "pdn_type") == 0)
      {
         if (STRCASECMP((const S8 *)pXmlBuf->buf.pVal, "ipv4"))
         {
            GTP_ENC_PDN_TYPE(m_val, GTP_PDN_TYPE_IPV4);
         }
         else if (STRCASECMP((const S8 *)pXmlBuf->buf.pVal, "ipv6"))
         {
            GTP_ENC_PDN_TYPE(m_val, GTP_PDN_TYPE_IPV6);
         }
         else if (STRCASECMP((const S8 *)pXmlBuf->buf.pVal, "ipv4v6"))
         {
            GTP_ENC_PDN_TYPE(m_val, GTP_PDN_TYPE_IPV4V6);
         }
         else
         {
            LOG_DEBUG("Invalid PDN Type [%s]", pXmlBuf->buf.pVal);
            ret = RFAILED;
            break;
         }
      }
      else if (STRCASECMP(pXmlBuf->paramName, "ipv4") == 0)
      {
         IpAddr ipAddr = convIpStrToIpAddr((const S8 *)pXmlBuf->buf.pVal,\
               pXmlBuf->buf.len);
         GTP_ENC_IPV4_ADDR((m_val + 1), ipAddr.u.ipv4Addr.addr);
         this->hdr.len += IPV4_ADDR_MAX_LEN;
      }
      else if (STRCASECMP(pXmlBuf->paramName, "ipv6") == 0)
      {
         IpAddr ipAddr = convIpStrToIpAddr((const S8 *)pXmlBuf->buf.pVal,\
               pXmlBuf->buf.len);
         GTP_ENC_IPV6_ADDR((m_val + 1), ipAddr.u.ipv6Addr.addr);
         this->hdr.len += IPV6_ADDR_MAX_LEN;
      }
      else if (STRCASECMP(pXmlBuf->paramName, "ipv4v6") == 0)
      {
      }
      else
      {
         LOG_ERROR("PAA Parameter type [%s]", pXmlBuf->paramName);
      }

      delete *b;
   }

   delete pBufLst;
   LOG_EXITFN(ret);
}

RETVAL GtpPaa::encode(U8 *pBuf, U32 *pLen)
{
   LOG_ENTERFN();

   gtpEncIeUsingHexBuf(m_val, &this->hdr, pBuf, pLen);

   LOG_EXITFN(ROK);
}
