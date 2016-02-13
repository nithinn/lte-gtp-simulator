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

#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define GSIM_UDP_READ_LEN        2048
#define GTP_HDR_PEEK_LEN         4
#define GSIM_MAX_SOCK_CNT        3
#define GSIM_MAX_POLL_FDS        32
#define GSIM_MAX_RECV_LOOPS      1000

typedef enum
{
   SOCK_TYPE_INVALID,
   SOCK_TYPE_STDIN,
   SOCK_TYPE_GTPC,
   SOCK_TYPE_GTPU,
   SOCK_TYPE_GTPU_CTRL,
   SOCK_TYPE_MAX
} SockType_t;

typedef struct pollfd   GSimPollFd;

class GSimSocket
{
   public:
      GSimSocket(SockType_t);
      GSimSocket(SockType_t, IPEndPoint);
      ~GSimSocket();

      S32               fd();
      SockType_t        type();
      IpAddrTypeEn      ipAddrType();
      RETVAL            bindSocket();
      RETVAL            recvMsg(UdpData **msg);

   private:
      S32               m_fd;
      U32               m_pollFdIndex;
      SockType_t        m_type;
      IPEndPoint        m_ep;
      RETVAL            recvMsgV6(UdpData **msg);
      RETVAL            recvMsgV4(UdpData **msg);
      U32               getFullGtpMsgLen(U8* pBuf, U32 len);
};

#endif

