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

// Utility functions for gtp

#include <stdlib.h>
#include <list>
#include <arpa/inet.h>

#include "types.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "gtp_macro.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"

S8 g_gtpMsgName[GTPC_MSG_TYPE_MAX][GTP_MSG_NAME_LEN] = 
{
   "",
   "Echo Req",
   "Echo Rsp",
   "Version Not Supported Ind",
   "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "",
   "Create Sessn Req",
   "Create Sessn Rsp",
   "Modify Bearer Req",
   "Modify Bearer Rsp",
   "Delete Sessn Req",
   "Delete Sessn Rsp",
   "Change Notif Req",
   "Change Notif Rsp",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "",
   "Modify Bearer Cmd",
   "Moidfy Bearer Fail Ind",
   "Delete Bearer Cmd",
   "Delete Bearer Fail Ind",
   "Bearer Res Cmd",
   "Bearer Res Fail Ind",
   "DL Data Notif Fail Ind"
   "Trace Sessn Activation",
   "Trace Sessn Deactivation",
   "Stop Paging Ind",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "",
   "Create Bearer Req",
   "Create Bearer Rsp",
   "Update Bearer Req",
   "Update Bearer Rsp",
   "Delete Bearer Req",
   "Delete Bearer Rsp",
   "Delete PDN Conn Set Req",
   "Delete PDN Conn Set Rsp",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "",
   "Identification Req",
   "Identification Rsp",
   "Cntxt Req",
   "Cntxt Rsp",
   "Cntxt Ack",
   "Fwd Reloc Req",
   "Fwd Reloc Rsp",
   "Fwd Reloc Compl Notif",
   "Fwd Reloc Compl Ack",
   "Fwd Access Cntxt Notif",
   "Fwd Access Cntxt Ack",
   "Reloc Cancel Req",
   "Reloc Cancel Rsp",
   "Cfg Transfer Tun",
   "", "", "", "", "", "", "",
   "Detach Notif",
   "Detach Ack",
   "CS Paging Ind",
   "RAN Info Relay",
   "Alert MME Notif",
   "Alert MME Ack",
   "UE Activity Notif",
   "UE Activity Ack",
   "", "", "",
   "Create Fwding Tun Req",
   "Create Fwding Tun Rsp",
   "Suspend Notif",
   "Suspend Ack",
   "Resume Notif",
   "Resume Ack",
   "Create IDF Tun Req",
   "Create IDF Tun Rsp",
   "Delete IDF Tun Req",
   "Delete IDF Tun Rsp",
   "Rel Access Bearers Req",
   "Rel Access Bearers Rsp",
   "", "", "", "",
   "DL Data Notif",
   "DL Data Notif Ack",
   "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "",
   "", "",
   "Update PDN Conn Set Req",
   "Update PDN Conn Set Rsp",
   "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "",
   "MBMS Sessn Start Req",
   "MBMS Sessn Start Rsp",
   "MBMS Sessn Update Req",
   "MBMS Sessn Update Rsp",
   "MBMS Sessn Stop Req",
   "MBMS Sessn Stop Rsp"
};

GtpMsgCategory_E g_gtpMsgCat[GTPC_MSG_TYPE_MAX] = \
{
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG_RSP
};

