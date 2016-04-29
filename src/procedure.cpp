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
#include <list>

#include "types.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "procedure.hpp"

Job::Job()
{
   m_type    = JOB_TYPE_INV;
   m_pGtpMsg = NULL;
}

Job::Job(GtpMsg *pGtpMsg, JobType_t taskType)
{
   m_type          = taskType;
   m_pGtpMsg       = pGtpMsg;
   m_numSnd        = 0;
   m_numRcv        = 0;
   m_numSndRetrans = 0;
   m_numRcvRetrans = 0;
   m_numTimeOut    = 0;
   m_numUnexp      = 0;

   STRCPY(m_msgName, gtpGetMsgName(pGtpMsg->type()));
}

Job::~Job()
{
   if (m_pGtpMsg)
   {
      delete m_pGtpMsg;
   }
}

/**
 * @brief
 *    Returns the GTP message in the scenario element
 *
 * @return
 *    Pointer to GTP Message
 */
GtpMsg* Job::getGtpMsg()
{
   return m_pGtpMsg;
}

BOOL Procedure::addJob(Job *job)
{
   BOOL fullProc = FALSE;

   LOG_ENTERFN();

   if (JOB_TYPE_WAIT == job->type())
   {
      /* this is a wait in between a procedure */
      m_wait = job;
      m_type = PROC_TYPE_WAIT;
   }
   else
   {
      GtpMsg *gtpMsg = job->getGtpMsg();
      GtpMsgType_t msgType = gtpMsg->type();
      GtpMsgCategory_t msgCat = gtpGetMsgCategory(gtpMsg->type());

      if (GTP_MSG_CAT_CMD == msgCat)
      {
         m_initial = job;
      }
      else if (GTP_MSG_CAT_CMD_FAIL == msgCat)
      {
         m_trigMsg = job;
         fullProc = TRUE;
         m_type = PROC_TYPE_REQ_RSP;
      }
      else if (GTP_MSG_CAT_REQ == msgCat)
      {
         if ((GTPC_MSG_CB_REQ == msgType) || \
            (GTPC_MSG_MB_REQ == msgType) || \
            (GTPC_MSG_DB_REQ == msgType))
         {
            /* the request message is triggered by a command message */
            if (NULL != m_initial)
            {
               m_trigMsg = job;
            }
            else
            {
               m_initial = job;
            }
         }
         else
         {
            m_initial = job;
         }
      }
      else if (GTP_MSG_CAT_RSP == msgCat)
      {
         if ((GTPC_MSG_CB_RSP == msgType) || \
            (GTPC_MSG_MB_RSP == msgType) || \
            (GTPC_MSG_DB_RSP == msgType))
         {
            if (NULL != m_trigMsg)
            {
               m_trigReply = job;
               m_type = PROC_TYPE_REQ_TRIG_REP;
            }
            else
            {
               m_trigMsg = job;
               m_type = PROC_TYPE_REQ_RSP;
            }
         }
         else
         {
            m_trigMsg = job;
            m_type = PROC_TYPE_REQ_RSP;
         }
         
         fullProc = TRUE;
      }
   }

   LOG_EXITFN(fullProc);
}

