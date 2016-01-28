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
#define GTP_IMSI_MAX_LEN         8
   U32   len;
   U8    val[GTP_IMSI_MAX_LEN];
} GtpImsiKey;


class GtpImsi : public GtpIe
{
#define GTP_IMSI_MAX_DIGITS      15
   private:
      U8          m_val[GTP_IMSI_MAX_LEN];

   public:
      GtpImsi(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_IMSI;
         hdr.instance = inst;
         hdr.len = 0;
         MEMSET(m_val, 0, GTP_IMSI_MAX_LEN);
      }

      RETVAL buildIe(const S8 *pVal);
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;}
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      VOID   setImsi(GtpImsiKey*);
      BOOL   isGroupedIe() {return FALSE;}

      const U8* imsi() {return m_val;}
};

class GtpMsisdn : public GtpIe
{
#define GTP_MSISDN_MAX_LEN          8
#define GTP_MSISDN_MAX_DIGITS       15
   private:
      U8       m_val[GTP_MSISDN_MAX_LEN];

   public:
      GtpMsisdn(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_MSISDN;
         hdr.instance = inst;
         hdr.len = 0;
         MEMSET(m_val, 0, GTP_MSISDN_MAX_LEN);
      }

      RETVAL buildIe(const S8 *pVal);
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;}
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};


class GtpUli : public GtpIe
{
   private:
#define GTP_ULI_CGI_PRESENT         (1 << 0)
#define GTP_ULI_SAI_PRESENT         (1 << 1)
#define GTP_ULI_RAI_PRESENT         (1 << 2)
#define GTP_ULI_TAI_PRESENT         (1 << 3)
#define GTP_ULI_ECGI_PRESENT        (1 << 4)
#define GTP_ULI_LAI_PRESENT         (1 << 5)
      U8          m_pres;

      GtpCgi_t    m_cgi;
      GtpSai_t    m_sai;
      GtpRai_t    m_rai;
      GtpTai_t    m_tai;
      GtpEcgi_t   m_ecgi;

#define GTP_ULI_MAX_LEN             44
      U8          m_val[GTP_ULI_MAX_LEN];

   public:
      GtpUli(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_ULI;
         hdr.instance = inst;
         hdr.len = 0;
         m_pres = 0;
         MEMSET(m_val, 0, GTP_ULI_MAX_LEN);
      }

      RETVAL buildIe(const S8 *pVal) {return ROK;}
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf);
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;}
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};


class GtpBearerContext : public GtpIe
{
#define              GTP_BEARER_CNTXT_MAX_LEN   256
   private:
      GtpIeLst       m_ieLst;
      U8             *m_pVal;

   public:
      GtpBearerContext(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_BEARER_CNTXT;
         hdr.instance = inst;
         hdr.len = 0;
         m_pVal = NULL;
      }

      ~GtpBearerContext();

      RETVAL buildIe(const S8 *pVal) {return ROK;}
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst);
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return TRUE;}
      GtpEbi_t getEbi();
      VOID   setGtpuTeid(GtpTeid_t, GtpInstance_t);
};

class GtpFteid : public GtpIe
{
   private:
#define GTP_FTEID_IPV4_ADDR_PRESENT       (1 << 7)
#define GTP_FTEID_IPV6_ADDR_PRESENT       (1 << 6)
      U8                   m_pres;
      Ipv4Addr             m_ipv4;
      Ipv6Addr             m_ipv6;
      GtpIfType_E          m_ifType;
      GtpTeid_t            m_teid;
#define GTP_FTEID_MAX_LEN  25
      U8                   m_val[GTP_FTEID_MAX_LEN];

   public:
      GtpFteid(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_FTEID;
         hdr.instance = inst;
         hdr.len = 0;
         m_pres = 0;
         m_teid = 0;
         MEMSET(&m_ipv4, 0, sizeof(Ipv4Addr));
         MEMSET(&m_ipv6, 0, sizeof(Ipv6Addr));
         m_ifType = GTP_IF_INF_INV;
         MEMSET(m_val, 0, GTP_FTEID_MAX_LEN);
      }