S8 g_gtpIeName[GTP_IE_MAX][GTP_IE_NAME_LEN] = \
{
   "Reserved",
   "Imsi",
   "Cause",
   "Recovery"
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "stn_sr",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "apn",
   "ambr",
   "ebi",
   "ip_addr",
   "mei",
   "msisdn",
   "indication",
   "pco",
   "paa",
   "bearer_qos",
   "flow_qos",
   "RAT_Type",
   "Serving_Network",
   "EPS_Bearer_TFT",
   "tad",
   "uli",
   "fteid",
   "TMSI",
   "Global_CN_Id",
   "S103PDF",
   "S1UDF",
   "delay",
   "Bcontext",
   "Charging_ID",
   "Charging_Char",
   "Trace_Info",
   "Bearer_Flags",
   "Reserved",
   "PDN_Type",
   "Pti",
   "DRX_param",
   "UE_Network_Cap",
   "MM_Context_gsm_key_trip",
   "MM_Context_umts_key_used_cipher_quint",
   "MM_Context_gsm_key_used_cipher_quint",
   "MM_Context_umts_key_quint",
   "MM_Context_EPS_Sec_Context_quadr_quint",
   "MM_Context_umts_Key_Quadr_Quint",
   "PDN_Connection",
   "PDU_Numbers",
   "PTMSI",
   "PTMSI_Signature",
   "Hop_Counter",
   "UE_Time_Zone",
   "Trace_Reference",
   "Complete_Request_Message",
   "GUTI",
   "FContainer",
   "FCause",
   "Selected_PLMN_ID",
   "Target_Id",
   "Reserved",
   "Packet_Flow_ID",
   "RAB_Context",
   "Source_RNC_PDCP_Context_Info",
   "UDP_Source_Port_Number",
   "APN_Restriction",
   "Selection_Mode",
   "Source_Id",
   "Reserved",
   "Change_Reporting_Action",
   "fqcsid",
   "Channel_needed",
   "eMLPP_Priority",
   "Node_Type",
   "Fqdn",
   "ti",
   "MBMS_Session_Duration",
   "MBMS_Service_Area",
   "MBMS_Session_Id",
   "MBMS_Flow_Id",
   "MBMS_IP_Multicast_Distribution",
   "MBMS_Distribution_Acknowledge",
   "RFSP_Index",
   "UCI",
   "CSG_Info_Reporting_Action",
   "CSG_ID",
   "cmi",
   "Service_indicator",
   "Detach_Type",
   "Ldn",
   "Reserved",
   "MBMS_Time_to_Data_Transfer",
   "Reserved",
   "Reserved",
   "Reserved",
   "Reserved",
   "Tmgi",
   "Additional_MM_context_for_SRVCC",
   "Additional_flags_for_SRVCC",
   "Reserved",
   "Reserved",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
   "Spare",
};

S8 g_gtpMsgXmlTag[GTPC_MSG_TYPE_MAX][GTP_MSG_XML_TAG_MAX_LEN] = \
{
   "",
   "ereq",
   "ersp",
   "vnsind",
   "",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "", "", "", "",
   "csreq",
   "csrsp",
   "mbreq",
   "mbrsp",
   "dsreq",
   "dsrsp",
   "cnreq",
   "cnrsp"
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "",
   "mbcmd",
   "mbcmdfind",
   "dbcmd",
   "dbcmdfind",
   "brcmd",
   "brcmdfind",
   "tsact",
   "tsdact",
   "spind",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "",
   "cbreq",
   "cbrsp",
   "ubreq",
   "ubrsp",
   "dbreq",
   "dbrsp",
   "dpcsreq",
   "dpcsrsp",
   "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "",
   "idreq",
   "idrsp",
   "creq",
   "crsp",
   "cack",
   "frreq",
   "frrsp",
   "frcnotif",
   "frcack",
   "facnotif",
   "facack",
   "rcreq",
   "rcrsp",
   "cttun",
   "", "", "", "", "", "", "",
   "dnotif",
   "dack",
   "cspind",
   "rirelay",
   "amnotif",
   "amack",
   "uanotif",
   "uaack",
   "", "", "",
   "cftreq",
   "cftrsp",
   "snotif",
   "sack",
   "rnotif",
   "rack",
   "citreq",
   "citrsp",
   "ditreq",
   "ditrsp",
   "rabreq",
   "rabrsp",
   "", "", "", "",
   "dldnotif",
   "dldnotifack",
   "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "",
   "", "",
   "upcsreq",
   "upcsrsp",
   "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "", "",
   "", "", "", "", "", "", "", "", "",
   "mssreq",
   "mssrsp",
   "msureq",
   "msursp",
   "msstreq",
   "msstrsp"
};

S8* gtpGetMsgName(GtpMsgType_E msgType)
{
   S8 *pMsgName = NULL;

   if (msgType <= GTPC_MSG_TYPE_MAX)
   {
      if (STRCMP(g_gtpMsgName[msgType], "") != 0)
      {
         pMsgName = g_gtpMsgName[msgType];
      }
   }

   return pMsgName;
}

GtpIeType_E gtpGetIeType(const S8   *pIeName)
{
   for (U32 ieType = GTP_IE_RESERVED; ieType < GTP_IE_MAX; ieType++)
   {
      if (strcasecmp(g_gtpIeName[ieType], pIeName) == 0)
      {
         return (GtpIeType_E)ieType;
      }
   }

   return GTP_IE_MAX;
}

