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

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <exception>
#include <poll.h>
#include <sys/select.h>
#include <string.h>
#include <list>

#include "types.hpp"
#include "macros.hpp"
#include "logger.hpp"
#include "error.hpp"
#include "thread.hpp"
#include "transport.hpp"
#include "keyboard.hpp"
#include "gtp_types.hpp"
#include "socket.hpp"
#include "sim_cfg.hpp"
#include "gtp_macro.hpp"

/******************* Function Declarations ***********************************/
EXTERN VOID procGtpcMsg(UdpData_t *data);
PRIVATE RETVAL sendMsgV4(GSimSocket *pSock, IPEndPoint *pDst, Buffer *data);
PRIVATE RETVAL sendMsgV6(GSimSocket *pSock, IPEndPoint *pDst, Buffer *data);
PRIVATE RETVAL handleGtpcSock(GSimSocket     *pSock);
PRIVATE RETVAL handleGtpuSock(GSimSocket     *pSock);
PRIVATE VOID handleStdinSock(GSimSocket     *pSock);
/******************* Function Declarations ***********************************/

GSimSocket           *g_gsimSockArr[GSIM_MAX_SOCK_CNT];
GSimPollFd           s_pollFdArr[GSIM_MAX_POLL_FDS];
static U32           s_pollFdCnt = 0;
static GSimSocket    *s_pListener = NULL;
static GSimSocket    *s_pSender = NULL;
static U8            s_recvBuf[GSIM_UDP_READ_LEN];

/**
 * @brief 
 *    Reads the UDP socket, allocates socket buffer
 *
 * @return 
 */
RETVAL GSimSocket::recvMsgV4(UdpData_t **msg)
{
   struct sockaddr_in  fromAddr;
   socklen_t           fromLen = sizeof(sockaddr_in);

   S32 recvLen = recvfrom(m_fd, s_recvBuf, GSIM_UDP_READ_LEN,\
         MSG_DONTWAIT, (struct sockaddr *)&fromAddr, &fromLen);
   if (recvLen > 0)
   {
      *msg = new UdpData_t;
      BUFFER_CPY(&(*msg)->buf, s_recvBuf, recvLen);
      (*msg)->connId = m_pollFdIndex;
      (*msg)->peerEp.ipAddr.ipAddrType = IP_ADDR_TYPE_V4;
      (*msg)->peerEp.ipAddr.u.ipv4Addr.addr = 
         ntohl(fromAddr.sin_addr.s_addr);
      (*msg)->peerEp.port = ntohs(fromAddr.sin_port);
      return ROK;
   }

   return RFAILED;
}

/**
 * @brief 
 *    Peeks the gtp message, and reads the message of gtp msg length
 *    and fills the UdpData_t structre from IPv6 socket
 *
 * @param data
 *
 * @return 
 */
RETVAL GSimSocket::recvMsgV6(UdpData_t **msg)
{
   struct sockaddr_in6 fromAddr;
   socklen_t           fromLen = 0;

   U32 recvLen = recvfrom(m_fd, s_recvBuf, GSIM_UDP_READ_LEN, MSG_DONTWAIT,
         (struct sockaddr *)&fromAddr, &fromLen);
   if (recvLen > 0)
   {
      *msg = new UdpData_t;
      BUFFER_CPY(&(*msg)->buf, s_recvBuf, recvLen);
      (*msg)->connId = m_pollFdIndex;
      (*msg)->peerEp.ipAddr.ipAddrType = IP_ADDR_TYPE_V4;
      MEMCPY((*msg)->peerEp.ipAddr.u.ipv6Addr.addr,
            fromAddr.sin6_addr.s6_addr, IPV6_ADDR_MAX_LEN);
      (*msg)->peerEp.port = ntohs(fromAddr.sin6_port);
      return ROK;
   }

   return RFAILED;
}

RETVAL GSimSocket::recvMsg(UdpData_t **msg)
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (IP_ADDR_TYPE_V4 == m_ep.ipAddr.ipAddrType)
   {
      ret = recvMsgV4(msg);
   }
   else
   {
      ret = recvMsgV6(msg);
   }

   LOG_EXITFN(ret);
}

