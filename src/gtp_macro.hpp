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

#ifndef _GTP_MACRO_HPP_
#define _GTP_MACRO_HPP_

#define GTP_MSG_GET_TYPE(_buf, _type)                       \
{                                                           \
   _type = (GtpMsgType_E)(_buf[1]);                         \
}

#define GTP_MSG_GET_LEN(_buf, _len)                         \
{                                                           \
   _len = (GtpLength_t)((_buf[2] << 8) | (_buf[3]));        \
}

#define GTP_MSG_GET_SEQN(_buf, _seq)                        \
{                                                           \
   U32 _s = 0;                                              \
   if (GTP_CHK_T_BIT_PRESENT(_buf))                         \
   {                                                        \
      _s  = (U32)((_buf[8] << 16));                         \
      _s  = (U32)((_buf[9] << 8));                          \
      _s  = (U32)((_buf[10]));                              \
      _seq = (GtpSeqNumber_t)_s;                            \
   }                                                        \
   else                                                     \
   {                                                        \
      _s  = (U32)((_buf[4] << 16));                         \
      _s  = (U32)((_buf[5] << 8));                          \
      _s  = (U32)((_buf[6]));                               \
      _seq = (GtpSeqNumber_t)_s;                            \
   }                                                        \
}                     

#define GTP_SET_MSG_TYPE(_buf, _mtype)    \
{                                         \
   _buf[1] = (U8)(_mtype);                \
}

#define GTP_SET_MSG_LEN(_buf, _len)          \
{                                            \
   _buf[2] = (U8)((_len - 4) & 0xff00) >> 8; \
   _buf[3] = (U8)((_len - 4) & 0x00ff);      \
}

#define GTP_MSG_DEC_TEID(_buf, _teid)        \
{                                            \
   U8    *_tbuf = _buf + 4;                  \
   _teid = 0;                                \
   _teid |= ((U32)(_tbuf[0])) << 24;         \
   _teid |= ((U32)(_tbuf[1])) << 16;         \
   _teid |= ((U32)(_tbuf[2])) << 8;          \
   _teid |= ((U32)(_tbuf[3]));               \
}

#define GTP_CHK_TBIT(_buf)    ((_buf[0]) & 0x08)
   
#define GTP_GET_IE_LEN(_buf, _len)           \
{                                            \
   _len = (U16)((_buf[1] << 8) | (_buf[2])); \
}

#define GTP_GET_IE_TYPE(_buf, _type)         \
{                                            \
   _type = (GtpIeType_E)(_buf[0]);           \
}

#define GTP_GET_IE_INSTANCE(_buf, _ins)      \
{                                            \
   _ins = (GtpInstance_t)(_buf[3]);          \
}

#define GTP_GET_INSTANCE(buf)       ()((buf[1] << 8) | (buf[2]))

#define ENCODE_IMSI_DIGIT(_buf, _n, _d) {          \
   U8 _shift = 0;                                  \
   if (_n % 2 == 0) {                              \
      _shift = 4;                                  \
   }                                               \
   _buf[(_n - 1)/ 2] |= ((U8)(_d << _shift));      \
}

#endif

#define GTP_ENC_MSG_TYPE(_buf, _type)        \
{                                            \
   _buf[0] = (U8)(_type & 0xff);             \
}

#define GTP_ENC_LEN(_buf, _len)              \
{                                            \
   _buf[0] = (U8)((_len & 0xff00) >> 8);     \
   _buf[1] = (U8)(_len & 0x00ff);            \
}

#define GTP_ENC_TEID(_buf, _teid)                  \
{                                                  \
   _buf[0] = (U8)(((_teid) & 0xff000000) >> 24);   \
   _buf[1] = (U8)(((_teid) & 0x00ff0000) >> 16);   \
   _buf[2] = (U8)(((_teid) & 0x0000ff00) >> 8);    \
   _buf[3] = (U8)(((_teid) & 0x000000ff));         \
}

#define GTP_DEC_TEID(_buf, _teid)                  \
{                                                  \
   _teid = 0;                                      \
   _teid |= ((U32)((_buf)[0])) << 24;                \
   _teid |= ((U32)((_buf)[1])) << 16;                \
   _teid |= ((U32)((_buf)[2])) << 8;                 \
   _teid |= ((U32)((_buf)[3]));                      \
}

#define GTP_ENC_SEQN(_buf, _seqN)                  \
{                                                  \
   _buf[0] = (U8)(((_seqN) & 0x00ff0000) >> 16);   \
   _buf[1] = (U8)(((_seqN) & 0x0000ff00) >> 8);    \
   _buf[2] = (U8)(((_seqN) & 0x000000ff));         \
}

#define GTP_ENC_IE_LENGTH(_buf, _len)              \
{                                                  \
   _buf[0] = (U8)(((_len) & 0xff00) >> 8);         \
   _buf[1] = (U8)(((_len) & 0x00ff));              \
}

