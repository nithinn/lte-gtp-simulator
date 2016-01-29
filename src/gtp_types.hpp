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

#ifndef _GTP_TYPES_HPP
#define _GTP_TYPES_HPP

#define GTP_MSG_HDR_LEN                   12  
#define GTP_MSG_HDR_LEN_WITHOUT_TEID      8
#define GTP_TEID_LEN                      4  
#define GTP_IE_HDR_LEN                    4  
#define GTP_T3_TIMER                      5     /* in seconds  */
#define GTP_N3_REQ                        3     /* max no.of retransmissions */
#define SND_MSG_WAIT_TIMER                0     /* time to wait before 
                                                 * sending a msg */
#define GTP_MSG_BUF_LEN                   1024

typedef U8           GtpVersion_t;
typedef U32          GtpTeid_t;
typedef U8           GtpInstance_t;
typedef U32          GtpSeqNumber_t;    
typedef U16          GtpLength_t;
typedef U16          GtpLac_t;
typedef U16          GtpRac_t;
typedef U16          GtpTac_t;
typedef U16          GtpCellId_t;
typedef U16          GtpSac_t;
typedef U32          GtpECellId_t;
typedef U8           GtpEbi_t;
typedef U8           GtpArp_t;
typedef U8           GtpQci_t;

typedef enum
{
   EPC_NODE_INV,
   EPC_NODE_MME,
   EPC_NODE_SGW,
   EPC_NODE_PGW,
   EPC_NODE_SGSN,
   EPC_NODE_MAX
} EpcNodeType_E;

typedef enum
{
   GTPC_MSG_TYPE_INVALID         = 0,
   GTPC_MSG_ECHO_REQ             = 1,
   GTPC_MSG_ECHO_RSP,
   GTPC_MSG_VER_N_SUPP_IND,
   GTPC_MSG_CS_REQ               = 32,
   GTPC_MSG_CS_RSP,
   GTPC_MSG_MB_REQ,
   GTPC_MSG_MB_RSP,
   GTPC_MSG_DS_REQ,
   GTPC_MSG_DS_RSP,
   GTPC_MSG_CN_REQ,
   GTPC_MSG_CN_RSP,
   GTPC_MSG_MB_CMD               = 64,
   GTPC_MSG_MB_FAIL_IND,
   GTPC_MSG_DB_CMD,
   GTPC_MSG_DB_FAIL_IN,
   GTPC_MSG_BR_CMD,
   GTPC_MSG_BR_FAIL_IND,
   GTPC_MSG_DD_NOTIF_FAIL_IND,
   GTPC_MSG_TRC_SSN_ACTVN,
   GTPC_MSG_TRC_SSN_DEACTVN,
   GTPC_MSG_STOP_PAGING_IND,
   GTPC_MSG_CB_REQ               = 95,
   GTPC_MSG_CB_RSP,
   GTPC_MSG_UB_REQ,
   GTPC_MSG_UB_RSP,
   GTPC_MSG_DB_REQ,
   GTPC_MSG_DB_RSP,
   GTPC_MSG_DEL_PDN_CON_SET_REQ,
   GTPC_MSG_DEL_PDN_CON_SET_RSP,
   GTPC_MSG_ID_REQ               = 128,
   GTPC_MSG_ID_RSP,
   GTPC_MSG_CNTXT_REQ,
   GTPC_MSG_CNTXT_RSP,
   GTPC_MSG_CNTXT_ACK,
   GTPC_MSG_FR_REQ,
   GTPC_MSG_FR_RSP,
   GTPC_MSG_FR_CMPL_NOTIF,
   GTPC_MSG_FR_CMPL_ACK,
   GTPC_MSG_FWD_ACC_CNTXT_NOTIF,
   GTPC_MSG_FWD_ACC_CNTXT_ACK,
   GTPC_MSG_RC_REQ,
   GTPC_MSG_RC_RSP,
   GTPC_MSG_CFG_TRAN_TUNN,
   GTPC_MSG_DETACH_NOTIF           = 149,
   GTPC_MSG_DETACH_ACK,
   GTPC_MSG_CS_PGNG_IND,
   GTPC_MSG_RAN_INFO_RELAY,
   GTPC_MSG_ALRT_MME_NOTIF,
   GTPC_MSG_ALRT_MME_ACK,
   GTPC_MSG_UE_ACT_NOTIF,
   GTPC_MSG_UE_ACT_ACK,
   GTPC_MSG_CF_TUNN_REQ          = 160,
   GTPC_MSG_CF_TUNN_RSP,
   GTPC_MSG_SSPND_NOTIF,
   GTPC_MSG_SSPND_ACK,
   GTPC_MSG_RSME_NOTIF,
   GTPC_MSG_RSME_ACK,
   GTPC_MSG_CIDF_TUNN_REQ,
   GTPC_MSG_CIDF_TUNN_RSP,
   GTPC_MSG_DIDF_TUNN_REQ,
   GTPC_MSG_DIDF_TUNN_RSP,
   GTPC_MSG_RAB_REQ,
   GTPC_MSG_RAB_RSP,
   GTPC_MSG_DL_DATA_NOTIF       = 176,
   GTPC_MSG_DL_DATA_NOTIF_ACK,
   GTPC_MSG_UPD_PDN_CON_SET_REQ  = 200,
   GTPC_MSG_UPD_PDN_CON_SET_RSP,
   GTPC_MSG_MBMS_SSN_START_REQ   = 231,
   GTPC_MSG_MBMS_SSN_START_RSP,
   GTPC_MSG_MBMS_SSN_UPD_REQ,
   GTPC_MSG_MBMS_SSN_UPD_RSP,
   GTPC_MSG_MBMS_SSN_STOP_REQ,
   GTPC_MSG_MBMS_SSN_STOP_RSP,
   GTPC_MSG_TYPE_MAX
} GtpMsgType_E;

