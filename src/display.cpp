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

#include <curses.h>
#include <signal.h>
#include <iostream>
#include <exception>
#include <list>
#include <vector>

#include "types.hpp"
#include "error.hpp"
#include "macros.hpp"
#include "logger.hpp"
#include "timer.hpp"
#include "task.hpp"
#include "gtp_types.hpp"
#include "sim_cfg.hpp"
#include "gtp_util.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "keyboard.hpp"
#include "message.hpp"
#include "scenario.hpp"
#include "gtp_stats.hpp"
#include "display.hpp"

#define COUT            std::cout
#define CIN             std::cin
#define ENDL            std::endl
#define CLEAR_SCREEN()  printf("\033[2J")
#define ENDLINE         "\r\n"
#define PRINT_SEPERATOR() \
{\
   fprintf(stdout, "+------------------------+----------------------------+-----------------------+\r\n");\
}

#define PRINT_END_SEPERATOR_PAUSE() \
{\
   fprintf(stdout, "+--Adjust-Rate [+|-|*|/]--+-----Pause-Traffic [p]-----+-------Quit [q]--------+\r\n");\
}

#define PRINT_END_SEPERATOR_RESUME() \
{\
   fprintf(stdout, "+---Adjust-Rate [+/-]----+----Resume-Traffic [p]------+-------Quit [q]--------+\r\n");\
}

#define PRINT_BLANK_LINE() \
{\
   fprintf(stdout, "\r\n");\
}

class Display *Display::m_pDisp = NULL;
PRIVATE VOID screen_exit();

Display* Display::getInstance()
{
   try
   {
      if (NULL == m_pDisp)
      {
         m_pDisp = new Display;
      }
   }
   catch (std::exception &e)
   {
      LOG_FATAL("Memory allocation failure, Display");
      throw ERR_DISPLAY_INIT;
   }

   return m_pDisp;
}

Display::~Display()
{
   screen_exit();
}

PRIVATE VOID screen_exit()
{
   /* Some signals may be delivered twice during exit() execution,
    * and we must prevent all this from beeing done twice */
   static BOOL alreadyExited = FALSE;
   if (alreadyExited)
   {
      return;
   }

   alreadyExited = TRUE;
   endwin();
}

VOID Display::init()
{
   struct sigaction action_quit;

   initscr();
   noecho();

   m_dispIntvl = Config::getInstance()->getDisplayRefreshTimer();
   m_pStats = Stats::getInstance();
   getTimeStr(m_timeStr);
   m_startTime = getMilliSeconds() / 1000;
   m_remPort = Config::getInstance()->getRemoteGtpcPort(); 
   STRCPY(m_remIpAddrStr, (Config::getInstance()->\
         getRemIpAddrStr()).c_str());

   m_pMsgVec = &(Scenario::getInstance()->m_msgVec);
   m_msgVecLen = m_pMsgVec->size();

   /* Map exit handlers to curses reset procedure */
   memset(&action_quit, 0, sizeof(action_quit));
   (*(void **)(&(action_quit.sa_handler))) = (VOID *)screen_exit;
   sigaction(SIGTERM, &action_quit, NULL);
   sigaction(SIGINT, &action_quit, NULL);
   sigaction(SIGKILL, &action_quit, NULL);  

   CLEAR_SCREEN();
}

RETVAL Display::run(VOID *arg)
{
   LOG_ENTERFN();

   m_lastRunTime = getMilliSeconds();
   disp();
   pause();

   LOG_EXITFN(ROK);
}

VOID Display::disp()
{
   static BOOL firTime = TRUE;

   CLEAR_SCREEN();

   if (firTime)
   {
      firTime = FALSE;
      fprintf(stdout,
            "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"\
            "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
   }

   PRINT_SEPERATOR();
   fprintf(stdout, "Start: %s  ", m_timeStr);
   Time_t runTime = (getMilliSeconds() / 1000) - m_startTime;
   fprintf(stdout, "Run-Time: %us   ", (U32)runTime);
   fprintf(stdout, "Remote-Host: %s:%d \r\n", m_remIpAddrStr, m_remPort);

   Counter ssnCreated = getStats(GSIM_STAT_NUM_SESSIONS_CREATED);
   Counter ssnSucc = getStats(GSIM_STAT_NUM_SESSIONS_SUCC);
   Counter ssnFail = getStats(GSIM_STAT_NUM_SESSIONS_FAIL);
   fprintf(stdout, "Total-Sessions: %u   \r\n", ssnCreated);
   fprintf(stdout, "Session-Completed: %u   \r\n", ssnSucc);
   fprintf(stdout, "Session-Aborted: %u   \r\n", ssnFail);

   PRINT_SEPERATOR();
   fprintf(stdout,"                                 "\
           "Messages  Retrans   Timeout   Unexpected-Msg\r\n");

   for (U32 i = 0; i < m_msgVecLen; i++)
   {
      MsgTask *pMsgTask = (*m_pMsgVec)[i];
      fprintf(stdout, "%s  ", pMsgTask->m_msgName);

      if (pMsgTask->type() == MSG_TASK_SEND)
      {
         fprintf(stdout, "\t--->");
         fprintf(stdout, " \t%9d", pMsgTask->m_numSnd);
         fprintf(stdout, "%9d", pMsgTask->m_numSndRetrans);
         fprintf(stdout, " %9d", pMsgTask->m_numTimeOut);
         fprintf(stdout, ENDLINE);
      }
      else if (pMsgTask->type() == MSG_TASK_RECV)
      {
         fprintf(stdout, " \t<---");
         fprintf(stdout, "\t%9d", pMsgTask->m_numRcv);
         fprintf(stdout, "%9d", pMsgTask->m_numRcvRetrans);
         fprintf(stdout, "                  %9d", pMsgTask->m_numUnexp);
         fprintf(stdout, ENDLINE);
      }
      else
      {
         fprintf(stdout, "[Wait %5d]\r\n", 0);
         fprintf(stdout, ENDLINE);
      }
   }

   PRINT_BLANK_LINE();
   if (KB_KEY_PAUSE_TRAFFIC == Keyboard::key)
   {
      PRINT_END_SEPERATOR_RESUME();
   }
   else
   {
      PRINT_END_SEPERATOR_PAUSE();
   }

   fflush(stdout);
}

Counter Display::getStats(GtpStat_t type)
{
   return m_pStats->getStats(type);
}

VOID Display::displayStats()
{
   Display::getInstance()->disp();
}
