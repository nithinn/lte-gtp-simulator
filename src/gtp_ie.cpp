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
#include <string>

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

/**
 * @brief Factor Method to create GTP IE
 *
 * @param ieType
 * @param instance
 *
 * @return 
 */
GtpIe* GtpIe::createGtpIe(GtpIeType_E  ieType, GtpInstance_t instance)
{
   switch (ieType)
   {
      case GTP_IE_IMSI:
         return new GtpImsi(instance);
      case GTP_IE_CAUSE:
         return new GtpCause(instance);
      case GTP_IE_RECOVERY:
         return new GtpRecovery(instance);
      case GTP_IE_STN_SR:
         return new GtpStnSr(instance);
      case GTP_IE_APN:
         return new GtpApn(instance);
      case GTP_IE_AMBR:
         return new GtpAmbr(instance);
      case GTP_IE_EBI:
         return new GtpEbi(instance);
      case GTP_IE_IP_ADDR:
         return new GtpIpAddress(instance);
      case GTP_IE_MEI:
         return new GtpMei(instance);
      case GTP_IE_MSISDN:
         return new GtpMsisdn(instance);
      case GTP_IE_INDICATION:
         return new GtpIndication(instance);
      case GTP_IE_PCO:
         return new GtpPco(instance);
      case GTP_IE_PAA:
         return new GtpPaa(instance);
      case GTP_IE_BEARER_QOS:
         return new GtpBearerQos(instance);
      case GTP_IE_FLOW_QOS:
         return new GtpFlowQos(instance);
      case GTP_IE_RAT_TYPE:
         return new GtpRatType(instance);
      case GTP_IE_SERVING_NW:
         return new GtpServingNw(instance);
      case GTP_IE_EPS_BEARER_TFT:
         return new GtpEpsBearerTft(instance);
      case GTP_IE_TAD:
         return new GtpTad(instance);
      case GTP_IE_ULI:
         return new GtpUli(instance);
      case GTP_IE_FTEID:
         return new GtpFteid(instance);
      case GTP_IE_TMSI:
         return new GtpTmsi(instance);
      case GTP_IE_GLOBAL_CN_ID:
         return new GtpGlobalCnId(instance);
      case GTP_IE_S103_PDF:
         return new GtpS103Pdf(instance);
      case GTP_IE_S1UDF:
         return new GtpS1uDf(instance);
      case GTP_IE_DELAY_VALUE:
         return new GtpDelayValue(instance);
      case GTP_IE_BEARER_CNTXT:
         return new GtpBearerContext(instance);
      case GTP_IE_CHARGING_ID:
         return new GtpChargingId(instance);
      case GTP_IE_CHARGING_CHARACTERISTICS:
         return new GtpChargingCharcs(instance);
      case GTP_IE_TRACE_INFO:
         return new GtpTraceInfo(instance);
      case GTP_IE_BEARER_FLAGS:
         return new GtpBearerFlags(instance);
      case GTP_IE_PDN_TYPE:
         return new GtpPdnType(instance);
      case GTP_IE_PTI:
         return new GtpPti(instance);
      case GTP_IE_DRX_PARAM:
         return new GtpDrxParam(instance);
      case GTP_IE_UE_NETWORK_CAP:
         return new GtpUeNetworkCap(instance);
      case GTP_IE_MM_CNTXT_GSM_KEY_N_TRIPLETS:
         return new GtpMmCntxtGsmKeyAndTriplets(instance);
      case GTP_IE_MM_CNTXT_UMTS_KEY_USED_CIPHER_N_QUINT:
         return new GtpMmCntxtUmtsKeyUsedCipherAndQuint(instance);
      case GTP_IE_MM_CNTXT_GSM_KEY_USED_CIPHER_N_QUINT:
         return new GtpMmCntxtGsmKeyUsedCipherAndQuint(instance);
      case GTP_IE_MM_CNTXT_UMTS_KEY_N_QUINT:
         return new GtpMmCntxtUmtsKeyAndQuint(instance);
      case GTP_IE_MM_CNTXT_EPS_SEC_CNTXT_QUADR_N_QUINT:
         return new GtpMmCntxtEpcSecCntxtQuadrAndQuint(instance);
      case GTP_IE_MM_CNTXT_UMTS_KEY_QUADR_N_QUINT:
         return new GtpMmCntxtUmtsKeyQuadrAndQuint(instance);
      case GTP_IE_PDN_CONNECTION:
         return new GtpPdnConnection(instance);
      case GTP_IE_PDU_NUMBERS:
         return new GtpPduNumbers(instance);
      case GTP_IE_PTMSI:
         return new GtpPtmsi(instance);
      case GTP_IE_PTMSI_SIGNATURE:
         return new GtpPtmsiSignature(instance);
      case GTP_IE_HOP_COUNTER:
         return new GtpHopCounter(instance);
      case GTP_IE_UE_TIME_ZONE:
         return new GtpUeTimeZone(instance);
      case GTP_IE_TRACE_REFERENCE:
         return new GtpTraceReference(instance);
      case GTP_IE_COMPLETE_REQ_MSG:
         return new GtpCompleteReqMsg(instance);
      case GTP_IE_GUTI:
         return new GtpGuti(instance);
      case GTP_IE_FCONTAINER:
         return new GtpFContainer(instance);
      case GTP_IE_FCAUSE:
         return new GtpFCause(instance);
      case GTP_IE_SELECTED_PLMN_ID:
         return new GtpSelectedPlmnId(instance);
      case GTP_IE_TARGET_ID:
         return new GtpTargetId(instance);
      case GTP_IE_PACKET_FLOW_ID:
         return new GtpPacketFlowId(instance);
      case GTP_IE_RAB_CNTXT:
         return new GtpRabCntxt(instance);
      case GTP_IE_SOURCE_RNC_PDCP_CNTXT_INFO:
         return new GtpSourceRncPdcpCntxtInfo(instance);
      case GTP_IE_UDP_SRC_PORT:
         return new GtpUdpSrcPort(instance);
      case GTP_IE_APN_RESTRICTION:
         return new GtpApnRestriction(instance);
      case GTP_IE_SELECTION_MODE:
         return new GtpSelectionMode(instance);
      case GTP_IE_SRC_ID:
         return new GtpSrcId(instance);
      case GTP_IE_CHANGE_REPORTING_ACTION:
         return new GtpChangeReportingAction(instance);
      case GTP_IE_FQ_CSID:
         return new GtpFqCsid(instance);
      case GTP_IE_CHANNEL_NEEDED:
         return new GtpChannelNeeded(instance);
      case GTP_IE_EMLPP_PRIORITY:
         return new GtpEmlppPriority(instance);
      case GTP_IE_NODE_TYPE:
         return new GtpNodeType(instance);
      case GTP_IE_FQDN:
         return new GtpFqdn(instance);
      case GTP_IE_TI:
         return new GtpTi(instance);
      case GTP_IE_MBMS_SESSION_DURATION:
         return new GtpMbmsSessionDuration(instance);
      case GTP_IE_MBMS_SERVICE_AREA:
         return new GtpMbmsServiceArea(instance);
      case GTP_IE_MBMS_SESSION_ID:
         return new GtpMbmsSessionId(instance);
      case GTP_IE_MBMS_FLOW_ID:
         return new GtpMbmsFlowId(instance);
      case GTP_IE_MBMS_IP_MULTICAST_DISTRIBUTION:
         return new GtpMbmsIpMulticastDistribution(instance);
      case GTP_IE_MBMS_DISTRIBUTION_ACK:
         return new GtpMbmsDistributionAck(instance);
      case GTP_IE_RFSP_INDEX:
         return new GtpRfspIndex(instance);
      case GTP_IE_UCI:
         return new GtpUci(instance);
      case GTP_IE_CSG_INFO_REPORTING_ACTION:
         return new GtpCsgInfoReportingAction(instance);
      case GTP_IE_CSG_ID:
         return new GtpCsgId(instance);
      case GTP_IE_CMI:
         return new GtpCmi(instance);
      case GTP_IE_SERVICE_INDICATOR:
         return new GtpServiceIndicator(instance);
      case GTP_IE_DETACH_TYPE:
         return new GtpDetachType(instance);
      case GTP_IE_LDN:
         return new GtpLdn(instance);
      case GTP_IE_MBMS_TIME_TO_DATA_TRANSFER:
         return new GtpMbmsTimeToDataTransfer(instance);
      case GTP_IE_TMGI:
         return new GtpTmgi(instance);
      case GTP_IE_ADDITIONAL_MM_CNTXT_FOR_SRVCC:
         return new GtpAdditionalMmCntxtForSrvcc(instance);
      case GTP_IE_ADDITIONAL_FLAGS_FOR_SRVCC:
         return new GtpAdditionalFlagsForSrvcc(instance);
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
      IeParam *param = *b;

     if (STRCASECMP(param->paramName, "ul") == 0)
      {
         GtpApnAmbr_t ambrul = (GtpApnAmbr_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_APN_AMBR(m_val, ambrul);
         this->hdr.len += 4;
      }
      else if (STRCASECMP(param->paramName, "dl") == 0)
      {
         GtpApnAmbr_t ambrdl = (GtpApnAmbr_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_APN_AMBR((m_val + 4), ambrdl);
         this->hdr.len += 4;
      }
      else
      {
         LOG_ERROR("Invalid Ambr Parameter type [%s]", param->paramName);
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

/**
 * @brief Builds Bearer QOS IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpBearerQos::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_BEARER_QOS_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Bearer QoS Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds Bearer QoS ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpBearerQos::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      if (STRCASECMP(param->paramName, "arp") == 0)
      {
         GtpArp_t arp = (GtpArp_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_ARP(m_val, arp);
         this->hdr.len += 1;
      }
      else if (STRCASECMP(param->paramName, "qci") == 0)
      {
         GtpQci_t qci = (GtpQci_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_QCI((m_val + 1), qci);
         this->hdr.len += 1;
      }
      else if (STRCASECMP(param->paramName, "mbrul") == 0)
      {
         GtpBitRate_t br = (GtpBitRate_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_BIT_RATE((m_val + 2), br);
         this->hdr.len += 4;
      }
      else if (STRCASECMP(param->paramName, "mbrdl") == 0)
      {
         GtpBitRate_t br = (GtpBitRate_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_BIT_RATE((m_val + 6), br);
         this->hdr.len += 4;
      }
      else if (STRCASECMP(param->paramName, "gbrul") == 0)
      {
         GtpBitRate_t br = (GtpBitRate_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_BIT_RATE((m_val + 10), br);
         this->hdr.len += 4;
      }
      else if (STRCASECMP(param->paramName, "gbrdl") == 0)
      {
         GtpBitRate_t br = (GtpBitRate_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_BIT_RATE((m_val + 14), br);
         this->hdr.len += 4;
      }
      else
      {
         LOG_ERROR("Ignoring uknown BearerQos Parameter type [%s]",\
               param->paramName);
      }

      delete *b;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes Bearer QOS IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpBearerQos::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the Bearer Qos IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpBearerQos::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}


/**
 * @brief Builds Flow QOS IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpFlowQos::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_FLOW_QOS_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Flow QoS Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds Flow QoS ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpFlowQos::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      if (STRCASECMP(param->paramName, "qci") == 0)
      {
         GtpQci_t qci = (GtpQci_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_QCI(m_val, qci);
         this->hdr.len += 1;
      }
      else if (STRCASECMP(param->paramName, "mbrul") == 0)
      {
         GtpBitRate_t br = (GtpBitRate_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_BIT_RATE((m_val + 1), br);
         this->hdr.len += 4;
      }
      else if (STRCASECMP(param->paramName, "mbrdl") == 0)
      {
         GtpBitRate_t br = (GtpBitRate_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_BIT_RATE((m_val + 5), br);
         this->hdr.len += 4;
      }
      else if (STRCASECMP(param->paramName, "gbrul") == 0)
      {
         GtpBitRate_t br = (GtpBitRate_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_BIT_RATE((m_val + 9), br);
         this->hdr.len += 4;
      }
      else if (STRCASECMP(param->paramName, "gbrdl") == 0)
      {
         GtpBitRate_t br = (GtpBitRate_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_BIT_RATE((m_val + 13), br);
         this->hdr.len += 4;
      }
      else
      {
         LOG_ERROR("Ignoring uknown FlowQos Parameter type [%s]",\
               param->paramName);
      }

      delete *b;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes Flow QOS IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpFlowQos::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the Flow Qos IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpFlowQos::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}


/**
 * @brief Builds PCO IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpPco::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_PCO_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid PCO Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds PCO ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpPco::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes PCO IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpPco::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the PCO IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpPco::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}


/**
 * @brief Builds Cause IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpCause::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_CAUSE_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Cause Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds Cause ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpCause::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   U8       errPres = 0;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      if (STRCASECMP(param->paramName, "value") == 0)
      {
         GtpCause_t cause = (GtpCause_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_CAUSE_VALUE(m_val, cause);
         this->hdr.len += 1;
      }
      else if (STRCASECMP(param->paramName, "bce") == 0)
      {
         if (STRNCMP((const S8*)param->buf.pVal, "1", 1) == 0)
         {
            GSIM_SET_MASK(errPres, GTP_CAUSE_BCE_PRES);
         }
      }
      else if (STRCASECMP(param->paramName, "pce") == 0)
      {
         if (STRNCMP((const S8*)param->buf.pVal, "1", 1) == 0)
         {
            GSIM_SET_MASK(errPres, GTP_CAUSE_PCE_PRES);
         }
      }
      else if (STRCASECMP(param->paramName, "cs") == 0)
      {
         if (STRNCMP((const S8*)param->buf.pVal, "1", 1) == 0)
         {
            GSIM_SET_MASK(errPres, GTP_CAUSE_CS_PRES);
         }
      }
      else if (STRCASECMP(param->paramName, "offending_ie_type") == 0)
      {
         GtpIeType_E ieType = (GtpIeType_E)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_IE_TYPE((m_val + 2), ieType);
         this->hdr.len += 1;
      }
      else if (STRCASECMP(param->paramName, "offending_ie_length") == 0)
      {
         GtpLength_t ieLen = (GtpLength_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_IE_LENGTH((m_val + 3), ieLen);
         this->hdr.len += 2;
      }
      else if (STRCASECMP(param->paramName, "offending_ie_instance") == 0)
      {
         GtpInstance_t ieInst = (GtpInstance_t)gtpConvStrToU32(\
               (const S8*)param->buf.pVal, param->buf.len);
         GTP_ENC_IE_INSTANCE((m_val + 5), ieInst);
         this->hdr.len += 1;
      }
      else
      {
         LOG_ERROR("Ignoring uknown FlowQos Parameter type [%s]",\
               param->paramName);
      }

      delete *b;
   }

   if (!errPres)
   {
      GSIM_ENC_U8((m_val + 1), errPres);
      this->hdr.len += 1;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes Cause IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpCause::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the Cause IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpCause::decode(const U8 *pBuf)
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
 *    Encodes Recovery IE
 *
 * @param pVal
 *    string containing Recovery number
 *
 * @return
 *    ROK if Recovery is encoded successfully
 *    RFAILED otherwise
 */
RETVAL GtpRecovery::buildIe(const S8 *value)
{
   LOG_ENTERFN();

   m_val = (GtpRecovery_t)gtpConvStrToU32((const S8*)value, STRLEN(value));
   this->hdr.len = STRLEN(value);;
   LOG_EXITFN(ROK);
}

/**
 * @brief Encodes Recovery IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpRecovery::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   GTP_ENC_RECOVERY(pTmpBuf, m_val);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the Recovery IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpRecovery::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   m_val = pBuf[GTP_IE_HDR_LEN];
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds STN-SR IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpStnSr::buildIe(const S8 *value)
{
   LOG_ENTERFN();

   (VOID)value;

   LOG_EXITFN(ROK);
}

/**
 * @brief Builds STN-SR IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpStnSr::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_STN_SR_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid STN-SR Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes STN-SR IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpStnSr::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the STN-SR IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpStnSr::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds EPS Bearer TFT IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpEpsBearerTft::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_EPS_BEARER_TFT_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid EPS Bearer TFT Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds EPS Bearer TFT ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpEpsBearerTft::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   U8       errPres = 0;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      /* TODO */

      delete param;
   }

   if (!errPres)
   {
      GSIM_ENC_U8((m_val + 1), errPres);
      this->hdr.len += 1;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes EPS Bearer TFT IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpEpsBearerTft::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the EPS Bearer TFT IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpEpsBearerTft::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds TAD IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpTad::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_TAD_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid TAD Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds TAD ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpTad::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   U8       errPres = 0;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      /* TODO */

      delete param;
   }

   if (!errPres)
   {
      GSIM_ENC_U8((m_val + 1), errPres);
      this->hdr.len += 1;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes TAD IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpTad::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the TAD IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpTad::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds TMSI IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpTmsi::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_TMSI_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid TMSI Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes TMSI IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpTmsi::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the TMSI IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpTmsi::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds P-TMSI IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpPtmsi::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_PTMSI_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid TMSI Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes P-TMSI IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpPtmsi::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the P-TMSI IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpPtmsi::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds PTMSI Signature IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpPtmsiSignature::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_PTMSI_SIGNAURE_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid PTMSI Signature Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes PTMSI Signature IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpPtmsiSignature::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the PTMSI Signature IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpPtmsiSignature::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds Hop Counter IE from string
 *
 * @param pVal
 *
 * @return 
 */
RETVAL GtpHopCounter::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();
   
   m_val = (U8)gtpConvStrToU32((const S8*)pVal, STRLEN(pVal));
   this->hdr.len = STRLEN(pVal);

   LOG_EXITFN(ROK);
}

/**
 *
 * @brief Encodes Hop Counter IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpHopCounter::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   GTP_ENC_HOP_COUNTER(pTmpBuf, m_val);

   LOG_EXITFN(this->hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the Hop Counter IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpHopCounter::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   m_val = pBuf[GTP_IE_HDR_LEN];
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds Global Connection ID IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpGlobalCnId::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_GLOBAL_CN_ID_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Global Connection ID Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds Global Connection ID ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpGlobalCnId::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   U8       errPres = 0;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      /* TODO */

      delete param;
   }

   if (!errPres)
   {
      GSIM_ENC_U8((m_val + 1), errPres);
      this->hdr.len += 1;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes Global Connection ID IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpGlobalCnId::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the Global Connection ID IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpGlobalCnId::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds S103 PDF IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpS103Pdf::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_S103_PDN_DATA_FWD_INFO_MAX_BUF_LEN >= \
         GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid S103 PDF Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds S103 PDF ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpS103Pdf::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   U8       errPres = 0;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      /* TODO */

      delete param;
   }

   if (!errPres)
   {
      GSIM_ENC_U8((m_val + 1), errPres);
      this->hdr.len += 1;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes S103 PDF IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpS103Pdf::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the S103 PDF IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpS103Pdf::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds S1U DF IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpS1uDf::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_S103_PDN_DATA_FWD_INFO_MAX_BUF_LEN >= \
         GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid S1U DF Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds S1U DF ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpS1uDf::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   U8       errPres = 0;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      /* TODO */

      delete param;
   }

   if (!errPres)
   {
      GSIM_ENC_U8((m_val + 1), errPres);
      this->hdr.len += 1;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes S1U DF IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpS1uDf::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the S1U DF IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpS1uDf::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds Delay Value IE from string
 *
 * @param pVal
 *
 * @return 
 */
RETVAL GtpDelayValue::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();
   
   m_val = (U8)gtpConvStrToU32((const S8*)pVal, STRLEN(pVal));
   this->hdr.len = STRLEN(pVal);

   LOG_EXITFN(ROK);
}

/**
 *
 * @brief Encodes Delay Value IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpDelayValue::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   GTP_ENC_DELAY_VALUE(pTmpBuf, m_val);

   LOG_EXITFN(this->hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the Delay Value IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpDelayValue::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   m_val = pBuf[GTP_IE_HDR_LEN];
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds Charging ID IE from string
 *
 * @param pVal
 *
 * @return 
 */
RETVAL GtpChargingId::buildIe(const S8 *pVal)
{
   LOG_ENTERFN();
   
   U32 chargingId = gtpConvStrToU32((const S8*)pVal, STRLEN(pVal));
   GTP_ENC_CHARGING_ID(m_val, chargingId);
   this->hdr.len = STRLEN(pVal);

   LOG_EXITFN(ROK);
}

/**
 * @brief Builds S1U DF IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpChargingId::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_CHARGING_ID_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid S1U DF Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 *
 * @brief Encodes Delay Value IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpChargingId::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &this->hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, GTP_CHARGING_ID_MAX_BUF_LEN);

   LOG_EXITFN(this->hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the Delay Value IE from the buffer pointed by pBuf
 *    into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpChargingId::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds Charging Charecteristics IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpChargingCharcs::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_CHARGING_CHARCS_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Charging Charecteristics Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Builds Charging Charecteristics ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpChargingCharcs::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   U8       errPres = 0;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      /* TODO */

      delete param;
   }

   if (!errPres)
   {
      GSIM_ENC_U8((m_val + 1), errPres);
      this->hdr.len += 1;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes Charging Charecteristics IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpChargingCharcs::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the Charging Charecteristics IE from the buffer 
 *    pointed by pBuf into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpChargingCharcs::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds Trace Info IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpTraceInfo::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_TRACE_INFO_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Trace Info Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes Trace Info IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpTraceInfo::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the Trace Info IE from the buffer 
 *    pointed by pBuf into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpTraceInfo::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds Bearer Flags IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpBearerFlags::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_BEARER_FLAGS_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, &m_val);
   }
   else
   {
      LOG_ERROR("Invalid Bearer Flags Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Builds Bearer Flags ie from list of IE params
 *
 * @param paramLst
 *
 * @return 
 */
RETVAL GtpBearerFlags::buildIe(IeParamLst *paramLst)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;

   if (paramLst->empty())
   {
      LOG_ERROR("No parameters to encode");
      LOG_EXITFN(RFAILED);
   }

   for (IeParamLstItr b = paramLst->begin(); b != paramLst->end(); b++)
   {
      IeParam *param = *b;

      /* TODO */

      delete param;
   }

   delete paramLst;
   LOG_EXITFN(ret);
}

/**
 * @brief Encodes Bearer Flags IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpBearerFlags::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   GTP_ENC_IE_HDR(pBuf, &hdr);
   pBuf[GTP_IE_HDR_LEN] = m_val;

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the Bearer Flags IE from the buffer 
 *    pointed by pBuf into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpBearerFlags::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   m_val = pBuf[GTP_IE_HDR_LEN];
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds PTI IE from string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpPti::buildIe(const S8 *value)
{
   LOG_ENTERFN();
   
   m_val = (U8)gtpConvStrToU32((const S8*)value, STRLEN(value));
   this->hdr.len = STRLEN(value);

   LOG_EXITFN(ROK);
}

/**
 * @brief Builds PTI IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpPti::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_PTI_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, &m_val);
   }
   else
   {
      LOG_ERROR("Invalid PTI Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}


/**
 * @brief Encodes PTI IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpPti::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   GTP_ENC_IE_HDR(pBuf, &hdr);
   pBuf[GTP_IE_HDR_LEN] = m_val;

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}


/**
 * @brief Decodes the PTI IE from the buffer 
 *    pointed by pBuf into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpPti::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   m_val = pBuf[GTP_IE_HDR_LEN];
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds Drx Parameter IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpDrxParam::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_DRX_PARAM_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid Drx Parameter Buffer Length [%d]", value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes Drx Parameter IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpDrxParam::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the Drx Parameter IE from the buffer 
 *    pointed by pBuf into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpDrxParam::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds UE Network Capability IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpUeNetworkCap::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_UE_NETWORK_CAP_MAX_BUF_LEN >= GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid UE Network Capability Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes UE Network Capability IE into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpUeNetworkCap::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the UE Network Capability IE from the buffer 
 *    pointed by pBuf into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpUeNetworkCap::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds MM Context GSM Key and Triplets IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpMmCntxtGsmKeyAndTriplets::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_MM_CNTXT_GSM_KEY_AND_TRIPLETS_MAX_BUF_LEN >= \
         GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid UE Network Capability Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes MM Context GSM Key and Triplets IE into byte array 
 * pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpMmCntxtGsmKeyAndTriplets::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the MM Context GSM Key and Triplets IE from the buffer 
 *    pointed by pBuf into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpMmCntxtGsmKeyAndTriplets::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds MM Context GSM Key Used cipher and quintiuples IE from 
 *    hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpMmCntxtGsmKeyUsedCipherAndQuint::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_MM_CNTXT_GSM_KEY_USED_CIPHER_N_QUINT_MAX_BUF_LEN >= \
         GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid UE Network Capability Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes MM Context GSM Key Used Cipher and Quintiuples IE 
 *    into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpMmCntxtGsmKeyUsedCipherAndQuint::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the MM Context GSM Key Used Cipher and Quintuples IE
 *    from the buffer pointed by pBuf into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpMmCntxtGsmKeyUsedCipherAndQuint::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}

/**
 * @brief Builds MM Context UMTS Used Cipher and Quintuples IE from hex string
 *
 * @param value
 *
 * @return 
 */
RETVAL GtpMmCntxtUmtsKeyUsedCipherAndQuint::buildIe(const HexString *value)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   if (GTP_MM_CNTXT_UMTS_KEY_USED_CIPHER_AND_QUINTS_MAX_BUF_LEN >= \
         GSIM_CEIL_DIVISION(value->size(), 2))
   {
      this->hdr.len = gtpConvStrToHex(value, m_val);
   }
   else
   {
      LOG_ERROR("Invalid UE Network Capability Buffer Length [%d]",\
            value->size());
      ret = RFAILED;
   }

   LOG_EXITFN(ret);
}

/**
 * @brief Encodes MM Context UMTS Key Used Cipher and Quintuples IE 
 * into byte array pointed by pBuf
 *
 * @param pBuf
 *
 * @return 
 *    returns the total encoded length (header length inclusive)
 */
GtpLength_t GtpMmCntxtUmtsKeyUsedCipherAndQuint::encode(U8 *pBuf)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pBuf;

   GTP_ENC_IE_HDR(pTmpBuf, &hdr);
   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, m_val, hdr.len);

   LOG_EXITFN(hdr.len + GTP_IE_HDR_LEN);
}

/**
 * @brief Decodes the MM Context UMTS Key Used Cipher and Quintuples IE
 *    from the buffer pointed by pBuf into local data structure
 *
 * @param pBuf
 *
 * @return 
 */
GtpLength_t GtpMmCntxtUmtsKeyUsedCipherAndQuint::decode(const U8 *pBuf)
{
   LOG_ENTERFN();

   U32 ieLen = 0;

   GTP_GET_IE_LEN(pBuf, ieLen);
   MEMCPY(m_val, pBuf + GTP_IE_HDR_LEN, ieLen);
   this->hdr.len = ieLen;

   LOG_EXITFN(GTP_IE_HDR_LEN + ieLen);
}
