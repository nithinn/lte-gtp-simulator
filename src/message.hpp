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

class MsgTask;
typedef std::vector<MsgTask* >   MsgVec;
typedef MsgVec::iterator         MsgVecItr;

typedef enum
{
   MSG_TASK_INV,
   MSG_TASK_SEND,
   MSG_TASK_RECV,
   MSG_TASK_WAIT,
   MSG_TASK_MAX
} MsgTaskType_t;

class MsgTask
{
   public:
      GtpMsg*        getGtpMsg();
      MsgTaskType_t  type();
      Time_t         wait();
      MsgTask();
      MsgTask(GtpMsg*, MsgTaskType_t);
      ~MsgTask();

      Counter        m_numSnd;
      Counter        m_numRcv;
      Counter        m_numSndRetrans;
      Counter        m_numRcvRetrans;
      Counter        m_numTimeOut;
      Counter        m_numUnexp;
      S8             m_msgName[GTP_MSG_NAME_LEN];

   private:
      GtpMsg         *m_pGtpMsg;
      MsgTaskType_t  m_type;
      Time_t         m_wait;
};

#endif /* _SCENARIO_MSG_HPP_ */
