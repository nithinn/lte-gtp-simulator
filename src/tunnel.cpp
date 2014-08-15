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

#include <list>
#include <map>

#include "types.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "gtp_types.hpp"
#include "sim_cfg.hpp"
#include "tunnel.hpp"

static TunMap        s_gtpcTunMap;
static U32           s_cTeid = 0;
static U32           s_uTeid = 0;

PRIVATE U32          generateUTeid();
PRIVATE U32          generateCTeid();

PRIVATE U32 generateCTeid()
{
   return ++s_cTeid;
}

PRIVATE U32 generateUTeid()
{
   return ++s_uTeid;
}

PUBLIC VOID deleteCTun(GtpcTun *pTun)
{
   LOG_ENTERFN();

   pTun->m_refCount--;
   if (pTun->m_refCount == 0)
   {
      LOG_DEBUG("Deleting GTP-C Tunnel, TEID [%d]", pTun->m_locTeid);
      s_gtpcTunMap.erase(pTun->m_locTeid);
      delete pTun;
   }

   LOG_EXITVOID();
}

GtpcTun::GtpcTun()
{
   m_locTeid = generateCTeid();
   m_remTeid = 0;
   m_refCount = 1;
   m_localEp.port = Config::getInstance()->getLocalGtpcPort();
   m_localEp.ipAddr = *(Config::getInstance()->getLocalIpAddr());

   s_gtpcTunMap.insert(TunMapPair(m_locTeid, this));
   LOG_DEBUG("Creating GTP-C Tunnel, TEID [%d]", m_locTeid);
}

PUBLIC GtpcTun* findCTun(GtpTeid_t teid)
{
   GtpcTun     *pTun = NULL;

   TunMapItr itr = s_gtpcTunMap.find(teid);
   if (itr != s_gtpcTunMap.end())
   {
      pTun = itr->second;
      LOG_TRACE("Found GTP-C Tunnel, TEID [%d]", teid);
   }
  
   LOG_EXITFN(pTun);
}

GtpuTun::GtpuTun()
{
   m_locTeid = generateUTeid();
   m_remTeid = 0;
   LOG_TRACE("GTP-U Tunnel Constructor, TEID [%d]", m_locTeid);
}