      RETVAL buildIe(const S8 *pVal) {return ROK;}
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf);
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;};
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      VOID   setTeid(GtpTeid_t teid);
      VOID   setIpAddr(const IpAddr *pIp);
      BOOL   isGroupedIe() {return FALSE;}
      GtpTeid_t   getTeid() {return m_teid;}
};

class GtpEbi : public GtpIe
{
   private:
      U8          m_ebi;
      U8          m_val;

   public:
      GtpEbi(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_EBI;
         hdr.instance = inst;
         hdr.len = 0;
         m_ebi = 0;
      }

      RETVAL buildIe(const S8 *pVal);
      RETVAL buildIe(const HexString *value) {return ROK;}
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;};
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};

class GtpMei : public GtpIe
{
#define GTP_MEI_MAX_LEN          8
#define GTP_MEI_MAX_DIGITS       16
   private:
      U8       m_val[GTP_MEI_MAX_LEN];

   public:
      GtpMei(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_MEI;
         hdr.instance = inst;
         hdr.len = 0;
         MEMSET(m_val, 0, GTP_MEI_MAX_LEN);
      }

      RETVAL buildIe(const S8 *pVal);
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;}
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};

class GtpRatType : public GtpIe
{
   private:
      GtpRatType_E      m_ratType;
      U8                m_val;

   public:
      GtpRatType(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_RAT_TYPE;
         hdr.instance = inst;
         hdr.len = 0;
         m_ratType = GTP_RAT_TYPE_RESERVED;
      }

      RETVAL buildIe(const S8 *pVal);
      RETVAL buildIe(const HexString *value) {return ROK;}
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;};
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};

class GtpServingNw : public GtpIe
{
#define GTP_SERVING_NW_MAX_STR_LEN      6
#define GTP_SERVING_NW_MAX_BUF_LEN      3

   private:
      U8             m_val[GTP_SERVING_NW_MAX_BUF_LEN];
      GtpPlmnId_t    m_plmnId;

   public:
      GtpServingNw(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_SERVING_NW;
         hdr.instance = inst;
         hdr.len = 0;
         MEMSET(&m_plmnId, 0, sizeof(GtpPlmnId_t));
         MEMSET(m_val, 0, GTP_SERVING_NW_MAX_BUF_LEN);
      }

      RETVAL buildIe(const S8 *pVal);
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;}
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};

class GtpApn : public GtpIe
{
#define GTP_APN_MAX_LEN      128

   private:
      U8             m_val[GTP_APN_MAX_LEN];

   public:
      GtpApn(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_APN;
         hdr.instance = inst;
         hdr.len = 0;
         MEMSET(m_val, 0, GTP_APN_MAX_LEN);
      }

      RETVAL buildIe(const S8 *pVal);
      RETVAL buildIe(const HexString *value) {return ROK;}
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;}
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};

class GtpAmbr : public GtpIe
{
#define GTP_AMBR_MAX_BUF_LEN        8
   private:
      GtpApnAmbr_t   ul;
      GtpApnAmbr_t   dl;
      U8             m_val[GTP_AMBR_MAX_BUF_LEN];

   public:
      GtpAmbr(GtpInstance_t inst)
      {
         hdr.ieType   = GTP_IE_AMBR;
         hdr.instance = inst;
         hdr.len      = 0;
         ul           = 0;
         dl           = 0;
         MEMSET(m_val, 0, GTP_AMBR_MAX_BUF_LEN);
      }

      RETVAL buildIe(const S8 *pVal) {return ROK;}
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf);
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;}
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};


