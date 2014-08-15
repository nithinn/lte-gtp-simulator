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

// GTP utility functions

#ifndef _GTP_UTIL_HPP_
#define _GTP_UTIL_HPP_

#define GTP_MSG_NAME_LEN         64
#define GTP_IE_NAME_LEN          64
#define GTP_MSG_XML_TAG_MAX_LEN  16

S8* gtpGetMsgName(GtpMsgType_E msgType);
GtpIeType_E gtpGetIeType(const S8   *pIeName);
GtpMsgType_E gtpGetMsgType(const S8 *pXmlMsgTag);
GtpMsgCategory_E gtpGetMsgCategory(GtpMsgType_E msgType);
GtpLength_t gtpConvStrToHex(const Buffer *pBuf, U8 *pDst);
VOID gtpEncIeUsingHexBuf(U8 *pSrc, GtpIeHdr *pHdr, U8 *pDst, U32 *pDstLen);
EXTERN U32 gtpConvStrToU32(const S8 *pVal, U32 len);
GtpIfType_E gtpConvStrToIfType(const S8 *pVal, U32 len);
IpAddr convIpStrToIpAddr(const S8* pIp, U32 len);
VOID decIeHdr(U8 *pBuf, GtpIeHdr *pHdr);
U32 encodeImsi(S8 *pImsiStr, U32 imsiStrLen, U8 *pBuf);
EXTERN VOID numericStrIncriment(S8 *pStr, U32 len);
PUBLIC U8* getImsiBufPtr(Buffer *pGtpcBuf);

#endif
