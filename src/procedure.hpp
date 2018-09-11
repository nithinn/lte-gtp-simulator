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

#ifndef _SCENARIO_MSG_HPP_
#define _SCENARIO_MSG_HPP_

class Job;
class Procedure;

typedef std::vector<Job*>        JobSequence;
typedef JobSequence::iterator    JobSeqItr;
typedef std::vector<Procedure*>  ProcSequence;
typedef ProcSequence::iterator   ProcedureItr;

typedef enum
{
   JOB_TYPE_INV,
   JOB_TYPE_SEND,
   JOB_TYPE_RECV,
   JOB_TYPE_WAIT,
   JOB_TYPE_MAX
} JobType_t;

typedef enum
{
   PROC_TYPE_INV,
   PROC_TYPE_WAIT,
   PROC_TYPE_REQ_RSP,
   PROC_TYPE_REQ_TRIG_REP
} ProcedureType_t;

class Job
{
   public:
      Job();
      ~Job();
      Job(GtpMsg*, JobType_t);
      Job(Time_t wait);

      GtpMsg*        getGtpMsg();
      inline JobType_t type() { return m_type; }
      inline Time_t wait() { return m_wait; }

      Counter        m_numSnd;
      Counter        m_numRcv;
      Counter        m_numSndRetrans;
      Counter        m_numRcvRetrans;
      Counter        m_numTimeOut;
      Counter        m_numUnexp;
      S8             m_msgName[GTP_MSG_NAME_LEN];

   private:
      GtpMsg         *m_pGtpMsg;
      JobType_t      m_type;
      Time_t         m_wait;
};

class Procedure
{
   private:
      ProcedureType_t   m_type;

   public:
      Procedure()
      {
         m_type      = PROC_TYPE_INV;
         m_initial   = NULL;
         m_trigMsg   = NULL;
         m_trigReply = NULL;
         m_wait      = NULL;
      }

      ~Procedure()
      {
         if (NULL != m_initial)
         {
            delete m_initial;
         }

         if (NULL != m_trigMsg)
         {
            delete m_trigMsg;
         }

         if (NULL != m_trigReply)
         {
            delete m_trigReply;
         }

         if (NULL != m_wait)
         {
            delete m_wait;
         }
      }

      ProcedureType_t type() {return m_type;}

      BOOL addJob(Job *job);

      Job            *m_initial; /* initial request / command message */

      Job            *m_trigMsg; /* triggered message is sent in response to
                                  * an initial message. This can be a request
                                  * message if the initial message is a command
                                  * message
                                  */

      Job            *m_trigReply; /* Triggered reply message is sent in
                                    * response to a triggered message
                                    */
      
      Job            *m_wait;    /* Only one wait allowd in a procedure
                                  * before sending a triggered request 
                                  * message/response for an initial message
                                  * at a client side
                                  * OR
                                  * before sending a response for triggered
                                  * request at a server side
                                  */
};

#endif /* _SCENARIO_MSG_HPP_ */