class GtpIndication : public GtpIe
{
#define GTP_INDICATION_MAX_BUF_LEN        3

#define GTP_INDICATION_DAF_PRES           (1 << 0)
#define GTP_INDICATION_DTF_PRES           (1 << 1)
#define GTP_INDICATION_DFI_PRES           (1 << 2)
#define GTP_INDICATION_OI_PRES            (1 << 3)
#define GTP_INDICATION_ISRSI_PRES         (1 << 4)
#define GTP_INDICATION_ISRAI_PRES         (1 << 5)
#define GTP_INDICATION_SGWCI_PRES         (1 << 6)
#define GTP_INDICATION_SQCI_PRES          (1 << 7)
#define GTP_INDICATION_UIMSI_PRES         (1 << 8)
#define GTP_INDICATION_CFSI_PRES          (1 << 9)
#define GTP_INDICATION_P_PRES             (1 << 10)
#define GTP_INDICATION_PT_PRES            (1 << 11)
#define GTP_INDICATION_SI_PRES            (1 << 12)
#define GTP_INDICATION_MSV_PRES           (1 << 13)
#define GTP_INDICATION_ISRAU_PRES         (1 << 14)
#define GTP_INDICATION_CCRSI_PRES         (1 << 15)

   private:
      U32      bitmask;
      U8       m_val[GTP_INDICATION_MAX_BUF_LEN];

   public:
      GtpIndication(GtpInstance_t inst)
      {
         hdr.ieType   = GTP_IE_INDICATION;
         hdr.instance = inst;
         hdr.len      = 0;
         MEMSET(m_val, 0, GTP_INDICATION_MAX_BUF_LEN);
      }

      RETVAL buildIe(const S8 *pVal) {return ROK;}
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf);
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;}
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};


class GtpSelectionMode : public GtpIe
{
#define GTP_SEL_MODE_BUF_MAX_LEN       1

#define GTP_SEL_MODE_MS_OR_NW_APN      0
#define GTP_SEL_MODE_MS_APN            1
#define GTP_SEL_MODE_NW_APN            2
#define GTP_SEL_MODE_FUTURE            3
   private:
      U8       m_selMode;
      U8       m_val;

   public:
      GtpSelectionMode(GtpInstance_t inst)
      {
         hdr.ieType   = GTP_IE_SELECTION_MODE;
         hdr.instance = inst;
         hdr.len      = 0;
         m_selMode = 0;
      }

      RETVAL buildIe(const S8 *pVal);
      RETVAL buildIe(const HexString *value) {return ROK;}
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;}
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};


class GtpPdnType : public GtpIe
{
#define GTP_PDN_TYPE_MAX_BUF_LEN    1
   private:
      GtpPdnType_E   m_pdnType;
      U8             m_val;

   public:
      GtpPdnType(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_PDN_TYPE;
         hdr.instance = inst;
         hdr.len = 0;
         m_pdnType = GTP_PDN_TYPE_IPV4;
      }

      RETVAL buildIe(const S8 *pVal);
      RETVAL buildIe(const HexString *value) {return ROK;}
      RETVAL buildIe(XmlBufferLst *pBuf) {return ROK;}
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;};
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};


class GtpPaa : public GtpIe
{
#define GTP_PAA_MAX_BUF_LEN    22
   private:
      GtpPdnType_E   m_pdnType;
      U8             m_val[GTP_PAA_MAX_BUF_LEN];

   public:
      GtpPaa(GtpInstance_t inst)
      {
         hdr.ieType = GTP_IE_PAA;
         hdr.instance = inst;
         hdr.len = 0;
         m_pdnType = GTP_PDN_TYPE_IPV4;
      }

      RETVAL buildIe(const S8 *pVal) {return ROK;}
      RETVAL buildIe(const HexString *value);
      RETVAL buildIe(XmlBufferLst *pBuf);
      RETVAL buildIe(const GtpIeLst *pIeLst) {return ROK;};
      GtpLength_t encode(U8 *pBuf);

      GtpLength_t decode(const U8 *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};
#endif
