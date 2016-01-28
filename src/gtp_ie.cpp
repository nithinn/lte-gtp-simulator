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

GtpLength_t GtpImsi::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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
GtpLength_t GtpImsi::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
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

GtpLength_t GtpMsisdn::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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
GtpLength_t GtpMsisdn::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


GtpLength_t GtpUli::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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

RETVAL GtpUli::buildIe(IeParamLst *pBufLst)
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   U8    pres = 0;
   U8    *pBuf = m_val + 1;   /* skip the first byte for ULI parameter flags */
   for (IeParamLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      IeParam   *pXmlBuf = *b;

      if (STRCASECMP(pXmlBuf->paramName, "cgi") == 0)
      {
         GSIM_SET_MASK(pres, GTP_ULI_CGI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "sai") == 0)
      {
         GSIM_SET_MASK(pres, GTP_ULI_SAI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "rai") == 0)
      {
         GSIM_SET_MASK(pres, GTP_ULI_RAI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "tai") == 0)
      {
         GSIM_SET_MASK(pres, GTP_ULI_TAI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "ecgi") == 0)
      {
         GSIM_SET_MASK(pres, GTP_ULI_ECGI_PRESENT);
      }
      else if (STRCASECMP(pXmlBuf->paramName, "lai") == 0)
      {
         GSIM_SET_MASK(pres, GTP_ULI_LAI_PRESENT);
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
   GSIM_ENC_U8(m_val, pres);
   this->hdr.len += 1;

   delete pBufLst;
   LOG_EXITFN(ret);
}

GtpLength_t GtpUli::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

GtpLength_t GtpBearerContext::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

VOID GtpBearerContext::setGtpuTeid(GtpTeid_t teid, GtpInstance_t inst)
{
   LOG_ENTERFN();

   GtpIeHdr    ieHdr;
   U8          *pBuf = m_val;
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

   U8 *pBuf = getIeBufPtr(m_val, this->hdr.len, GTP_IE_EBI, 0, 1);
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
      if (GTP_BEARER_CNTXT_MAX_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
      {
         this->hdr.len = gtpConvStrToHex(value, m_val);
      }
      else
      {
         ret = RFAILED;
      }
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
   U8                         buf[GTP_BEARER_CNTXT_MAX_LEN];
   U8                         *pBuf = &buf[0];
   GtpIeLst::const_iterator  ie;

   LOG_DEBUG("[%d] IEs to encode", pIeLst->size());
   MEMSET(buf, 0, GTP_BEARER_CNTXT_MAX_LEN); 
   for (ie = pIeLst->begin(); ie != pIeLst->end(); ie++)
   {
      U32 len = (*ie)->encode(pBuf);
      pBuf += len;
      this->hdr.len += len;

      delete *ie;
   }

   MEMCPY(m_val, buf, this->hdr.len);

   LOG_EXITFN(ret);
}

GtpLength_t GtpBearerContext::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

GtpLength_t GtpFteid::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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

RETVAL GtpFteid::buildIe(IeParamLst *pBufLst)
{
   LOG_ENTERFN();

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      IeParam   *pXmlBuf = *b;

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

GtpLength_t GtpFteid::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
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

/**
 * @brief
 *    Decodes FTEID value and returns the teid
 *
 * @return
 *    teid value
 */
GtpTeid_t GtpFteid::getTeid()
{
   LOG_ENTERFN();

   GtpTeid_t   id = 0;
   GTP_DEC_TEID(m_val + 1, id);

   LOG_EXITFN(id);
}

RETVAL GtpEbi::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();
   
   m_val = (U8)gtpConvStrToU32((const S8*)pVal, STRLEN(pVal));
   this->hdr.len = STRLEN(pVal);

   LOG_EXITFN(ROK);
}

GtpLength_t GtpEbi::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   m_val = pBuf[GTP_IE_HDR_LEN];
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

GtpLength_t GtpEbi::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;

   GTP_ENC_EBI(pTmpBuf, m_val);
   LOG_EXITFN(this->hdr.len + GTP_IE_HDR_LEN);
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

GtpLength_t GtpMei::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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
GtpLength_t GtpMei::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
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
   
   m_val = (U8)gtpConvStrToU32((const S8*)pVal, STRLEN(pVal));
   this->hdr.len = STRLEN(pVal);

   LOG_EXITFN(ROK);
}

GtpLength_t GtpRatType::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   m_val = pBuf[GTP_IE_HDR_LEN];
   this->hdr.len = ieLen;
   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

GtpLength_t GtpRatType::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;

   GTP_ENC_RAT_TYPE(pTmpBuf, m_val);

   LOG_EXITFN(this->hdr.len + GTP_IE_HDR_LEN);
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

   GtpPlmnId_t plmnId;
   MEMSET(&plmnId, 0, sizeof(GtpPlmnId_t));

   plmnId.mcc[0] = GSIM_CHAR_TO_DIGIT(pVal[0]);
   plmnId.mcc[1] = GSIM_CHAR_TO_DIGIT(pVal[1]);
   plmnId.mcc[2] = GSIM_CHAR_TO_DIGIT(pVal[2]);

   plmnId.mnc[0] = GSIM_CHAR_TO_DIGIT(pVal[3]);
   plmnId.mnc[1] = GSIM_CHAR_TO_DIGIT(pVal[4]);

   if (STRLEN(pVal) == GTP_SERVING_NW_MAX_STR_LEN)
   {
      plmnId.mnc[2] = GSIM_CHAR_TO_DIGIT(pVal[5]);
      plmnId.numMncDigits = 3; 
   }
   else
   {
      plmnId.numMncDigits = 2;
   }

   gtpUtlEncPlmnId(&plmnId, m_val);
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

GtpLength_t GtpServingNw::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

GtpLength_t GtpServingNw::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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

GtpLength_t GtpApn::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

GtpLength_t GtpApn::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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


RETVAL GtpAmbr::buildIe(IeParamLst *pBufLst)
{
   LOG_ENTERFN();

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      IeParam   *pXmlBuf = *b;

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

GtpLength_t GtpAmbr::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

RETVAL GtpIndication::buildIe(IeParamLst *pBufLst)
{
   LOG_ENTERFN();

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   U32 bitmask = 0;
   for (IeParamLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      IeParam   *pXmlBuf = *b;

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

   GSIM_ENC_3B(m_val, bitmask);
   this->hdr.len += GTP_INDICATION_MAX_BUF_LEN;

   delete pBufLst;
   LOG_EXITFN(ROK);
}


GtpLength_t GtpAmbr::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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


GtpLength_t GtpIndication::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

GtpLength_t GtpIndication::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

RETVAL GtpSelectionMode::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();

   m_val = (U8)gtpConvStrToU32((const S8*)pVal, STRLEN(pVal));
   this->hdr.len = STRLEN(pVal);

   LOG_EXITFN(ROK);
}

GtpLength_t GtpSelectionMode::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;

   GTP_ENC_SEL_MODE(pTmpBuf, m_val);

   LOG_EXITFN(this->hdr.len + GTP_IE_HDR_LEN);
}

GtpLength_t GtpSelectionMode::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   m_val = pBuf[GTP_IE_HDR_LEN];
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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
      m_val = (U8)GTP_PDN_TYPE_IPV4;
   }
   else if (STRCASECMP(pVal, "ipv6"))
   {
      m_val = (U8)GTP_PDN_TYPE_IPV6;
   }
   else if (STRCASECMP(pVal, "ipv4v6"))
   {
      m_val = (U8)GTP_PDN_TYPE_IPV4V6;
   }
   else
   {
      LOG_DEBUG("Invalid PDN Type [%s]", pVal);
      LOG_EXITFN(RFAILED);
   }

   this->hdr.len = GTP_PDN_TYPE_MAX_BUF_LEN;

   LOG_EXITFN(ROK);
}

GtpLength_t GtpPdnType::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   m_val = pBuf[GTP_IE_HDR_LEN];
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

GtpLength_t GtpPdnType::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   GTP_ENC_PDN_TYPE(pTmpBuf, m_val);

   LOG_EXITFN(this->hdr.len + GTP_IE_HDR_LEN);
}


GtpLength_t GtpPaa::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
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


RETVAL GtpPaa::buildIe(IeParamLst *pBufLst)
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (pBufLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = pBufLst->begin(); b != pBufLst->end(); b++)
   {
      IeParam   *pXmlBuf = *b;

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

GtpLength_t GtpPaa::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}
