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
typedef ProcSequence::iterator   ProcSeqItr;

typedef enum
{
   JOB_TYPE_INV,
   JOB_TYPE_SEND,
   JOB_TYPE_RECV,
   JOB_TYPE_WAIT,
   JOB_TYPE_MAX
} JobType_t;

class Job
{
   public:
      Job();
      ~Job();
      Job(GtpMsg*, JobType_t);

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
   public:
      ~Procedure();
      VOID addJob(Job *job);
      const JobSequence *getJobSequence();

   private:
      JobSequence      m_jobSeq;
};

#endif /* _SCENARIO_MSG_HPP_ */
