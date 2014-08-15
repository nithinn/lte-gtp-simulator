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

#ifndef __TUNNEL_HPP__
#define __TUNNEL_HPP__

class GtpcPdn;
class UeSession;

class GtpcTun
{
   public:
      GtpcTun();

      GtpTeid_t   m_locTeid;
      GtpTeid_t   m_remTeid;

      IPEndPoint  m_localEp;
      IPEndPoint  m_peerEp;

      UeSession   *m_pUeSession;
      GtpcPdn     *m_pPdn;

      U32         m_refCount; /* On S11 and S4 interface the multiple PDNs
                               * will point to same GtpcTun object, while
                               * while assigning the cTun to a particular PDN
                               * and when deleting check the refcount
                               */
};

class GtpuTun
{
   private:
      GtpTeid_t   m_locTeid;
      GtpTeid_t   m_remTeid;

   public:
      GtpuTun();
      GtpTeid_t   localTeid() {return m_locTeid;}
      GtpTeid_t   remoteTeid() {return m_remTeid;}
};

typedef std::map<GtpTeid_t, GtpcTun*>  TunMap;
typedef std::pair<GtpTeid_t,GtpcTun*>  TunMapPair;
typedef TunMap::iterator               TunMapItr;

EXTERN VOID       deleteCTun(GtpcTun *pTun);
EXTERN GtpcTun*   findCTun(GtpTeid_t teid);
PUBLIC GtpcTun*   createCTun(GtpcPdn *pPdn);

#endif
