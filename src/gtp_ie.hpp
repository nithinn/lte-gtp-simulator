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
      GtpImsi()
      {
         hdr.ieType = GTP_IE_IMSI;
         hdr.instance = 0;
         hdr.len = 0;
         MEMSET(m_val, 0, GTP_IMSI_MAX_LEN);
      }

      RETVAL encode(const U8 *pVal);
      RETVAL encode(XmlBuffer *pBuf);
      RETVAL encode(XmlBufferLst *pBuf) {return ROK;}
      RETVAL encode(const GtpIeLst *pIeLst) {return ROK;}
      RETVAL encode(U8 *pBuf, U32 *pLen);

      RETVAL decode(const Buffer *pBuf);
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
      GtpMsisdn()
      {
         hdr.ieType = GTP_IE_MSISDN;
         hdr.instance = 0;
         hdr.len = 0;
         MEMSET(m_val, 0, GTP_MSISDN_MAX_LEN);
      }

      RETVAL encode(const U8 *pVal);
      RETVAL encode(XmlBuffer *pBuf);
      RETVAL encode(XmlBufferLst *pBuf) {return ROK;}
      RETVAL encode(const GtpIeLst *pIeLst) {return ROK;}
      RETVAL encode(U8 *pBuf, U32 *pLen);

      RETVAL decode(const Buffer *pBuf);
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
      GtpUli()
      {
         hdr.ieType = GTP_IE_ULI;
         hdr.instance = 0;
         hdr.len = 0;
         m_pres = 0;
         MEMSET(m_val, 0, GTP_ULI_MAX_LEN);
      }

      RETVAL encode(const U8 *pVal) {return ROK;}
      RETVAL encode(XmlBuffer *pBuf);
      RETVAL encode(XmlBufferLst *pBuf);
      RETVAL encode(const GtpIeLst *pIeLst) {return ROK;}
      RETVAL encode(U8 *pBuf, U32 *pLen);

      RETVAL decode(const Buffer *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};


class GtpBearerContext : public GtpIe
{
#define              GTP_BEARER_CNTXT_MAX_LEN   256
   private:
      GtpIeLst       m_ieLst;
      U8             *m_pVal;

   public:
      GtpBearerContext()
      {
         hdr.ieType = GTP_IE_BEARER_CNTXT;
         hdr.instance = 0;
         hdr.len = 0;
         m_pVal = NULL;
      }

      ~GtpBearerContext();

      RETVAL encode(const U8 *pVal) {return ROK;}
      RETVAL encode(XmlBuffer *pBuf);
      RETVAL encode(XmlBufferLst *pBuf) {return ROK;}
      RETVAL encode(const GtpIeLst *pIeLst);
      RETVAL encode(U8 *pBuf, U32 *pLen);

      RETVAL decode(const Buffer *pBuf);
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
      GtpFteid()
      {
         hdr.ieType = GTP_IE_FTEID;
         hdr.instance = 0;
         hdr.len = 0;
         m_pres = 0;
         m_teid = 0;
         MEMSET(&m_ipv4, 0, sizeof(Ipv4Addr));
         MEMSET(&m_ipv6, 0, sizeof(Ipv6Addr));
         m_ifType = GTP_IF_INF_INV;
         MEMSET(m_val, 0, GTP_FTEID_MAX_LEN);
      }

      RETVAL encode(const U8 *pVal) {return ROK;}
      RETVAL encode(XmlBuffer *pBuf);
      RETVAL encode(XmlBufferLst *pBuf);
      RETVAL encode(const GtpIeLst *pIeLst) {return ROK;};
      RETVAL encode(U8 *pBuf, U32 *pLen);

      RETVAL decode(const Buffer *pBuf);
      VOID   setTeid(GtpTeid_t teid);
      VOID   setIpAddr(const IpAddr *pIp);
      BOOL   isGroupedIe() {return FALSE;}
      GtpTeid_t   getTeid() {return m_teid;}
};

class GtpEbi : public GtpIe
{
   private:
      U8          m_ebi;

   public:
      GtpEbi()
      {
         hdr.ieType = GTP_IE_EBI;
         hdr.instance = 0;
         hdr.len = 0;
         m_ebi = 0;
      }

      RETVAL encode(const U8 *pVal);
      RETVAL encode(XmlBuffer *pBuf) {return ROK;}
      RETVAL encode(XmlBufferLst *pBuf) {return ROK;}
      RETVAL encode(const GtpIeLst *pIeLst) {return ROK;};
      RETVAL encode(U8 *pBuf, U32 *pLen);

      RETVAL decode(const Buffer *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};

class GtpMei : public GtpIe
{
#define GTP_MEI_MAX_LEN          8
#define GTP_MEI_MAX_DIGITS       16
   private:
      U8       m_val[GTP_MEI_MAX_LEN];

   public:
      GtpMei()
      {
         hdr.ieType = GTP_IE_MEI;
         hdr.instance = 0;
         hdr.len = 0;
         MEMSET(m_val, 0, GTP_MEI_MAX_LEN);
      }

      RETVAL encode(const U8 *pVal);
      RETVAL encode(XmlBuffer *pBuf);
      RETVAL encode(XmlBufferLst *pBuf) {return ROK;}
      RETVAL encode(const GtpIeLst *pIeLst) {return ROK;}
      RETVAL encode(U8 *pBuf, U32 *pLen);

      RETVAL decode(const Buffer *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};

class GtpRatType : public GtpIe
{
   private:
      GtpRatType_E      m_ratType;

   public:
      GtpRatType()
      {
         hdr.ieType = GTP_IE_RAT_TYPE;
         hdr.instance = 0;
         hdr.len = 0;
         m_ratType = GTP_RAT_TYPE_RESERVED;
      }

      RETVAL encode(const U8 *pVal);
      RETVAL encode(XmlBuffer *pBuf) {return ROK;}
      RETVAL encode(XmlBufferLst *pBuf) {return ROK;}
      RETVAL encode(const GtpIeLst *pIeLst) {return ROK;};
      RETVAL encode(U8 *pBuf, U32 *pLen);

      RETVAL decode(const Buffer *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};

class GtpServingNw : public GtpIe
{
#define GTP_SERVING_NW_MAX_LEN      6

   private:
      U8             m_val[GTP_SERVING_NW_MAX_LEN];
      GtpPlmnId_t    m_plmnId;

   public:
      GtpServingNw()
      {
         hdr.ieType = GTP_IE_SERVING_NW;
         hdr.instance = 0;
         hdr.len = 0;
         MEMSET(&m_plmnId, 0, sizeof(GtpPlmnId_t));
         MEMSET(m_val, 0, GTP_SERVING_NW_MAX_LEN);
      }

      RETVAL encode(const U8 *pVal);
      RETVAL encode(XmlBuffer *pBuf);
      RETVAL encode(XmlBufferLst *pBuf) {return ROK;}
      RETVAL encode(const GtpIeLst *pIeLst) {return ROK;}
      RETVAL encode(U8 *pBuf, U32 *pLen);

      RETVAL decode(const Buffer *pBuf);
      BOOL   isGroupedIe() {return FALSE;}
};

#endif