typedef enum
{
   GTP_MSG_CAT_INV,
   GTP_MSG_CAT_INITIAL,
   GTP_MSG_CAT_TRIG,
   GTP_MSG_CAT_TRIG_RSP,
   GTP_MSG_CAT_MAX
} GtpMsgCategory_E;

typedef enum
{
   GTP_IE_RESERVED                              = 0,
   GTP_IE_IMSI                                  = 1,
   GTP_IE_CAUSE                                 = 2,
   GTP_IE_RECOVERY                              = 3,
   GTP_IE_RESERVED_S101                         = 4,
   GTP_IE_STN_SR                                = 51,
   GTP_IE_RESERVED_SV                           = 52,
   GTP_IE_APN                                   = 71,
   GTP_IE_AMBR,
   GTP_IE_EBI,
   GTP_IE_IP_ADDR,
   GTP_IE_MEI,
   GTP_IE_MSISDN,
   GTP_IE_INDICATION,
   GTP_IE_PCO,
   GTP_IE_PAA,
   GTP_IE_BEARER_QOS,
   GTP_IE_FLOW_QOS,
   GTP_IE_RAT_TYPE,
   GTP_IE_SERVING_NW,
   GTP_IE_EPS_BEARER_TFT,
   GTP_IE_TAD,
   GTP_IE_ULI,
   GTP_IE_FTEID,
   GTP_IE_TMSI,
   GTP_IE_GLOBAL_CN_ID,
   GTP_IE_S103_PDF,
   GTP_IE_S1UDF,
   GTP_IE_DELAY_VALUE,
   GTP_IE_BEARER_CNTXT,
   GTP_IE_CHARGING_ID,
   GTP_IE_CHARGING_CHARACTERISTICS,
   GTP_IE_TRACE_INFO,
   GTP_IE_BEARER_FLAGS,
   GTP_IE_RESERVED2,
   GTP_IE_PDN_TYPE,
   GTP_IE_PTI,
   GTP_IE_DRX_PARAM,
   GTP_IE_UE_NETWORK_CAP,
   GTP_IE_MM_CNTXT_GSM_KEY_N_TRIPLETS,
   GTP_IE_MM_CNTXT_UMTS_KEY_USED_CIPHER_N_QUINT,
   GTP_IE_MM_CNTXT_GSM_KEY_USED_CIPHER_N_QUINT,
   GTP_IE_MM_CNTXT_UMTS_KEY_N_QUINT,
   GTP_IE_MM_CNTXT_EPS_SEC_CNTXT_QUADR_N_QUINT,
   GTP_IE_MM_CNTXT_UMTS_KEY_QUADR_N_QUINT,
   GTP_IE_PDN_CONNECTION,
   GTP_IE_PDU_NUMBERS,
   GTP_IE_PTMSI,
   GTP_IE_PTMSI_SIGNATURE,
   GTP_IE_HOP_COUNTER,
   GTP_IE_UE_TIME_ZONE,
   GTP_IE_TRACE_REFERENCE,
   GTP_IE_COMPLETE_REQ_MSG,
   GTP_IE_GUTI,
   GTP_IE_FCONTAINER,
   GTP_IE_FCAUSE,
   GTP_IE_SELECTED_PLMN_ID,
   GTP_IE_TARGET_ID,
   GTP_IE_RESERVED3,
   GTP_IE_PACKET_FLOW_ID,
   GTP_IE_RAB_CNTXT,
   GTP_IE_SOURCE_RNC_PDCP_CNTXT_INFO,
   GTP_IE_UDP_SRC_PORT,
   GTP_IE_APN_RESTRICTION,
   GTP_IE_SELECTION_MODE,
   GTP_IE_SRC_ID,
   GTP_IE_RESERVED4,
   GTP_IE_CHANGE_REPORTING_ACTION,
   GTP_IE_FQ_CSID,
   GTP_IE_CHANNEL_NEEDED,
   GTP_IE_EMLPP_PRIORITY,
   GTP_IE_NODE_TYPE,
   GTP_IE_FQDN,
   GTP_IE_TI,
   GTP_IE_MBMS_SESSION_DURATION,
   GTP_IE_MBMS_SERVICE_AREA,
   GTP_IE_MBMS_SESSION_ID,
   GTP_IE_MBMS_FLOW_ID,
   GTP_IE_MBMS_IP_MULTICAST_DISTRIBUTION,
   GTP_IE_MBMS_DISTRIBUTION_ACK,
   GTP_IE_RFSP_INDEX,
   GTP_IE_UCI,
   GTP_IE_CSG_INFO_REPORTING_ACTION,
   GTP_IE_CSG_ID,
   GTP_IE_CMI,
   GTP_IE_SERVICE_INDICATOR,
   GTP_IE_DETACH_TYPE,
   GTP_IE_LDN,
   GTP_IE_RESERVED5,
   GTP_IE_MBMS_TIME_TO_DATA_TRANSFER,
   GTP_IE_RESERVED6,
   GTP_IE_RESERVED7,
   GTP_IE_RESERVED8,
   GTP_IE_RESERVED9,
   GTP_IE_TMGI,
   GTP_IE_ADDITIONAL_MM_CNTXT_FOR_SRVCC,
   GTP_IE_ADDITIONAL_FLAGS_FOR_SRVCC,
   GTP_IE_RESERVED10,
   GTP_IE_RESERVED11,
   GTP_IE_SPARE                                 = 163,
   GTP_IE_PRIVATE_IE                            = 255,
   GTP_IE_MAX
} GtpIeType_E;

