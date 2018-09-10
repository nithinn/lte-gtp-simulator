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

#ifndef _GTP_IE_HPP_
#define _GTP_IE_HPP_

typedef struct
{
#define GTP_IMSI_MAX_BUF_LEN 8
    U32 len;
    U8  val[GTP_IMSI_MAX_BUF_LEN];
} GtpImsiKey;

class GtpImsi : public GtpIe
{
#define GTP_IMSI_MAX_DIGITS 15
private:
    U8 m_val[GTP_IMSI_MAX_BUF_LEN];

public:
    GtpImsi(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_IMSI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
        MEMSET(m_val, 0, GTP_IMSI_MAX_BUF_LEN);
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_IMSI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_IMSI_MAX_BUF_LEN);
    }

    VOID setImsi(GtpImsiKey *);
    BOOL isGroupedIe()
    {
        return FALSE;
    }

    const U8 *imsi()
    {
        return m_val;
    }
};

class GtpMsisdn : public GtpIe
{
#define GTP_MSISDN_MAX_BUF_LEN 8
#define GTP_MSISDN_MAX_DIGITS 15
private:
    U8 m_val[GTP_MSISDN_MAX_BUF_LEN];

public:
    GtpMsisdn(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MSISDN;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
        MEMSET(m_val, 0, GTP_MSISDN_MAX_BUF_LEN);
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_MSISDN_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_MSISDN_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpUli : public GtpIe
{
#define GTP_ULI_CGI_PRESENT (1 << 0)
#define GTP_ULI_SAI_PRESENT (1 << 1)
#define GTP_ULI_RAI_PRESENT (1 << 2)
#define GTP_ULI_TAI_PRESENT (1 << 3)
#define GTP_ULI_ECGI_PRESENT (1 << 4)
#define GTP_ULI_LAI_PRESENT (1 << 5)
#define GTP_ULI_MAX_BUF_LEN 44

private:
    U8 m_val[GTP_ULI_MAX_BUF_LEN];

public:
    GtpUli(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_ULI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
        MEMSET(m_val, 0, GTP_ULI_MAX_BUF_LEN);
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_ULI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_ULI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpBearerContext : public GtpIe
{
#define GTP_BEARER_CNTXT_MAX_BUF_LEN 256
private:
    U8 m_val[GTP_BEARER_CNTXT_MAX_BUF_LEN];

public:
    GtpBearerContext(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_BEARER_CNTXT;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    ~GtpBearerContext(){};

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_BEARER_CNTXT_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst);

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_BEARER_CNTXT_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return TRUE;
    }
    GtpEbi_t getEbi();
    VOID     setGtpuTeid(GtpTeid_t, GtpInstance_t);
};

class GtpFteid : public GtpIe
{
#define GTP_FTEID_MAX_BUF_LEN 25
#define GTP_FTEID_IPV4_ADDR_PRESENT (1 << 7)
#define GTP_FTEID_IPV6_ADDR_PRESENT (1 << 6)

private:
    U8 m_val[GTP_FTEID_MAX_BUF_LEN];

public:
    GtpFteid(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_FTEID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
        MEMSET(m_val, 0, GTP_FTEID_MAX_BUF_LEN);
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_FTEID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_FTEID_MAX_BUF_LEN);
    }

    VOID setTeid(GtpTeid_t teid);
    VOID setIpAddr(const IpAddr *pIp);
    BOOL isGroupedIe()
    {
        return FALSE;
    }
    GtpTeid_t getTeid();
};

class GtpEbi : public GtpIe
{
#define GTP_EBI_MAX_BUF_LEN 1

private:
    U8 m_val;

public:
    GtpEbi(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_EBI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, &m_val, GTP_EBI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(&m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, &m_val, GTP_EBI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMei : public GtpIe
{
#define GTP_MEI_MAX_BUF_LEN 8
#define GTP_MEI_MAX_DIGITS 16

private:
    U8 m_val[GTP_MEI_MAX_BUF_LEN];

public:
    GtpMei(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MEI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
        MEMSET(m_val, 0, GTP_MEI_MAX_BUF_LEN);
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_MEI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_MEI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpRatType : public GtpIe
{
#define GTP_RAT_TYPE_MAX_BUF_LEN 1
private:
    U8 m_val;

public:
    GtpRatType(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_RAT_TYPE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, &m_val, GTP_RAT_TYPE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(&m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, &m_val, GTP_RAT_TYPE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpServingNw : public GtpIe
{
#define GTP_SERVING_NW_MAX_STR_LEN 6
#define GTP_SERVING_NW_MAX_BUF_LEN 3

private:
    U8 m_val[GTP_SERVING_NW_MAX_BUF_LEN];

public:
    GtpServingNw(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_SERVING_NW;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
        MEMSET(m_val, 0, GTP_SERVING_NW_MAX_BUF_LEN);
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_SERVING_NW_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_SERVING_NW_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpApn : public GtpIe
{
#define GTP_APN_MAX_BUF_LEN 128

private:
    U8 m_val[GTP_APN_MAX_BUF_LEN];

public:
    GtpApn(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_APN;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
        MEMSET(m_val, 0, GTP_APN_MAX_BUF_LEN);
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_APN_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_APN_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpAmbr : public GtpIe
{
#define GTP_AMBR_MAX_BUF_LEN 8
private:
    U8 m_val[GTP_AMBR_MAX_BUF_LEN];

public:
    GtpAmbr(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_AMBR;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
        MEMSET(m_val, 0, GTP_AMBR_MAX_BUF_LEN);
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_AMBR_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_AMBR_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpIndication : public GtpIe
{
#define GTP_INDICATION_MAX_BUF_LEN 5
#define GTP_INDICATION_DAF_PRES (1 << 0)
#define GTP_INDICATION_DTF_PRES (1 << 1)
#define GTP_INDICATION_DFI_PRES (1 << 2)
#define GTP_INDICATION_OI_PRES (1 << 3)
#define GTP_INDICATION_ISRSI_PRES (1 << 4)
#define GTP_INDICATION_ISRAI_PRES (1 << 5)
#define GTP_INDICATION_SGWCI_PRES (1 << 6)
#define GTP_INDICATION_SQCI_PRES (1 << 7)
#define GTP_INDICATION_UIMSI_PRES (1 << 8)
#define GTP_INDICATION_CFSI_PRES (1 << 9)
#define GTP_INDICATION_P_PRES (1 << 10)
#define GTP_INDICATION_PT_PRES (1 << 11)
#define GTP_INDICATION_SI_PRES (1 << 12)
#define GTP_INDICATION_MSV_PRES (1 << 13)
#define GTP_INDICATION_ISRAU_PRES (1 << 14)
#define GTP_INDICATION_CCRSI_PRES (1 << 15)

private:
    U8 m_val[GTP_INDICATION_MAX_BUF_LEN];

public:
    GtpIndication(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_INDICATION;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
        MEMSET(m_val, 0, GTP_INDICATION_MAX_BUF_LEN);
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_INDICATION_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_INDICATION_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpSelectionMode : public GtpIe
{
#define GTP_SEL_MODE_MAX_BUF_LEN 1

#define GTP_SEL_MODE_MS_OR_NW_APN 0
#define GTP_SEL_MODE_MS_APN 1
#define GTP_SEL_MODE_NW_APN 2
#define GTP_SEL_MODE_FUTURE 3
private:
    U8 m_val;

public:
    GtpSelectionMode(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_SELECTION_MODE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, &m_val, GTP_SEL_MODE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(&m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, &m_val, GTP_SEL_MODE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpPdnType : public GtpIe
{
#define GTP_PDN_TYPE_MAX_BUF_LEN 1
private:
    U8 m_val;

public:
    GtpPdnType(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_PDN_TYPE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, &m_val, GTP_PDN_TYPE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(&m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, &m_val, GTP_PDN_TYPE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpPaa : public GtpIe
{
#define GTP_PAA_MAX_BUF_LEN 22
private:
    U8 m_val[GTP_PAA_MAX_BUF_LEN];

public:
    GtpPaa(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_PAA;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_PAA_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_PAA_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpBearerQos : public GtpIe
{
#define GTP_BEARER_QOS_MAX_BUF_LEN 18
private:
    U8 m_val[GTP_BEARER_QOS_MAX_BUF_LEN];

public:
    GtpBearerQos(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_BEARER_QOS;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_BEARER_QOS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_BEARER_QOS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpFlowQos : public GtpIe
{
#define GTP_FLOW_QOS_MAX_BUF_LEN 17
private:
    U8 m_val[GTP_FLOW_QOS_MAX_BUF_LEN];

public:
    GtpFlowQos(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_FLOW_QOS;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_FLOW_QOS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_FLOW_QOS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpPco : public GtpIe
{
#define GTP_PCO_MAX_BUF_LEN 253
private:
    U8 m_val[GTP_PCO_MAX_BUF_LEN];

public:
    GtpPco(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_PCO;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_PCO_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_PCO_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpCause : public GtpIe
{
#define GTP_CAUSE_MAX_BUF_LEN 10
#define GTP_CAUSE_CS_PRES (1 << 0)
#define GTP_CAUSE_BCE_PRES (1 << 1)
#define GTP_CAUSE_PCE_PRES (1 << 2)

private:
    U8 m_val[GTP_CAUSE_MAX_BUF_LEN];

public:
    GtpCause(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_CAUSE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_CAUSE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_CAUSE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpRecovery : public GtpIe
{
#define GTP_RECOVERY_MAX_BUF_LEN 4
private:
    U8 m_val;

public:
    GtpRecovery(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_RECOVERY;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, &m_val, GTP_RECOVERY_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(&m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, &m_val, GTP_RECOVERY_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMbmsSessionDuration : public GtpIe
{
#define GTP_MBMS_SESSION_DURATION_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MBMS_SESSION_DURATION_MAX_BUF_LEN];

public:
    GtpMbmsSessionDuration(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MBMS_SESSION_DURATION;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_MBMS_SESSION_DURATION_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_MBMS_SESSION_DURATION_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpStnSr : public GtpIe
{
#define GTP_STN_SR_MAX_BUF_LEN 15

private:
    U8 m_val[GTP_STN_SR_MAX_BUF_LEN];

public:
    GtpStnSr(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_STN_SR;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_STN_SR_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_STN_SR_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpIpAddress : public GtpIe
{
#define GTP_IP_ADDRESS_MAX_BUF_LEN 128
private:
    U8 m_val[GTP_IP_ADDRESS_MAX_BUF_LEN];

public:
    GtpIpAddress(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_IP_ADDR;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_IP_ADDRESS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_IP_ADDRESS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpEpsBearerTft : public GtpIe
{
#define GTP_EPS_BEARER_TFT_MAX_BUF_LEN 255
private:
    U8 m_val[GTP_EPS_BEARER_TFT_MAX_BUF_LEN];

public:
    GtpEpsBearerTft(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_EPS_BEARER_TFT;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_EPS_BEARER_TFT_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_EPS_BEARER_TFT_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpTad : public GtpIe
{
#define GTP_TAD_MAX_BUF_LEN 128
private:
    U8 m_val[GTP_TAD_MAX_BUF_LEN];

public:
    GtpTad(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_TAD;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_TAD_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_TAD_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpTmsi : public GtpIe
{
#define GTP_TMSI_MAX_BUF_LEN 32
private:
    U8 m_val[GTP_TMSI_MAX_BUF_LEN];

public:
    GtpTmsi(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_TMSI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_TMSI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_TMSI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpGlobalCnId : public GtpIe
{
#define GTP_GLOBAL_CN_ID_MAX_BUF_LEN 32
private:
    U8 m_val[GTP_GLOBAL_CN_ID_MAX_BUF_LEN];

public:
    GtpGlobalCnId(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_GLOBAL_CN_ID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_GLOBAL_CN_ID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_GLOBAL_CN_ID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpS103Pdf : public GtpIe
{
#define GTP_S103_PDN_DATA_FWD_INFO_MAX_BUF_LEN 128
private:
    U8 m_val[GTP_S103_PDN_DATA_FWD_INFO_MAX_BUF_LEN];

public:
    GtpS103Pdf(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_S103_PDF;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_S103_PDN_DATA_FWD_INFO_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_S103_PDN_DATA_FWD_INFO_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpS1uDf : public GtpIe
{
#define GTP_S1U_DATA_FWD_INFO_MAX_BUF_LEN 128
private:
    U8 m_val[GTP_S1U_DATA_FWD_INFO_MAX_BUF_LEN];

public:
    GtpS1uDf(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_S1UDF;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_S1U_DATA_FWD_INFO_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_S1U_DATA_FWD_INFO_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpDelayValue : public GtpIe
{
#define GTP_DELAY_VALUE_MAX_BUF_LEN 1
private:
    U8 m_val;

public:
    GtpDelayValue(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_DELAY_VALUE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, &m_val, GTP_DELAY_VALUE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(&m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, &m_val, GTP_DELAY_VALUE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpChargingId : public GtpIe
{
#define GTP_CHARGING_ID_MAX_BUF_LEN 4
private:
    U8 m_val[GTP_CHARGING_ID_MAX_BUF_LEN];

public:
    GtpChargingId(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_CHARGING_ID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_CHARGING_ID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_CHARGING_ID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpChargingCharcs : public GtpIe
{
#define GTP_CHARGING_CHARCS_MAX_BUF_LEN 2
private:
    U8 m_val[GTP_CHARGING_CHARCS_MAX_BUF_LEN];

public:
    GtpChargingCharcs(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_CHARGING_CHARACTERISTICS;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_CHARGING_CHARCS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_CHARGING_CHARCS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpTraceInfo : public GtpIe
{
#define GTP_TRACE_INFO_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_TRACE_INFO_MAX_BUF_LEN];

public:
    GtpTraceInfo(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_TRACE_INFO;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_TRACE_INFO_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_TRACE_INFO_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpBearerFlags : public GtpIe
{
#define GTP_BEARER_FLAGS_MAX_BUF_LEN 1
private:
    U8 m_val;

public:
    GtpBearerFlags(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_BEARER_FLAGS;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, &m_val, GTP_BEARER_FLAGS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf);
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(&m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, &m_val, GTP_BEARER_FLAGS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpPti : public GtpIe
{
#define GTP_PTI_MAX_BUF_LEN 1
private:
    U8 m_val;

public:
    GtpPti(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_PTI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, &m_val, GTP_PTI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    }

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(&m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, &m_val, GTP_PTI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpDrxParam : public GtpIe
{
#define GTP_DRX_PARAM_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_DRX_PARAM_MAX_BUF_LEN];

public:
    GtpDrxParam(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_DRX_PARAM;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_DRX_PARAM_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_DRX_PARAM_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpUeNetworkCap : public GtpIe
{
#define GTP_UE_NETWORK_CAP_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_UE_NETWORK_CAP_MAX_BUF_LEN];

public:
    GtpUeNetworkCap(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_UE_NETWORK_CAP;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_UE_NETWORK_CAP_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_UE_NETWORK_CAP_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMmCntxtGsmKeyAndTriplets : public GtpIe
{
#define GTP_MM_CNTXT_GSM_KEY_AND_TRIPLETS_MAX_BUF_LEN 255
private:
    U8 m_val[GTP_MM_CNTXT_GSM_KEY_AND_TRIPLETS_MAX_BUF_LEN];

public:
    GtpMmCntxtGsmKeyAndTriplets(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MM_CNTXT_GSM_KEY_N_TRIPLETS;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_MM_CNTXT_GSM_KEY_AND_TRIPLETS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_MM_CNTXT_GSM_KEY_AND_TRIPLETS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMmCntxtUmtsKeyUsedCipherAndQuint : public GtpIe
{
#define GTP_MM_CNTXT_UMTS_KEY_USED_CIPHER_AND_QUINTS_MAX_BUF_LEN 255
private:
    U8 m_val[GTP_MM_CNTXT_UMTS_KEY_USED_CIPHER_AND_QUINTS_MAX_BUF_LEN];

public:
    GtpMmCntxtUmtsKeyUsedCipherAndQuint(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MM_CNTXT_UMTS_KEY_USED_CIPHER_N_QUINT;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val,
            GTP_MM_CNTXT_UMTS_KEY_USED_CIPHER_AND_QUINTS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val,
            GTP_MM_CNTXT_UMTS_KEY_USED_CIPHER_AND_QUINTS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMmCntxtGsmKeyUsedCipherAndQuint : public GtpIe
{
#define GTP_MM_CNTXT_GSM_KEY_USED_CIPHER_N_QUINT_MAX_BUF_LEN 255
private:
    U8 m_val[GTP_MM_CNTXT_GSM_KEY_USED_CIPHER_N_QUINT_MAX_BUF_LEN];

public:
    GtpMmCntxtGsmKeyUsedCipherAndQuint(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MM_CNTXT_GSM_KEY_USED_CIPHER_N_QUINT;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_MM_CNTXT_GSM_KEY_USED_CIPHER_N_QUINT_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_MM_CNTXT_GSM_KEY_USED_CIPHER_N_QUINT_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMmCntxtUmtsKeyAndQuint : public GtpIe
{
#define GTP_MM_CNTXT_UMTS_KEY_AND_QUINTS_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MM_CNTXT_UMTS_KEY_AND_QUINTS_MAX_BUF_LEN];

public:
    GtpMmCntxtUmtsKeyAndQuint(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MM_CNTXT_UMTS_KEY_N_QUINT;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_MM_CNTXT_UMTS_KEY_AND_QUINTS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_MM_CNTXT_UMTS_KEY_AND_QUINTS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMmCntxtEpcSecCntxtQuadrAndQuint : public GtpIe
{
#define GTP_MM_CNTXT_EPS_SEC_CNTXT_QUADR_AND_QUITNS_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MM_CNTXT_EPS_SEC_CNTXT_QUADR_AND_QUITNS_MAX_BUF_LEN];

public:
    GtpMmCntxtEpcSecCntxtQuadrAndQuint(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MM_CNTXT_EPS_SEC_CNTXT_QUADR_N_QUINT;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val,
            GTP_MM_CNTXT_EPS_SEC_CNTXT_QUADR_AND_QUITNS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val,
            GTP_MM_CNTXT_EPS_SEC_CNTXT_QUADR_AND_QUITNS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMmCntxtUmtsKeyQuadrAndQuint : public GtpIe
{
#define GTP_MM_CNTXT_UMTS_KEY_QUADR_AND_QUINTS_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MM_CNTXT_UMTS_KEY_QUADR_AND_QUINTS_MAX_BUF_LEN];

public:
    GtpMmCntxtUmtsKeyQuadrAndQuint(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MM_CNTXT_UMTS_KEY_QUADR_N_QUINT;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_MM_CNTXT_UMTS_KEY_QUADR_AND_QUINTS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_MM_CNTXT_UMTS_KEY_QUADR_AND_QUINTS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpPdnConnection : public GtpIe
{
#define GTP_PDN_CONNECTION_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_PDN_CONNECTION_MAX_BUF_LEN];

public:
    GtpPdnConnection(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_PDN_CONNECTION;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_PDN_CONNECTION_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_PDN_CONNECTION_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpPduNumbers : public GtpIe
{
#define GTP_PDU_NUMBERS_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_PDU_NUMBERS_MAX_BUF_LEN];

public:
    GtpPduNumbers(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_PDU_NUMBERS;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_PDU_NUMBERS_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_PDU_NUMBERS_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpPtmsi : public GtpIe
{
#define GTP_PTMSI_MAX_BUF_LEN 4
private:
    U8 m_val[GTP_PTMSI_MAX_BUF_LEN];

public:
    GtpPtmsi(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_PTMSI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_PTMSI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_PTMSI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpPtmsiSignature : public GtpIe
{
#define GTP_PTMSI_SIGNAURE_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_PTMSI_SIGNAURE_MAX_BUF_LEN];

public:
    GtpPtmsiSignature(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_PTMSI_SIGNATURE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_PTMSI_SIGNAURE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_PTMSI_SIGNAURE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpHopCounter : public GtpIe
{
#define GTP_HOP_COUNTER_MAX_BUF_LEN 1
private:
    U8 m_val;

public:
    GtpHopCounter(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_HOP_COUNTER;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal);

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, &m_val, GTP_HOP_COUNTER_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(&m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, &m_val, GTP_HOP_COUNTER_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpUeTimeZone : public GtpIe
{
#define GTP_UE_TIME_ZONE_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_UE_TIME_ZONE_MAX_BUF_LEN];

public:
    GtpUeTimeZone(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_UE_TIME_ZONE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_UE_TIME_ZONE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_UE_TIME_ZONE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpTraceReference : public GtpIe
{
#define GTP_TRACE_REFERENCE_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_TRACE_REFERENCE_MAX_BUF_LEN];

public:
    GtpTraceReference(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_TRACE_REFERENCE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_TRACE_REFERENCE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_TRACE_REFERENCE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpCompleteReqMsg : public GtpIe
{
#define GTP_COMPLETE_REQ_MSG_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_COMPLETE_REQ_MSG_MAX_BUF_LEN];

public:
    GtpCompleteReqMsg(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_COMPLETE_REQ_MSG;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_COMPLETE_REQ_MSG_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_COMPLETE_REQ_MSG_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpGuti : public GtpIe
{
#define GTP_GUTI_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_GUTI_MAX_BUF_LEN];

public:
    GtpGuti(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_GUTI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_GUTI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_GUTI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpFContainer : public GtpIe
{
#define GTP_FCONTAINER_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_FCONTAINER_MAX_BUF_LEN];

public:
    GtpFContainer(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_FCONTAINER;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_FCONTAINER_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_FCONTAINER_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpFCause : public GtpIe
{
#define GTP_FCAUSE_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_FCAUSE_MAX_BUF_LEN];

public:
    GtpFCause(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_FCAUSE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_FCAUSE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_FCAUSE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpSelectedPlmnId : public GtpIe
{
#define GTP_SELECTED_PLMNID_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_SELECTED_PLMNID_MAX_BUF_LEN];

public:
    GtpSelectedPlmnId(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_SELECTED_PLMN_ID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_SELECTED_PLMNID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_SELECTED_PLMNID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpTargetId : public GtpIe
{
#define GTP_TARGET_ID_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_TARGET_ID_MAX_BUF_LEN];

public:
    GtpTargetId(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_TARGET_ID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_TARGET_ID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_TARGET_ID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpPacketFlowId : public GtpIe
{
#define GTP_PACKET_FLOW_ID_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_PACKET_FLOW_ID_MAX_BUF_LEN];

public:
    GtpPacketFlowId(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_PACKET_FLOW_ID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_PACKET_FLOW_ID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_PACKET_FLOW_ID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpRabCntxt : public GtpIe
{
#define GTP_RAB_CNTXT_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_RAB_CNTXT_MAX_BUF_LEN];

public:
    GtpRabCntxt(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_RAB_CNTXT;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_RAB_CNTXT_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_RAB_CNTXT_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpSourceRncPdcpCntxtInfo : public GtpIe
{
#define GTP_SOURCE_RNC_PDCP_CNTXT_INFO_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_SOURCE_RNC_PDCP_CNTXT_INFO_MAX_BUF_LEN];

public:
    GtpSourceRncPdcpCntxtInfo(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_SOURCE_RNC_PDCP_CNTXT_INFO;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_SOURCE_RNC_PDCP_CNTXT_INFO_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_SOURCE_RNC_PDCP_CNTXT_INFO_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpUdpSrcPort : public GtpIe
{
#define GTP_UDP_SRC_PORT_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_UDP_SRC_PORT_MAX_BUF_LEN];

public:
    GtpUdpSrcPort(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_UDP_SRC_PORT;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_UDP_SRC_PORT_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_UDP_SRC_PORT_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpApnRestriction : public GtpIe
{
#define GTP_APN_RESTRICTION_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_APN_RESTRICTION_MAX_BUF_LEN];

public:
    GtpApnRestriction(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_APN_RESTRICTION;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_APN_RESTRICTION_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_APN_RESTRICTION_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpSrcId : public GtpIe
{
#define GTP_SRC_ID_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_SRC_ID_MAX_BUF_LEN];

public:
    GtpSrcId(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_SRC_ID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_SRC_ID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_SRC_ID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpChangeReportingAction : public GtpIe
{
#define GTP_CHANGE_REPORTING_ACTION_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_CHANGE_REPORTING_ACTION_MAX_BUF_LEN];

public:
    GtpChangeReportingAction(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_CHANGE_REPORTING_ACTION;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_CHANGE_REPORTING_ACTION_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_CHANGE_REPORTING_ACTION_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpFqdn : public GtpIe
{
#define GTP_FQDN_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_FQDN_MAX_BUF_LEN];

public:
    GtpFqdn(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_FQDN;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_FQDN_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_FQDN_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpChannelNeeded : public GtpIe
{
#define GTP_CHANNEL_NEEDED_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_CHANNEL_NEEDED_MAX_BUF_LEN];

public:
    GtpChannelNeeded(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_CHANNEL_NEEDED;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_CHANNEL_NEEDED_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_CHANNEL_NEEDED_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpEmlppPriority : public GtpIe
{
#define GTP_EMLPP_PRIORITY_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_EMLPP_PRIORITY_MAX_BUF_LEN];

public:
    GtpEmlppPriority(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_EMLPP_PRIORITY;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_EMLPP_PRIORITY_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_EMLPP_PRIORITY_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpNodeType : public GtpIe
{
#define GTP_NODE_TYPE_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_NODE_TYPE_MAX_BUF_LEN];

public:
    GtpNodeType(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_NODE_TYPE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_NODE_TYPE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_NODE_TYPE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpFqCsid : public GtpIe
{
#define GTP_FQ_CSID_MAX_BUF_LEN 128
private:
    U8 m_val[GTP_FQ_CSID_MAX_BUF_LEN];

public:
    GtpFqCsid(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_FQ_CSID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_FQ_CSID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_FQ_CSID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpTi : public GtpIe
{
#define GTP_TI_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_TI_MAX_BUF_LEN];

public:
    GtpTi(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_TI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_TI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_TI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMbmsServiceArea : public GtpIe
{
#define GTP_MBMS_SERVICE_AREA_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MBMS_SERVICE_AREA_MAX_BUF_LEN];

public:
    GtpMbmsServiceArea(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MBMS_SERVICE_AREA;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_MBMS_SERVICE_AREA_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_MBMS_SERVICE_AREA_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMbmsSessionId : public GtpIe
{
#define GTP_MBMS_SESSION_ID_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MBMS_SESSION_ID_MAX_BUF_LEN];

public:
    GtpMbmsSessionId(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MBMS_SESSION_ID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_MBMS_SESSION_ID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_MBMS_SESSION_ID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMbmsFlowId : public GtpIe
{
#define GTP_MBMS_FLOW_ID_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MBMS_FLOW_ID_MAX_BUF_LEN];

public:
    GtpMbmsFlowId(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MBMS_FLOW_ID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_MBMS_FLOW_ID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_MBMS_FLOW_ID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMbmsIpMulticastDistribution : public GtpIe
{
#define GTP_MBMS_IP_MULTICAST_DISTRIBUTION_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MBMS_IP_MULTICAST_DISTRIBUTION_MAX_BUF_LEN];

public:
    GtpMbmsIpMulticastDistribution(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MBMS_IP_MULTICAST_DISTRIBUTION;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_MBMS_IP_MULTICAST_DISTRIBUTION_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_MBMS_IP_MULTICAST_DISTRIBUTION_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMbmsDistributionAck : public GtpIe
{
#define GTP_MBMS_DISTRIBUTION_ACK_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MBMS_DISTRIBUTION_ACK_MAX_BUF_LEN];

public:
    GtpMbmsDistributionAck(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MBMS_DISTRIBUTION_ACK;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_MBMS_DISTRIBUTION_ACK_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_MBMS_DISTRIBUTION_ACK_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpRfspIndex : public GtpIe
{
#define GTP_RFSP_INDEX_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_RFSP_INDEX_MAX_BUF_LEN];

public:
    GtpRfspIndex(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_RFSP_INDEX;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_RFSP_INDEX_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_RFSP_INDEX_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpUci : public GtpIe
{
#define GTP_UCI_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_UCI_MAX_BUF_LEN];

public:
    GtpUci(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_UCI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_UCI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_UCI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpCsgInfoReportingAction : public GtpIe
{
#define GTP_CSG_INFO_REPORTING_ACTION_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_CSG_INFO_REPORTING_ACTION_MAX_BUF_LEN];

public:
    GtpCsgInfoReportingAction(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_CSG_INFO_REPORTING_ACTION;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_CSG_INFO_REPORTING_ACTION_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_CSG_INFO_REPORTING_ACTION_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpCsgId : public GtpIe
{
#define GTP_CSG_ID_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_CSG_ID_MAX_BUF_LEN];

public:
    GtpCsgId(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_CSG_ID;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_CSG_ID_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_CSG_ID_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpCmi : public GtpIe
{
#define GTP_CMI_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_CMI_MAX_BUF_LEN];

public:
    GtpCmi(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_CMI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_CMI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_CMI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpServiceIndicator : public GtpIe
{
#define GTP_SERVICE_INDICATOR_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_SERVICE_INDICATOR_MAX_BUF_LEN];

public:
    GtpServiceIndicator(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_SERVICE_INDICATOR;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_SERVICE_INDICATOR_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_SERVICE_INDICATOR_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpDetachType : public GtpIe
{
#define GTP_DETACH_TYPE_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_DETACH_TYPE_MAX_BUF_LEN];

public:
    GtpDetachType(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_DETACH_TYPE;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_DETACH_TYPE_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_DETACH_TYPE_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpLdn : public GtpIe
{
#define GTP_LDN_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_LDN_MAX_BUF_LEN];

public:
    GtpLdn(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_LDN;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_LDN_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_LDN_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpMbmsTimeToDataTransfer : public GtpIe
{
#define GTP_MBMS_TIME_TO_DATA_TRANSFER_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_MBMS_TIME_TO_DATA_TRANSFER_MAX_BUF_LEN];

public:
    GtpMbmsTimeToDataTransfer(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_MBMS_TIME_TO_DATA_TRANSFER;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_MBMS_TIME_TO_DATA_TRANSFER_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_MBMS_TIME_TO_DATA_TRANSFER_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpTmgi : public GtpIe
{
#define GTP_TMGI_MAX_BUF_LEN 1
private:
    U8 m_val[GTP_TMGI_MAX_BUF_LEN];

public:
    GtpTmgi(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_TMGI;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(value, m_val, GTP_TMGI_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(inbuf, m_val, GTP_TMGI_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpAdditionalMmCntxtForSrvcc : public GtpIe
{
#define GTP_ADDITIONAL_MM_CNTXT_FOR_SRVCC_MAX_BUF_LEN 256
private:
    U8 m_val[GTP_ADDITIONAL_MM_CNTXT_FOR_SRVCC_MAX_BUF_LEN];

public:
    GtpAdditionalMmCntxtForSrvcc(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_ADDITIONAL_MM_CNTXT_FOR_SRVCC;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_ADDITIONAL_MM_CNTXT_FOR_SRVCC_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_ADDITIONAL_MM_CNTXT_FOR_SRVCC_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

class GtpAdditionalFlagsForSrvcc : public GtpIe
{
#define GTP_ADDITIONAL_FLAGS_FOR_SRVCC_MAX_BUF_LEN 256
private:
    U8 m_val[GTP_ADDITIONAL_FLAGS_FOR_SRVCC_MAX_BUF_LEN];

public:
    GtpAdditionalFlagsForSrvcc(GtpInstance_t inst)
    {
        m_hdr.ieType   = GTP_IE_ADDITIONAL_FLAGS_FOR_SRVCC;
        m_hdr.instance = inst;
        m_hdr.len      = 0;
    }

    RETVAL buildIe(const S8 *pVal)
    {
        return ROK;
    }

    RETVAL buildIe(const HexString *value)
    {
        return buildIeHelper(
            value, m_val, GTP_ADDITIONAL_FLAGS_FOR_SRVCC_MAX_BUF_LEN);
    }

    RETVAL buildIe(IeParamLst *pBuf)
    {
        return ROK;
    }
    RETVAL buildIe(const GtpIeLst *pIeLst)
    {
        return ROK;
    };

    GtpLength_t encode(U8 *outbuf)
    {
        return encodeHelper(m_val, outbuf);
    }

    GtpLength_t decode(const U8 *inbuf)
    {
        return decodeHelper(
            inbuf, m_val, GTP_ADDITIONAL_FLAGS_FOR_SRVCC_MAX_BUF_LEN);
    }

    BOOL isGroupedIe()
    {
        return FALSE;
    }
};

#endif
