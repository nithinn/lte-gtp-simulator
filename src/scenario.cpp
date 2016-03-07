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

#include "pugixml.hpp"
using namespace pugi;

#include <vector>
#include <list>

#include "types.hpp"
#include "error.hpp"
#include "macros.hpp"
#include "logger.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "message.hpp"
#include "task.hpp"
#include "sim_cfg.hpp"
#include "scenario.hpp"

class Scenario* Scenario::m_pMainScn = NULL;  
EXTERN VOID parseXmlScenario(const S8*, JobSequence*) throw (ErrCodeEn);

Scenario* Scenario::getInstance()
{
   try
   {
      if (NULL == m_pMainScn)
      {
         m_pMainScn = new Scenario;
      }
   }
   catch (std::exception &e)
   {
      LOG_FATAL("Memory allocation failure, Scenario");
      throw ERR_MEMORY_ALLOC;
   }

   return m_pMainScn;
}

Scenario::Scenario()
{
   m_scnRunIntvl = Config::getInstance()->getScnRunInterval();
   m_ifType = (GtpIfType_t)Config::getInstance()->getIfType();
}

Scenario::~Scenario()
{
   for (JobSeqItr itr = m_jobSeq.begin(); itr != m_jobSeq.end(); itr++)
   {
      delete *itr;
   }
}


/**
 * @brief
 *    Constructor
 *    Creates the complete scenario by reading the xml scenario file
 *
 * @param pScnFile
 *    Name of xml file containing the scenario
 *    
 */
VOID Scenario::init(const S8 *pScnFile) throw (ErrCodeEn)
{
   try
   {
      parseXmlScenario(pScnFile, &m_jobSeq);  
   }
   catch (ErrCodeEn &e)
   {
      LOG_FATAL("Xml Parsing failed");
      throw e;
   }

   Job *firstJob = m_jobSeq[0];
   if (firstJob->type() == JOB_TYPE_SEND)
   {
      m_scnType = SCN_TYPE_INITIATING;
   }
   else
   {
      m_scnType = SCN_TYPE_WAITING;
   }

   createProcedure(&m_jobSeq);
}

/**
 * @brief Converts job sequence into set of procedures. A procedure is 
 *    identified by a Request-Response, or Command-Failure, or Command-
 *    Triggered Request-Response.
 *
 *    A wait job can within a procedure, or in between procedure. If a 
 *    wait job is at the beginning of the scennario or in between two
 *    procedures it is considered as a different scenario. Otherwise
 *    the wait job is considered part of the gtpc procedure.
 *
 * @param jobSeq
 */
VOID Scenario::createProcedure(JobSequence *jobSeq)
{
   LOG_ENTERFN();

   Procedure         *proc        = NULL;
   BOOL              fullProc = TRUE;
   GtpMsgCategory_t  initMsg      = GTP_MSG_CAT_INV;

   for (JobSeqItr itr = jobSeq->begin(); itr != jobSeq->end(); itr++) 
   {
      Job *job = *itr;

      if (TRUE == fullProc)
      {
         /* wait job between two procedures, or may be the first procedure */
         if (job->type() == JOB_TYPE_WAIT)
         {
            proc = new Procedure;
            proc->addJob(job);
            m_procSeq.push_back(proc);
         }
         else
         {
            fullProc = FALSE;
            proc = new Procedure;
            m_procSeq.push_back(proc);

            /* beginning of the procedure */
            proc->addJob(job);

            /* based on the first messagae find whether the next messages
             * have a complete procedure or not
             */
            GtpMsg *gtpMsg = job->getGtpMsg();
            initMsg = gtpGetMsgCategory(gtpMsg->type());
         }
      }
      else
      {
         /* a wait is within a procedure, just append it into the procedure
          * job sequence
          */
         if (job->type() == JOB_TYPE_WAIT)
         {
            proc->addJob(job);
         }
         else
         {
            GtpMsg *gtpMsg = job->getGtpMsg();
            GtpMsgCategory_t msgCat = gtpGetMsgCategory(gtpMsg->type());

            if (initMsg == GTP_MSG_CAT_REQ && GTP_MSG_CAT_RSP == msgCat)
            {
               fullProc = TRUE;
               proc->addJob(job);
            }
            else if (initMsg == GTP_MSG_CAT_CMD && GTP_MSG_CAT_RSP == msgCat)
            {
               fullProc = TRUE;
               proc->addJob(job);
            }
            else if (initMsg == GTP_MSG_CAT_TRIG_REQ && \
                  GTP_MSG_CAT_RSP == msgCat)
            {
               fullProc = TRUE;
               proc->addJob(job);
            }
            else if (initMsg == GTP_MSG_CAT_CMD && \
                  GTP_MSG_CAT_TRIG_REQ == msgCat)
            {
               /* a command message is matched with a triggered request message
                * so wait for the response message for the triggered request
                * message
                */
               initMsg = GTP_MSG_CAT_TRIG_REQ;
            }
         }
      }
   }

   LOG_EXITVOID();
}

/**
 * @brief
 *    Executes the scenario
 */
BOOL Scenario::run()
{
   return true;
}

ScenarioType_t Scenario::getScnType()
{
   return m_scnType;
}

VOID Scenario::shutdown()
{
   LOG_ENTERFN();

   delete this;

   LOG_EXITVOID();
}

GtpIfType_t Scenario::ifType()
{
   return m_ifType;
}
