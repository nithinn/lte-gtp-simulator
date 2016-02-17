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

#ifndef __GTP_PEER__
#define __GTP_PEER_

typedef struct
{
   IPEndPoint        peerEp;
   GtpSeqNumber_t    seqNumber;
} PeerData;

typedef vector<PeerData*> PeerDataVec;

BOOL validateSeqNumber(PeerData *peerData, Buffer *gtpMsg);
PeerData *addPeerData(IPEndPoint ep);
VOID updatePeerSeqNumber(IPEndPoint *ep, GtpSeqNumber_t seqNumber);
PUBLIC GtpSeqNumber_t generateSeqNum(IPEndPoint *peer, GtpMsgCategory_t cat);
#endif
