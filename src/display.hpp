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

#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

class Display: virtual public Task
{
   public:
      VOID init();
      static Display* getInstance();
      ~Display();
      RETVAL run(VOID *arg = NULL);  
      inline Time_t wake() { return m_lastRunTime + m_dispIntvl; }
      VOID createMsgDirLst();

      static void displayStats();
   private:
      static class Display  *m_pDisp;

      VOID              disp();
      Counter           getStats(GtpStat_t type);

      Time_t            m_lastRunTime;
      Time_t            m_dispIntvl;
      Time_t            m_startTime;
      U16               m_remPort;
      S8                m_remIpAddrStr[IPV6_ADDR_MAX_LEN];
      U16               m_localPort;
      S8                m_localIpAddrStr[IPV6_ADDR_MAX_LEN];
      Stats             *m_pStats;
      S8                m_timeStr[GSIM_TIME_STR_MAX_LEN];
      ProcSequence      *m_procSeq;
      VOID              printJob(Job*);
      std::string       m_ifTypeStr;
};

#endif