PRIVATE RETVAL sendMsgV4(GSimSocket *pSock, IPEndPoint *pDst, Buffer *data)
{
   LOG_ENTERFN();

   struct sockaddr_in  destAddr;

   destAddr.sin_addr.s_addr = htonl(pDst->ipAddr.u.ipv4Addr.addr);
   destAddr.sin_family = AF_INET;
   destAddr.sin_port = htons(pDst->port);
   MEMSET(destAddr.sin_zero, '\0', sizeof(destAddr.sin_zero));

   S32 ret = sendto(pSock->fd(), (VOID *)data->pVal, (size_t)data->len,\
         MSG_DONTWAIT, (struct sockaddr *)&destAddr, sizeof(destAddr));
   if (ret < 0)
   {
      LOG_FATAL("Socket sendto() failed, [%s]", strerror(errno));
      LOG_EXITFN(ERR_SYS_SOCK_SEND);
   }

   delete data;
   LOG_EXITFN(ROK);
}


PRIVATE RETVAL sendMsgV6(GSimSocket *pSock, IPEndPoint *pDst, Buffer *data)
{
   struct sockaddr_in6  destAddr;

   MEMCPY(destAddr.sin6_addr.s6_addr, pDst->ipAddr.u.ipv6Addr.addr,\
         pDst->ipAddr.u.ipv6Addr.len);
   destAddr.sin6_family = AF_INET6;
   destAddr.sin6_port = htons(pDst->port);

   if (sendto(pSock->fd(), data->pVal, data->len,
         MSG_DONTWAIT, (struct sockaddr *)&destAddr, sizeof(destAddr)) > 0)
   {
      return ROK;
   }

   delete data;
   return RFAILED;
}


PUBLIC VOID socketPoll(S32 wait)
{
   S32 rs; /* Number of times to execute recv().
            * For TCP with 1 socket per call: no. of events returned by poll
            * For UDP and TCP with 1 global socket:recv_count is a flag that 
            * stays up as long as there's data to read
            */

   /* Get socket events. */
   rs = poll(s_pollFdArr, s_pollFdCnt, wait);
   if((rs < 0) && (errno == EINTR))
   {
      LOG_ERROR("poll() error, [%s]", strerror(errno));
      return;
   }

   for (U32 pollIndx = 0; rs > 0 && pollIndx < s_pollFdCnt; pollIndx++)
   {
      GSimSocket *pSock = g_gsimSockArr[pollIndx];

      if (NULL == pSock)
      {
         LOG_FATAL("Null Socket, Sock Array. Index [%d]", pollIndx);
         continue;
      }

      if (GSIM_CHK_MASK(s_pollFdArr[pollIndx].revents, POLLOUT))
      {
         GSIM_UNSET_MASK(s_pollFdArr[pollIndx].events, POLLOUT);
      }

      if (GSIM_CHK_MASK(s_pollFdArr[pollIndx].revents, POLLIN))
      {
         rs--;

         switch (pSock->type()) 
         {
            case SOCK_TYPE_GTPC:
            {
               LOG_DEBUG("Reading GTP-C socket");
               RETVAL ret = handleGtpcSock(pSock);
               if (ROK != ret)
               {
                  LOG_ERROR("Reading GTP-C socket");
               }

               break;
            }

            case SOCK_TYPE_GTPU:
            {
               LOG_DEBUG("Reading GTP-U socket");
               RETVAL ret = handleGtpuSock(pSock);
               if (ROK != ret)
               {
                  LOG_ERROR("Reading GTP-U socket");
               }

               break;
            }

            case SOCK_TYPE_STDIN:
            {
               LOG_DEBUG("Reading Keyboard Event");
               handleStdinSock(pSock);
               break;
            }

            default:
            {
               break;
            }
         }
      }
      else if (GSIM_CHK_MASK(s_pollFdArr[pollIndx].revents, POLLERR))
      {
         LOG_FATAL("Socket Error, FD [%d]", pSock->fd());
         delete pSock;
      }

      s_pollFdArr[pollIndx].revents = 0;
   }
}


/**
 * @brief
 *    Handles keyboard event
 *
 * @param pSock
 */