GtpMsgType_E gtpGetMsgType(const S8 *pXmlMsgTag)
{
   for (U32 msgType = GTPC_MSG_TYPE_INVALID; msgType < GTPC_MSG_TYPE_MAX;
         msgType++)
   {
      if (strcasecmp(g_gtpMsgXmlTag[msgType], pXmlMsgTag) == 0)
      {
         return (GtpMsgType_E)msgType;
      }
   }

   return GTPC_MSG_TYPE_INVALID;
}

GtpMsgCategory_E gtpGetMsgCategory(GtpMsgType_E msgType)
{
   LOG_ENTERFN();

   GtpMsgCategory_E msgCat = GTP_MSG_CAT_INV;
   if (msgType <= GTPC_MSG_TYPE_MAX)
   {
      msgCat = g_gtpMsgCat[msgType]; 
   }

   LOG_EXITFN(msgCat);
}

PRIVATE U8 gtpCharToHex(U8 c)
{
   LOG_ENTERFN();

   U8 h = 0;
   switch (c)
   {
      case 'a':
         h = 0xa;
         break;
      case 'b':
         h = 0xb;
         break;
      case 'c':
         h = 0xc;
         break;
      case 'd':
         h = 0xd;
         break;
      case 'e':
         h = 0xe;
         break;
      case 'f':
         h = 0xf;
         break;
      default:
         h = GSIM_CHAR_TO_DIGIT(c);
         break;
   }
   
   LOG_EXITFN(h);
}

GtpLength_t gtpConvStrToHex(const Buffer *pBuf, U8 *pDst)
{
   LOG_ENTERFN();

   U32 begin = 0;
   U32 indx = 0;

   /* hex buffer string has odd len, so the first byte of hex buffer
    * will have 0 as High nibble
    */
   if (GSIM_IS_ODD(pBuf->len))
   {
      pDst[indx++] = 0x0f & gtpCharToHex(pBuf->pVal[0]);
      begin++;
   }

   for (U32 i = begin; i < pBuf->len; i += 2)               
   {                                                        
      U8 HNibble = gtpCharToHex(pBuf->pVal[i]); 
      U8 LNibble = gtpCharToHex(pBuf->pVal[i + 1]);     
      pDst[indx++] = (HNibble << 4) | (LNibble);
   }                                                        
                                                            
   LOG_EXITFN(indx);
}

VOID gtpEncIeUsingHexBuf(U8 *pSrc, GtpIeHdr *pHdr, U8 *pDst, U32 *pDstLen)
{
   LOG_ENTERFN();

   U8 *pTmpBuf = pDst;

   GTP_ENC_IE_HDR(pTmpBuf, pHdr);

   pTmpBuf += GTP_IE_HDR_LEN;
   MEMCPY(pTmpBuf, pSrc, pHdr->len);
   *pDstLen = pHdr->len + GTP_IE_HDR_LEN;

   LOG_EXITVOID();
}

PUBLIC U32 gtpConvStrToU32(const S8 *pVal, U32 len)
{
   LOG_ENTERFN();

   S8 *tmp = NULL;
   S8 buf[GSIM_SIZE_32] = {'\0'};

   MEMCPY(buf, pVal, len);
   U32 v = strtoul(buf, &tmp, 10);

   LOG_EXITFN(v);
}

GtpIfType_E gtpConvStrToIfType(const S8 *pVal, U32 len)
{
   LOG_ENTERFN();
   
   GtpIfType_E ifType = (GtpIfType_E)gtpConvStrToU32(pVal, len);
   if (ifType >= GTP_IF_INF_INV)
   {
      ifType = GTP_IF_INF_INV;
      LOG_ERROR("Unrecognized Interface type string [%s]", pVal);
   }

   LOG_EXITFN(ifType);
}