typedef enum
{
    GTP_IF_S1_U_ENB      = 0,
    GTP_IF_S1_U_SGW      = 1,
    GTP_IF_S5S8_U_SGW    = 4,
    GTP_IF_S5S8_U_PGW    = 5,
    GTP_IF_S5S8_C_SGW    = 6,
    GTP_IF_S5S8_C_PGW    = 7,
    GTP_IF_S11_C_MME     = 10,
    GTP_IF_S11_C_SGW     = 11,
    GTP_IF_S4_C_SGW      = 11,
    GTP_IF_S4_C_SGSN     = 12,
    GTP_IF_INF_INV       = 32
} GtpIfType_E;

typedef enum 
{
    GTP_IP_ADDR_V4,
    GTP_IP_ADDR_V6,
    GTP_IP_ADDR_V4V6,
    GTP_IP_ADDR_INV
} GtpIpAddrType_E;

typedef struct
{
   GtpIeType_E      ieType;
   GtpInstance_t    instance;
   GtpLength_t      len;
} GtpIeHdr;

struct GtpMsgHdr 
{
   U8                pres;
#define              GTP_MSG_HDR_TEID_PRES    (1 << 0)
#define              GTP_MSG_HDR_VER_PRES     (1 << 1)
#define              GTP_MSG_HDR_SEQ_PRES     (1 << 2)
#define              GTP_MSG_T_BIT_PRES       (1 << 3)
#define              GTP_MSG_P_BIT_PRES       (1 << 4)
#define              GTP_MSG_HDR_MSGTYPE_PRES (1 << 5)