PRIVATE VOID handleStdinSock
(
GSimSocket     *pSock
)
{
   LOG_ENTERFN();

   if (feof(stdin))
   {
      delete pSock;
   }

   S32 input = getchar();
   if (input == EOF)
   {
      LOG_EXITVOID();
   }
   else
   {
      Keyboard::getInstance()->processKey(input);
   }

   LOG_EXITVOID();
}

/**
 * @brief
 *    Hanldes GTP-C socket, reads GTP-C messages
 *
 * @param pSock
 *
 * @return 
 */
PRIVATE RETVAL handleGtpcSock(GSimSocket *pSock)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   U32      loops = GSIM_MAX_RECV_LOOPS;

   while (loops && (ROK == ret))
   {
      UdpData_t *msg = NULL;
      ret = pSock->recvMsg(&msg);
      if (ROK == ret)
      {
         LOG_DEBUG("Process the Received messages", pSock->fd());
         procGtpcMsg(msg);
      }

      loops--;
   };

   LOG_EXITFN(ROK);
}

/**
 * @brief
 *    Hanldes GTP-U socket, reads GTP-U Process control messages
 *
 * @param pSock
 *
 * @return 
 */
PRIVATE RETVAL handleGtpuSock
(
GSimSocket     *pSock
)
{
   LOG_ENTERFN();

   RETVAL      ret = ROK;


   LOG_EXITFN(ret);
}

PUBLIC RETVAL initTransport()
{
   LOG_ENTERFN();

   S16            ret = ROK;
   IPEndPoint     locListnerEp;
   IPEndPoint     locSenderEp;

   Config          *pCfg = Config::getInstance();

   for (U32 i = 0; i < GSIM_MAX_POLL_FDS; i++)
   {
      s_pollFdArr[i].fd = -1;
   }

   /* Simulator sends all GTP messages with source udp port number as 
    * Default GTP port + 1, using this socket
    */
   locSenderEp.port = pCfg->getLocalGtpcPort() + 1;
   locSenderEp.ipAddr = *pCfg->getLocalIpAddr();
   s_pSender = new GSimSocket(SOCK_TYPE_GTPC, locSenderEp);
   if (ROK != s_pSender->bindSocket())
   {
      LOG_FATAL("Binding to GTP local sending Socket");
   }

   /* This is the default GTPC socket, where the simlator listens
    * for initiating messages
    */
   locListnerEp.port = pCfg->getLocalGtpcPort();
   locListnerEp.ipAddr = *pCfg->getLocalIpAddr();
   s_pListener = new GSimSocket(SOCK_TYPE_GTPC, locListnerEp);
   if (ROK != s_pListener->bindSocket())
   {
      LOG_FATAL("Binding to GTP Listener Socket");
   }

   LOG_EXITFN(ret);
}

GSimSocket::GSimSocket(SockType_t sockType)
{
   if (SOCK_TYPE_STDIN == sockType)
   {
      m_fd = fileno(stdin);
      m_type = sockType;
      m_pollFdIndex = s_pollFdCnt++;
      g_gsimSockArr[m_pollFdIndex] = this;
      s_pollFdArr[m_pollFdIndex].fd = m_fd;
      s_pollFdArr[m_pollFdIndex].events  = POLLIN | POLLERR;
      s_pollFdArr[m_pollFdIndex].revents = 0;
   }
   else
   {
      throw ERR_INV_SOCKET_TYPE;
   }
}

