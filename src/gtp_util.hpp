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

#include <types.hpp>
#include <logger.hpp>
#include <macros.hpp>
#include <gtp_macro.hpp>
#include <gtp_types.hpp>

#define GTP_MSG_NAME_LEN 64
#define GTP_IE_NAME_LEN 64
#define GTP_MSG_XML_TAG_MAX_LEN 16

S8 *             gtpGetMsgName(GtpMsgType_t msgType);
GtpIeType_t      gtpGetIeType(const S8 *pIeName);
GtpMsgType_t     gtpGetMsgType(const S8 *pXmlMsgTag);
GtpMsgCategory_t gtpGetMsgCategory(GtpMsgType_t msgType);
GtpLength_t      gtpConvStrToHex(const Buffer *pBuf, U8 *pDst);
GtpLength_t      gtpConvStrToHex(const HexString *value, U8 *pDst);
EXTERN U32  gtpConvStrToU32(const S8 *pVal, U32 len);
GtpIfType_t gtpConvStrToIfType(const S8 *pVal, U32 len);
IpAddr      convIpStrToIpAddr(const S8 *pIp, U32 len);
VOID        decIeHdr(U8 *pBuf, GtpIeHdr *pHdr);
U32         encodeImsi(S8 *pImsiStr, U32 imsiStrLen, U8 *pBuf);
EXTERN VOID numericStrIncriment(S8 *pStr, U32 len);
PUBLIC U8 *getImsiBufPtr(Buffer *pGtpcBuf);
EXTERN VOID gtpUtlEncPlmnId(GtpPlmnId_t *pPlmnId, U8 *pBuf);
PUBLIC S8 *gtpGetIeName(GtpIeType_t ieType);
PUBLIC U8 gtpCharToHex(U8 c);

#endif