   GtpVersion_t      ver;
   GtpLength_t       len;
   GtpTeid_t         teid;
   GtpSeqNumber_t    seqN;
   GtpMsgType_E      msgType;

   GtpMsgHdr()
   {
      pres = 0;
      ver = 2;
      GSIM_SET_MASK(pres, GTP_MSG_T_BIT_PRES);
      len = 0;
      teid = 0;
      seqN = 0;
      msgType = GTPC_MSG_TYPE_INVALID;
   }
};

typedef struct
{
#define GTP_MAX_MCC_DIGITS       3
#define GTP_MAX_MNC_DIGITS       3

   U32      numMncDigits;
   U32      mcc[GTP_MAX_MCC_DIGITS];
   U32      mnc[GTP_MAX_MNC_DIGITS];
} GtpPlmnId_t;

typedef struct
{
   GtpPlmnId_t    plmnId;
   GtpLac_t       lac;
   GtpCellId_t    cellId;
} GtpCgi_t;

typedef struct
{
   GtpPlmnId_t    plmnId;
   GtpLac_t       lac;
   GtpSac_t       sac;
} GtpSai_t;

typedef struct
{
   GtpPlmnId_t    plmnId;
   GtpLac_t       lac;
   GtpRac_t       rac;
} GtpRai_t;

typedef struct
{
   GtpPlmnId_t    plmnId;
   GtpLac_t       lac;
   GtpTac_t       tac;
} GtpTai_t;

typedef struct
{
   GtpPlmnId_t    plmnId;
   GtpECellId_t   eCellId;    /* E-UTRAN Cell Id */
} GtpEcgi_t;

typedef U32 GtpApnAmbr_t;
typedef U32 GtpBitRate_t;

typedef enum
{
   GTP_RAT_TYPE_RESERVED,
   GTP_RAT_TYPE_UTRAN,
   GTP_RAT_TYPE_GERAN,
   GTP_RAT_TYPE_WLAN,
   GTP_RAT_TYPE_GAN,
   GTP_RAT_TYPE_HSPA_EVO,
   GTP_RAT_TYPE_EUTRAN,
   GTP_RAT_TYPE_MAX
} GtpRatType_E;

/**
 * @struct Stores Parameter details of Complex IEs that are read
 *    from xml scenario file
 */
struct IeParam
{
#define           IE_PARAM_NAME_MAX_LEN        64
   S8             paramName[IE_PARAM_NAME_MAX_LEN];
   Buffer         buf;

   ~IeParam()
   {
      delete[] buf.pVal;
   }
};

struct IeParam;
typedef std::list<IeParam*>      IeParamLst;
typedef IeParamLst::iterator     IeParamLstItr;

typedef enum
{
   GTPC_MSG_DIR_INV,
   GTPC_MSG_DIR_IN,
   GTPC_MSG_DIR_OUT
} GtpcMsgDir_E;

typedef enum
{
   GTP_PDN_TYPE_INVALID,
   GTP_PDN_TYPE_IPV4,
   GTP_PDN_TYPE_IPV6,
   GTP_PDN_TYPE_IPV4V6,
   GTP_PDN_TYPE_MAX
} GtpPdnType_E;

#endif /* _GTP_TYPES_HPP_ */
