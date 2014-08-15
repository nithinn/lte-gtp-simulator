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

#define GSIM_UDP_READ_LEN        65536
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
} SockType_E;

typedef struct pollfd   GSimPollFd;

class GSimSockBuf
{
   public:
   U32                  len;           /* length of pBuf                */
   U32                  pendingBytes;  /* bytes to be processed         */
   U8                   *pBuf;         /* buffer                        */
   U32                  offset;        /* offset at which processing 
                                        * has to continue
                                        */
   TransConnId          connId;        /* used to identify the socket
                                        * while sending response msgw
                                        */
   IPEndPoint           peerEp;        /* Peer endpoint                 */

   GSimSockBuf()
   {
      len          = 0;
      pBuf         = NULL;
      offset       = 0;
      pendingBytes = 0;
      connId       = 0;
      MEMSET(&peerEp, 0, sizeof(IPEndPoint));
   }

   ~GSimSockBuf()
   {
      delete[] pBuf;
   }
};

typedef std::list<GSimSockBuf *>  GSimSockBufLst;
typedef GSimSockBufLst::iterator  GSimSockBufLstItr;

class GSimSocket
{
   public:
      GSimSocket(SockType_E);
      GSimSocket(SockType_E, IPEndPoint);
      ~GSimSocket();

      S32               fd();
      SockType_E        type();
      IpAddrTypeEn      ipAddrType();
      RETVAL            bindSocket();
      RETVAL            recvMsg();
      VOID              procSockBuf();

   private:
      S32               m_fd;
      U32               m_pollFdIndex;
      SockType_E        m_type;
      IPEndPoint        m_ep;
      RETVAL            recvMsgV6();
      RETVAL            recvMsgV4();
      VOID              formUdpData(U8*, U32, GSimSockBuf*, UdpData**);
      VOID              mergeToNextBuf(GSimSockBuf *pCurr, GSimSockBuf *pNext);
      U32               getFullGtpMsgLen(U8* pBuf, U32 len);
      GSimSockBufLst    m_sockBufLst;
};

#endif

