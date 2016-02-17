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

#ifndef __TRAFFIC_TASK__
#define __TRAFFIC_TASK__

class GtpImsiGenerator
{
   public:
      GtpImsiGenerator();
      VOID allocNew(GtpImsiKey*);
      VOID init(string imsi);

   private:
      S8    m_imsiStr[GTP_IMSI_MAX_DIGITS];
      U32   m_len;
};

/* generates the traffic, if the scenario of Initiating type */
class TrafficTask: public Task
{
   public:
      TrafficTask();
      ~TrafficTask() {}
      RETVAL run();  

      inline Time_t wake()
      {
         return m_lastRunTime + m_ratePeriod;
      }

   private:
      U32               m_rate;
      Time_t            m_ratePeriod;   
      Time_t            m_lastRunTime;
      Counter           m_maxSessions;
      GtpImsiGenerator  m_imsiGen;
};

/* task for sending periodic echo request messages to the peer */
class GtpcEcho: public Task
{
   public:
      RETVAL run();
      Time_t  wake();

   private:
      U32      m_rate;
      Time_t   m_ratePeriod;   
};

PUBLIC VOID procGtpcMsg(UdpData *pData);
#endif