IpAddr convIpStrToIpAddr(const S8* pIp, U32 len)
{
   LOG_ENTERFN();

   IpAddr ipAddr;
   S8     ip[GSIM_SIZE_32] = {'\0'};
   MEMCPY(ip, pIp, len);

   ipAddr.ipAddrType = IP_ADDR_TYPE_INV;
   if (STRFIND(ip, ":") != NULL)
   {
      RETVAL ret = inet_pton(AF_INET6, ip, (VOID *)(ipAddr.u.ipv6Addr.addr));
      if (0 == ret)
      {
         LOG_FATAL("Invalid IPv6 Address");
      }
      else
      {
         ipAddr.u.ipv6Addr.len = STRLEN(ip);
         ipAddr.ipAddrType = IP_ADDR_TYPE_V6;
      }
   }
   else
   {
      RETVAL ret = inet_pton(AF_INET, ip, (VOID *)&(ipAddr.u.ipv4Addr.addr));
      if (0 == ret)
      {
         LOG_FATAL("Invalid IPv4 Address");
      }
      else
      {
         ipAddr.u.ipv4Addr.addr = ntohl(ipAddr.u.ipv4Addr.addr);
         ipAddr.ipAddrType = IP_ADDR_TYPE_V4;
      }
   }

   LOG_EXITFN(ipAddr);
}

VOID decIeHdr(U8 *pBuf, GtpIeHdr *pHdr)
{
   LOG_ENTERFN();

   MEMSET((VOID *)pHdr, 0, sizeof(GtpIeHdr));
   GTP_DEC_IE_TYPE(pBuf, pHdr->ieType);
   GTP_DEC_IE_LEN(pBuf, pHdr->len);
   GTP_DEC_IE_INSTANCE(pBuf, pHdr->instance);

   LOG_EXITVOID();
}

PUBLIC VOID numericStrIncriment(S8 *pStr, U32 len)
{
   LOG_ENTERFN();

   U32 d = 0;
   U32 carry = 1;

   for (S32 i = len - 1; i >= 0; i--)
   {
      d = GSIM_CHAR_TO_DIGIT(pStr[i]) + carry;
      if (d > 9)
      {
         pStr[i] = GSIM_DIGIT_TO_CHAR(d % 10);
         carry = 1;
      }
      else
      {
         pStr[i] = GSIM_DIGIT_TO_CHAR(d);
         carry = 0;
      }
   }

   LOG_EXITVOID();
}

U32 encodeImsi(S8 *pImsiStr, U32 imsiStrLen, U8 *pBuf)
{
   LOG_ENTERFN();

   U32      indx = 0;
   U32      len = 0;

   /* Enocde even number of digits into hex buffer */
   for (indx = 0; indx < (imsiStrLen - 1); indx += 2)
   {
      pBuf[len++] = GSIM_CHAR_TO_DIGIT(pImsiStr[indx]) | \
            GSIM_CHAR_TO_DIGIT(pImsiStr[indx + 1]) << 4;
   }

   /* Encode the last digit into hex buffer */
   if (GSIM_IS_ODD(imsiStrLen))
   {
      pBuf[len] = GSIM_CHAR_TO_DIGIT(pImsiStr[imsiStrLen - 1]) | 0xf0;
      len++;
   }

   LOG_EXITFN(len);
}


/**
 * @brief 
 *    returns the pointer to imsi ie in GTP message buffer
 *
 * @return 
 *    returns the pointer to imsi ie in GTP message buffer
 */
PUBLIC U8* getImsiBufPtr(Buffer *pGtpcBuf)
{
   LOG_ENTERFN();

   U8          *pImsi = NULL;
   GtpIeHdr    ieHdr;
   GtpLength_t len = pGtpcBuf->len;
   U8          *pBuf = pGtpcBuf->pVal;

   if (GTP_CHK_T_BIT_PRESENT(pBuf))
   {
      len -= GTP_MSG_HDR_LEN;
      pBuf += GTP_MSG_HDR_LEN;
   }
   else
   {
      len -= GTP_MSG_HDR_LEN_WITHOUT_TEID;
      pBuf += GTP_MSG_HDR_LEN_WITHOUT_TEID;
   }

   while (len)
   {
      decIeHdr(pBuf, &ieHdr);
      if (ieHdr.ieType == GTP_IE_IMSI)
      {
         pImsi = pBuf;
         break;
      }

      len -= (ieHdr.len + GTP_IE_HDR_LEN);
      pBuf += (ieHdr.len + GTP_IE_HDR_LEN);
   }

   LOG_EXITFN(pImsi);
}
