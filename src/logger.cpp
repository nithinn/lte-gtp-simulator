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

#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <string>
#include <list>

#include "types.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "gtp_types.hpp"
#include "sim_cfg.hpp"

LogLevel_t  Logger::m_logLevel = LOG_LVL_ERROR;
FILE*       Logger::m_logFile = NULL;
FILE*       Logger::m_traceMsgFile = NULL;
BOOL        Logger::m_traceMsgEnabled = FALSE;

U8 g_logLvlStr[LOG_LVL_END][LOG_LVL_STR_MAX] = \
{
   "INVALID",
   "FATAL",
   "WARN",
   "ERROR",
   "INFO",
   "DEBUG",
   "TRACE"
};

/**
 * @brief
 *    Initial Logger class
 *
 * @param level
 */
VOID Logger::init(U32 level)
{
   m_logLevel = (LogLevel_t)level;

   std::string logFile = Config::getInstance()->getLogFile();
   if (!logFile.empty())
   {
      m_logFile = fopen(logFile.c_str(), "w+");
   }
   else
   {
      m_logFile = stdout;
   }

   m_traceMsgEnabled = Config::getInstance()->getTraceMsg();
   if (m_traceMsgEnabled)
   {
      std::string traceMsgFile = Config::getInstance()->getTraceMsgFile();
      if (!traceMsgFile.empty())
      {
         m_traceMsgFile = fopen(traceMsgFile.c_str(), "w+");
      }
      else
      {
         m_traceMsgFile = stdout;
      }
   }
}

/**
 * @brief
 *    prints debug logs for gtp simulator
 */
VOID Logger::log
(
LogLevel_t        logLvl,
const S8          *fileName,
const U32         lineNum,
const S8          *format, ...
)
{
   S8          logBuf[LOG_BUF_MAX] = {'\0'};
   va_list     args;

   va_start(args, format);
   vsprintf(logBuf, format, args);

   fprintf(m_logFile, "[%s] ", g_logLvlStr[logLvl]);
   fprintf(m_logFile, "%s:%d:", fileName, lineNum);
   fprintf(m_logFile, "%s\n", logBuf);
   fflush(m_logFile);
    
   va_end(args);
}

/**
 * @brief
 *    Logs Hexdump of GTP messages sent/received
 *
 * @param pMsgName
 * @param pMsgBuf
 * @param msgLen
 * @param dir
 * @param pIpAddr
 * @param port
 */
VOID Logger::log
(
const U8             *pMsgName,\
const U8             *pMsgBuf,\
U32                  msgLen,\
MsgAction_t          dir,\
const U8             *pIpAddr,\
U16                  port
)
{
   if (MSG_ACTION_RECV == dir)
   {
      fprintf(m_traceMsgFile, "Received [%s:%d]\n", pIpAddr, port);
   }
   else
   {
      fprintf(m_traceMsgFile, "Sent     [%s:%d]\n", pIpAddr, port);
   }

   for (U32 i = 0; i < msgLen; i++)
   {
      if (i % 4 == 0)
      {
         fprintf(m_traceMsgFile, "\n");
      }

      fprintf(m_traceMsgFile, "%x", pMsgBuf[i]);
   }
}
