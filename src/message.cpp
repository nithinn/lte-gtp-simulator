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
#include "message.hpp"

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

VOID Procedure::addJob(GtpMsg *msg, JobType_t jobType)
{
   Job *job = new Job(msg, jobType);
   jobSeq.push_back(job);
}
