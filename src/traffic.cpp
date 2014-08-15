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
#include "sim_cfg.hpp"
#include "task.hpp"
#include "gtp_types.hpp"
#include "gtp_macro.hpp"
#include "gtp_if.hpp"
#include "gtp_util.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "message.hpp"
#include "gtp_stats.hpp"
#include "tunnel.hpp"
#include "session.hpp"
#include "traffic.hpp"

TrafficTask::TrafficTask()
{
   m_ratePeriod = Config::getInstance()->getSessionRatePeriod();
   m_rate = Config::getInstance()->getCallRate();
   m_maxSessions = Config::getInstance()->getNumSessions();

   string imsi = Config::getInstance()->getImsi();
   m_imsiGen.init(imsi);
}

BOOL TrafficTask::run()
{
   LOG_ENTERFN();

   BOOL     abortTraffiTask = FALSE;
   LOG_DEBUG("Running TrafficTask, Session Rate [%d]", m_rate);

   m_lastRunTime = getMilliSeconds();
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

   if (abortTraffiTask)
   {
      stop();
   }
   else
   {
      pauseTask();
   }

   LOG_EXITFN(TRUE);
}

Time_t TrafficTask::wake()
{
   return m_lastRunTime + m_ratePeriod; 
}

PUBLIC VOID procGtpcMsg(UdpData *pUdpData)
{
   LOG_ENTERFN();

   GtpTeid_t      teid        = 0;
   UeSession      *pUeSession = NULL;
   U8             *pGtpBuf    = NULL;
   GtpMsgType_E   msgType     = GTPC_MSG_TYPE_INVALID;
   
   pGtpBuf = pUdpData->buf.pVal;
   GTP_MSG_GET_TYPE(pGtpBuf, msgType);

   if (GTPC_MSG_CS_REQ == msgType || GTPC_MSG_FR_REQ == msgType)
   {
      U8 *pImsiBuf = getImsiBufPtr(&pUdpData->buf);

      GtpImsiKey  imsiKey;
      GTP_GET_IE_LEN(pImsiBuf, imsiKey.len);
      MEMCPY(imsiKey.val, pImsiBuf + GTP_IE_HDR_LEN, imsiKey.len);

      pUeSession = UeSession::getUeSession(imsiKey);
      if (NULL == pUeSession)
      {
         pUeSession = UeSession::createUeSession(imsiKey);
      }
      else
      {
         LOG_ERROR("Unhandled Incoming GTP Message");
      }
   }
   else
   {
      GTP_MSG_DEC_TEID(pGtpBuf, teid);
      if (0 != teid)
      {
         pUeSession = UeSession::getUeSession(teid);
      }
      else
      {
         LOG_ERROR("Unhandled Incoming GTP Message");
      }
   }

   if (NULL != pUeSession)
   {
      pUeSession->storeRcvdMsg(&pUdpData->buf, pUdpData->connId,\
            pUdpData->peerEp);
      pUeSession->run();
   }
   else
   {
      delete []pUdpData->buf.pVal;
   }

   delete pUdpData;

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

