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


// GTP Message class

#ifndef _GTP_MSG_HPP_
#define _GTP_MSG_HPP_

class GtpMsg
{
   public:
      GtpMsg(GtpMsgType_t);
      GtpMsg(Buffer*);
      ~GtpMsg();

      RETVAL            encode(GtpIeLst *pIeLst);
      RETVAL            encode(U8 *pBuf, U32 *pLen);
      RETVAL            decode();
      GtpMsgType_t      type() {return m_msgHdr.msgType;}
      VOID              setMsgHdr(const GtpMsgHdr* pHdr);
      RETVAL            setSenderFteid(GtpTeid_t teid, const IpAddr *pIp);
      GtpIe*            getIe(GtpIeType_t, GtpInstance_t, U32);
      U32               getIeCount(GtpIeType_t ieType, GtpInstance_t inst);
      U8*               getIeBufPtr(GtpIeType_t, GtpInstance_t, U32);
      GtpSeqNumber_t    seqNumber() {return m_msgHdr.seqN;}
      VOID              setImsi(GtpImsiKey*);
      GtpTeid_t         getTeid();
      GtpMsgCategory_t  category();
      U32               getBearersToCreate() {return m_bearersToCreate;}

   private:
      GtpMsgHdr      m_msgHdr;
      U8             m_gtpMsgBuf[GTP_MSG_BUF_LEN];
      GtpIeLst       m_ieLst;
      U8             m_bearersToCreate;
      U8             m_bearersToDelete;
      U8             m_bearersToModify;
      U32            encodeHdr(U8 *pBuf);
      VOID           decodeHdr(U8 *pBuf);
      VOID           updateBearerCount(GtpInstance_t bearerCntxtInst);
};

#endif /* _GTP_MSG_HPP_ */
