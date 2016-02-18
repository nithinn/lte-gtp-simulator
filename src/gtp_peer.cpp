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

#include <vector>
using std::vector;

#include "types.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "timer.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"
#include "gtp_types.hpp"
#include "gtp_macro.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "gtp_peer.hpp"

static PeerDataVec g_peerData;

PUBLIC BOOL isOldReq(PeerData *peer, Buffer *gtpMsg)
{
   LOG_ENTERFN();

   BOOL              isOld     = FALSE;
   GtpSeqNumber_t    seqNumber = 0;

   GTP_MSG_GET_SEQN(gtpMsg->pVal, seqNumber);
   if (peer->seqNumber > seqNumber)
   {
      isOld = TRUE;
   }

   LOG_EXITFN(isOld);
}

PRIVATE PeerData *findPeer(IPEndPoint *ep)
{
   LOG_ENTERFN();

   PeerData *peerData = NULL;

   for (U32 i = 0; i < g_peerData.size(); i++)
   {
      peerData = g_peerData[i];
      if ((peerData->peerEp.ipAddr.u.ipv4Addr.addr == \
         ep->ipAddr.u.ipv4Addr.addr) &&(peerData->peerEp.port == ep->port))
      {
         break;
      }
   }

   LOG_EXITFN(peerData);
}

VOID updatePeerSeqNumber(IPEndPoint *ep, GtpSeqNumber_t seqNumber)
{
   LOG_ENTERFN();

   PeerData *peerData = findPeer(ep);
   if (NULL != peerData && (seqNumber > peerData->seqNumber))
   {
      peerData->seqNumber = seqNumber;
   }

   LOG_EXITVOID();
}

PeerData *addPeerData(IPEndPoint ep)
{
   LOG_ENTERFN();

   PeerData *peerData = findPeer(&ep);;
   if (NULL != peerData)
   {
      LOG_EXITFN(peerData);
   }

   peerData = new PeerData;
   peerData->peerEp = ep;
   peerData->seqNumber = 0;
   g_peerData.push_back(peerData);

   return peerData;
}

/**
 * @brief Generates a new sequence number for the sending a request
 *    to the peer
 *
 * @param peer
 *
 * @return 
 */
PUBLIC GtpSeqNumber_t generateSeqNum(IPEndPoint *ep, GtpMsgCategory_t cat)
{
   LOG_ENTERFN();

   PeerData *peer = findPeer(ep);
   GtpSeqNumber_t seqNumber = ++(peer->seqNumber);
   if (GTP_MSG_CAT_CMD == cat)
      GTP_SET_SEQN_MSB(seqNumber);

   LOG_EXITFN(seqNumber);
}


