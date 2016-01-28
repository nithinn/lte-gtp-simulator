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

/* Common macros used across the application */

#ifndef __MACROS_HPP__
#define __MACROS_HPP__

#define GSIM_CHK_MASK(_var, _mask)  ((_var) & (_mask))
#define GSIM_SET_MASK(_var, _mask)  ((_var) |= (_mask))
#define GSIM_UNSET_MASK(_var, _mask)  ((_var) &= ~(_mask))
#define GSIM_IS_ODD(_num)            ((_num) & 0x1)
#define GSIM_IS_EVEN(_num)           (!(GSIM_IS_ODD(_num)))

/* Converts character '0' to '9' to Digit */
#define GSIM_CHAR_TO_DIGIT(_c) (((_c) - '0'))
#define GSIM_DIGIT_TO_CHAR(_c) (((_c) + '0'))

/* Rounds UP to the next integer after divistion */
#define GSIM_CEIL_DIVISION(_a, _b) (1 + (((_a) - 1) / (_b)))

#define GSIM_ENC_U8(_buf, _v)                      \
{                                                  \
   _buf[0] = (U8)(0xff & _v);                      \
}

#define GSIM_DEC_U8(_buf, _v)                      \
{                                                  \
   (_v) = 0;                                       \
   (_v) = (U8)(_buf[0] & 0xff);                    \
}

#define GSIM_DEC_U16(_buf, _v)                     \
{                                                  \
   (_v) = 0;                                       \
   (_v) |= ((U16)(_buf[0])) << 8;                  \
   (_v) |= ((U16)(_buf[1]));                       \
}

#define GSIM_DEC_U32(_buf, _v)                     \
{                                                  \
   (_v) = 0;                                       \
   (_v) |= ((U32)(_buf[0])) << 24;                 \
   (_v) |= ((U32)(_buf[1])) << 16;                 \
   (_v) |= ((U32)(_buf[2])) << 8;                  \
   (_v) |= ((U32)(_buf[3]));                       \
}

#define GSIM_ENC_U32(_buf, _v)                     \
{                                                  \
   _buf[0] = (U8)((0xff000000 & _v) >> 24);        \
   _buf[1] = (U8)((0x00ff0000 & _v) >> 16);        \
   _buf[2] = (U8)((0x0000ff00 & _v) >> 8);         \
   _buf[3] = (U8)((0x000000ff & _v));              \
}

/* Encodes least significant 3 bytes of unsigned int into byte buffer */
#define GSIM_ENC_3B(_buf, _v)                      \
{                                                  \
   _buf[0] = (U8)((0x00ff0000 & _v) >> 16);        \
   _buf[1] = (U8)((0x0000ff00 & _v) >> 8);         \
   _buf[2] = (U8)((0x000000ff & _v));              \
}

#endif
