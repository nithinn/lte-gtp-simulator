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

#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdio.h>
#include <string.h>

typedef int                S32;
typedef unsigned int       U32;
typedef char               S8;
typedef unsigned short     U16;
typedef unsigned char      U8;
typedef unsigned char      BOOL;
typedef void               VOID;
typedef unsigned int       RETVAL;
typedef short              S16;
typedef unsigned int       Counter;
typedef unsigned long      U64;
typedef U64                Time_t;
typedef U32                TransConnId;

#define EXTERN          extern
#define PRIVATE         static
#define PUBLIC          

#ifndef TRUE
#define TRUE            ((BOOL)1)
#endif

#ifndef FALSE
#define FALSE           ((BOOL)0)
#endif

#define ROK             0
#define RFAILED         1

#define MEMSET          memset
#define MEMCPY          memcpy
#define MEMCMP          memcmp
#define STRCPY          strcpy
#define STRNCPY         strncpy
#define STRLEN          strlen
#define STRCMP          strcmp
#define STRCASECMP      strcasecmp
#define STRFIND         strstr

typedef enum
{
   IP_ADDR_TYPE_INV,
   IP_ADDR_TYPE_V4,
   IP_ADDR_TYPE_V6
} IpAddrTypeEn;

typedef struct
{
#define IPV4_ADDR_MAX_LEN       4
   U32   addr;
} Ipv4Addr;

typedef struct
{
#define IPV6_ADDR_MAX_LEN       16

   U8    len;
   U8    addr[IPV6_ADDR_MAX_LEN];
} Ipv6Addr;

typedef struct
{
    IpAddrTypeEn     ipAddrType;
    union
    {
        Ipv4Addr    ipv4Addr;
        Ipv6Addr    ipv6Addr;
    } u;
} IpAddr;

typedef struct
{
   U16         port;
   IpAddr      ipAddr;
} IPEndPoint;

typedef enum
{
   MSG_ACTION_INVALID,
   MSG_ACTION_SEND,
   MSG_ACTION_RECV,
   MSG_ACTION_MAX
} MsgAction_E;

struct Buffer
{
   U32      len;
   U8       *pVal;
};

struct UdpData
{
   Buffer         buf;
   TransConnId    connId;
   IPEndPoint     peerEp; 
};


#define BUFFER_CPY(_buf, _src, _sz)                     \
do                                                      \
{                                                       \
   (_buf)->len = _sz;                                   \
   (_buf)->pVal = new U8 [_sz];                         \
   memcpy((VOID *)(_buf)->pVal, (VOID *)(_src), _sz);   \
} while (0)

#define GSIM_SIZE_32             32

#endif /* _TYPES_H_ */