GSimSocket::GSimSocket(SockType_t sockType, IPEndPoint ep)
{
   if (SOCK_TYPE_STDIN != sockType)
   {
      if (IP_ADDR_TYPE_V4 == ep.ipAddr.ipAddrType)
      {
         m_fd = socket(AF_INET, SOCK_DGRAM, 0);
      }
      else
      {
         m_fd = socket(AF_INET6, SOCK_DGRAM, 0);
      }

      if (m_fd < 0)
      {
         LOG_FATAL("socket system call, [%s]", strerror(errno));
         throw ERR_SYS_SOCKET_CREATE;
      }

      if (fcntl(m_fd, F_SETFL, O_NONBLOCK) < 0)
      {
         LOG_FATAL("socket system call, [%s]", strerror(errno));
         throw ERR_SYS_SOCK_CNTRL;
      }

      m_type = sockType;
      m_pollFdIndex = s_pollFdCnt++;
      m_ep = ep;
      g_gsimSockArr[m_pollFdIndex] = this;
      s_pollFdArr[m_pollFdIndex].fd = m_fd;
      s_pollFdArr[m_pollFdIndex].events  = POLLIN | POLLERR;
      s_pollFdArr[m_pollFdIndex].revents = 0;

      U32 sockRecvBuf = GSIM_MAX_SOCKET_RECV_BUF;
      if (setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, &sockRecvBuf,\
            sizeof(sockRecvBuf)) < 0)
      {
         LOG_ERROR("setsockopt() Failed, [%s]", strerror(errno));
      }

      U32 sockSendBuf = GSIM_MAX_SOCKET_SEND_BUF;
      if (setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, &sockSendBuf,\
            sizeof(sockSendBuf)) < 0)
      {
         LOG_ERROR("setsockopt() Failed, [%s]", strerror(errno));
      }
   }
   else
   {
      throw ERR_INV_SOCKET_TYPE;
   }
}

S32 GSimSocket::fd()
{
   return m_fd;
}

IpAddrTypeEn GSimSocket::ipAddrType()
{
   return m_ep.ipAddr.ipAddrType;
}

SockType_t GSimSocket::type()
{
   return m_type;
}

GSimSocket::~GSimSocket()
{
   LOG_DEBUG("Deallocating socket, Sock FD [%d]", m_fd);

   s_pollFdCnt--;
   s_pollFdArr[m_pollFdIndex].fd = 0;
   g_gsimSockArr[m_pollFdIndex] = NULL;
   close(m_fd);
}

RETVAL GSimSocket::bindSocket()
{
   S16                  ret = ROK;
   struct sockaddr_in   addr;
   struct sockaddr_in6  addr6;

   if (IP_ADDR_TYPE_V4 == m_ep.ipAddr.ipAddrType)
   {
      addr.sin_addr.s_addr = htonl(m_ep.ipAddr.u.ipv4Addr.addr);
      addr.sin_family = AF_INET;
      addr.sin_port = htons(m_ep.port);
      MEMSET(addr.sin_zero, '\0', sizeof(addr.sin_zero));
   }
   else
   {
      MEMCPY(addr6.sin6_addr.s6_addr, m_ep.ipAddr.u.ipv6Addr.addr,
            m_ep.ipAddr.u.ipv6Addr.len);
      addr6.sin6_family = AF_INET6;
      addr6.sin6_port = htons(m_ep.port);
   }

   if (IP_ADDR_TYPE_V4 == m_ep.ipAddr.ipAddrType)
   {
      ret = bind(m_fd, (struct sockaddr *)&addr, sizeof(addr));
   }
   else
   {
      ret = bind(m_fd, (struct sockaddr *)&addr6, sizeof(addr6));
   }

   if (ret < 0)
   {
      LOG_FATAL("Socket Binding Failed, [%s]", strerror(errno));
      return ERR_SYS_SOCKET_BIND;
   }

   return ROK;
}


/**
 * @brief
 *    Creates the socket for listening on Keyboard events from the user
 */
PUBLIC RETVAL setupStdinSock()
{
   LOG_ENTERFN();

   S16      ret = ROK;

   try
   {
      GSimSocket  *pKbSock = new GSimSocket(SOCK_TYPE_STDIN);
      (VOID)pKbSock;
   }
   catch (ErrCodeEn  &e)
   {
      ret = ERR_SOCK_ALLOC;
   }

   LOG_EXITFN(ret);
}

PUBLIC RETVAL sendMsg
(
TransConnId          connId,               
IPEndPoint           *pDst,
Buffer               *data
)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;

   GSimSocket  *pSock = g_gsimSockArr[connId];
   if (NULL != pSock)
   {
      if (pDst->ipAddr.ipAddrType == IP_ADDR_TYPE_V4)
      {
         ret = sendMsgV4(pSock, pDst, data);
      }
      else
      {
         ret = sendMsgV6(pSock, pDst, data);
      }
   }

   LOG_EXITFN(ret);
}