#define GTP_ENC_IE_TYPE(_buf, _type)               \
{                                                  \
   _buf[0] = (U8)(_type & 0xff);                   \
}

#define GTP_ENC_IE_INSTANCE(_buf, _inst)           \
{                                                  \
   _buf[0] = (U8)(_inst & 0x0f);                   \
}

#define GTP_ENC_IE_HDR(_buf, _hdr)                 \
{                                                  \
   U8 *_tmp = _buf;                                \
   GTP_ENC_IE_TYPE(_tmp, (_hdr)->ieType);          \
   _tmp += 1;                                      \
   GTP_ENC_IE_LENGTH(_tmp, (_hdr)->len);           \
   _tmp += 2;                                      \
   GTP_ENC_IE_INSTANCE(_tmp, (_hdr)->instance);    \
}

#define GTP_ENC_IPV4_ADDR(_buf, _addr)             \
{                                                  \
   _buf[0] = (U8)(((_addr) & 0xff000000) >> 24);   \
   _buf[1] = (U8)(((_addr) & 0x00ff0000) >> 16);   \
   _buf[2] = (U8)(((_addr) & 0x0000ff00) >> 8);    \
   _buf[3] = (U8)(((_addr) & 0x000000ff));         \
}

#define GTP_ENC_IPV6_ADDR(_buf, _addr)             \
{                                                  \
   MEMCPY(_buf, _addr, IPV6_ADDR_MAX_LEN);         \
}

#define GTP_ENC_EBI(_buf, _ebi)                    \
{                                                  \
   GSIM_ENC_U8(_buf, _ebi);                        \
}

#define GTP_DEC_IE_TYPE(_buf, _ieType)             \
{                                                  \
   U8 _t = 0;                                      \
   GSIM_DEC_U8(_buf, _t);                          \
   _ieType = (GtpIeType_E)_t;                      \
}

#define GTP_DEC_IE_LEN(_buf, _ieLen)               \
{                                                  \
   GSIM_DEC_U16((_buf + 1), _ieLen);               \
}

#define GTP_DEC_IE_INSTANCE(_buf, _inst)           \
{                                                  \
   GSIM_DEC_U8((_buf + 3), _inst);                 \
}

#define GTP_DEC_EBI(_buf, _ebi)                    \
{                                                  \
   U8 _e = 0;                                      \
   GSIM_DEC_U8((_buf + GTP_IE_HDR_LEN), _e);       \
   _ebi = (GtpEbi_t)_e;                            \
}

#define GTP_CHK_T_BIT_PRESENT(_buf)   ((_buf)[0] & GTP_MSG_T_BIT_PRES)
#define GTP_CHK_P_BIT_PRESENT(_buf)   ((_buf)[0] & GTP_MSG_P_BIT_PRES)

#define GTP_DEC_RAT_TYPE(_buf, _ebi)               \
{                                                  \
   U8 _e = 0;                                      \
   GSIM_DEC_U8((_buf + GTP_IE_HDR_LEN), _e);       \
   _ebi = (GtpRatType_E)_e;                        \
}

#define GTP_ENC_RAT_TYPE(_buf, _ebi)               \
{                                                  \
   GSIM_ENC_U8(_buf, _ebi);                        \
}

#define GTP_ENC_APN_AMBR(_buf, _apnambr)           \
{                                                  \
   GSIM_ENC_U32(_buf, _apnambr);                   \
}

#define GTP_ENC_BIT_RATE(_buf, _br)                \
{                                                  \
   GSIM_ENC_U32(_buf, _br);                        \
}

#define GTP_ENC_SEL_MODE(_buf, _sel)               \
{                                                  \
   GSIM_ENC_U8(_buf, _sel);                        \
}

#define GTP_ENC_PDN_TYPE(_buf, _v)                 \
{                                                  \
   GSIM_ENC_U8(_buf, _v);                          \
}

#define GTP_ENC_ARP(_buf, _v)                      \
{                                                  \
   GSIM_ENC_U8(_buf, _v);                          \
}

#define GTP_ENC_QCI(_buf, _v)                      \
{                                                  \
   GSIM_ENC_U8(_buf, _v);                          \
}

#define GTP_ENC_CAUSE_VALUE(_buf, _v)              \
do                                                 \
{                                                  \
   GSIM_ENC_U8(_buf, _v);                          \
} while (0);

#define GTP_ENC_RECOVERY(_buf, _v)                 \
do                                                 \
{                                                  \
   GSIM_ENC_U8(_buf, _v);                          \
} while (0);

#define GTP_ENC_HOP_COUNTER(_buf, _v)              \
do                                                 \
{                                                  \
   GSIM_ENC_U8(_buf, _v);                          \
} while (0);
