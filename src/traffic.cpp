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
#include <vector>

#include "types.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "timer.hpp"
#include "task.hpp"
#include "gtp_types.hpp"
#include "sim_cfg.hpp"
#include "gtp_macro.hpp"
#include "gtp_if.hpp"
#include "gtp_util.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "message.hpp"
#include "gtp_stats.hpp"
#include "tunnel.hpp"
#include "session.hpp"
#include "gtp_peer.hpp"
#include "display.hpp"
#include "traffic.hpp"

EXTERN BOOL g_serverMode;

TrafficTask::TrafficTask()
{
   m_ratePeriod = Config::getInstance()->getSessionRatePeriod();
   m_rate = Config::getInstance()->getCallRate();
   m_maxSessions = Config::getInstance()->getNumSessions();
   string imsi = Config::getInstance()->getImsi();
   m_imsiGen.init(imsi);
}

RETVAL TrafficTask::run(VOID *arg)
{
   LOG_ENTERFN();

   BOOL     abortTraffiTask = FALSE;
   LOG_DEBUG("Running TrafficTask, Session Rate [%d]", m_rate);

   Time_t currTime = getMilliSeconds();
   m_lastRunTime = currTime;
   Counter numSession = Stats::getStats(GSIM_STAT_NUM_SESSIONS_CREATED);
   for (U32 i = 0; i < m_rate; i++)
   {
      GtpImsiKey imsiKey;
      MEMSET(&imsiKey, 0, sizeof(GtpImsiKey));
      m_imsiGen.allocNew(&imsiKey);

      UeSession::createUeSession(imsiKey);
      numSession++;
      if ((0 != m_maxSessions) && (numSession >= m_maxSessions))
      {
         LOG_DEBUG("Max Sessions = [%d] Created, Stopping Traffic",\
               m_maxSessions);
         abortTraffiTask = TRUE;
         break;
      }
   }

   Display::displayStats();

   if (abortTraffiTask)
   {
      stop();
   }
   else
   {
      pause();
   }

   LOG_EXITFN(ROK);
}

PUBLIC VOID procGtpcMsg(UdpData_t *data)
{
   LOG_ENTERFN();

   UeSession      *ueSsn     = NULL;
   U8             *gtpMsgBuf = NULL;
   GtpMsgType_t   msgType    = GTPC_MSG_TYPE_INVALID;
   
   gtpMsgBuf = data->buf.pVal;
   GTP_MSG_GET_TYPE(gtpMsgBuf, msgType);

   if (GTPC_MSG_CS_REQ == msgType || GTPC_MSG_FR_REQ == msgType)
   {
      U8 *imsiBuf = getImsiBufPtr(&data->buf);

      GtpImsiKey  imsiKey;
      GTP_GET_IE_LEN(imsiBuf, imsiKey.len);
      MEMCPY(imsiKey.val, imsiBuf + GTP_IE_HDR_LEN, imsiKey.len);

      ueSsn = UeSession::getUeSession(imsiKey);
      if (NULL == ueSsn)
      {
         addPeerData(data->peerEp); 
         ueSsn = UeSession::createUeSession(imsiKey);
      }
   }
   else
   {
      GtpTeid_t teid = 0;
      GTP_MSG_DEC_TEID(gtpMsgBuf, teid);
      if (0 != teid)
      {
         ueSsn = UeSession::getUeSession(teid);
         if (NULL == ueSsn)
         {
            LOG_ERROR("GTPC Message received with unknown TEID [%d]", teid);
            delete data;
         }
      }
      else
      {
         LOG_ERROR("Unhandled Incoming GTP Message");
         delete data;
      }
   }

   if (NULL != ueSsn)
   {
      ueSsn->run(data);
   }

   LOG_EXITVOID();
}

GtpImsiGenerator::GtpImsiGenerator()
{
   MEMSET((VOID *)m_imsiStr, 0, GTP_IMSI_MAX_DIGITS);
}

VOID GtpImsiGenerator::init(string imsi)
{
   m_len = imsi.size();
   if (m_len <= GTP_IMSI_MAX_DIGITS)
   {
      MEMCPY(m_imsiStr, imsi.c_str(), m_len);
   }
   else
   {
      LOG_ERROR("Invalid IMSI string length [%d]", m_len);
   }
}

/**
 * @brief
 *    allocates a new imsi incremented by one
 *
 * @return 
 *    imsi
 */
VOID GtpImsiGenerator::allocNew(GtpImsiKey *pImsi)
{
   LOG_ENTERFN();

   numericStrIncriment(m_imsiStr, m_len); 
   pImsi->len = encodeImsi(m_imsiStr, m_len, pImsi->val);
   
   LOG_EXITVOID();
}

