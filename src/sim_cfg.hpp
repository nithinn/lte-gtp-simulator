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

#ifndef _GTP_CFG_HPP_
#define _GTP_CFG_HPP_

#include <iostream>
#include <string>

using namespace std;

#define DFLT_GTPC_PORT           2123
#define DFLT_GTPC_SEND_PORT      2124
#define DFLT_T3_TIMER            3000    // milli seconds
#define DFLT_N3_REQUESTS         3
#define DFLT_DISP_REFRESH_TIMER  3000    // milli seconds
#define DFLT_SESSION_RATE_PERIOD 1000    // 1000 milli seconds 
#define DFLT_SESSION_RATE        1       // 1 session per rate period
#define GSIM_TRACE_MSG_FILE_NAME_LEN   64

typedef enum
{
    OPT_INV,
    OPT_NODE,
    OPT_NUM_CALLS,
    OPT_CALL_RATE,
    OPT_LOC_IP_ADDR,
    OPT_REM_IP_ADDR,
    OPT_LOC_PORT,
    OPT_REM_PORT,
    OPT_T3_TIMER,
    OPT_N3_REQ,
    OPT_DISP_TIMER,
    OPT_DISP_OPT,
    OPT_ERR_FILE,
    OPT_LOG_FILE,
    OPT_SCN_FILE,
    OPT_LOG_LEVEL,
    OPT_TRACE_MSG,
    OPT_MAX
} CmdLineOptEn;

typedef struct
{
    CmdLineOptEn    type;
    string          option;
    string          value;   

} CmdLineParam;

typedef enum
{
    DISP_TARGET_NONE,
    DISP_TARGET_SCREEN,
    DISP_TARGET_FILE,
    DISP_TARGET_MAX
} DisplayTargetEn;

// Config will be a singleton object, accessed using getInstance
class Config
{
   public:
      static Config* getInstance();
      ~Config();

      // service routines 
      // these routines are made public, for setting configuration 
      // property from a GUI
      VOID setNoOfCalls(U32 n);
      VOID setLocalIpAddr(string ip) throw (ErrCodeEn);
      VOID setRemoteIpAddr(string ip) throw (ErrCodeEn);
      VOID setLocalGtpcPort(U16 port);
      VOID setLocalGtpcSendPort(U16 port);
      VOID setRemoteGtpcPort(U16 port);
      VOID setT3TimerSeconds(U32 val);
      VOID setN3Requests(U32 n);
      VOID setDisplayRefreshTimer(U32 n);
      VOID setDisplayTarget(DisplayTargetEn target);
      VOID setErrorFile(string filename) throw (ErrCodeEn);
      VOID setScenarioFile(string filename) throw (ErrCodeEn);
      VOID setLogFile(string filename) throw (ErrCodeEn);
      VOID setDisplayTargetFile(string filename) throw (ErrCodeEn);
      VOID setCallRate(U32 n);
      VOID setLogLevel(string logLvl);
      VOID setTraceMsg(BOOL);
      VOID setTraceMsgFile(string);

      const IpAddr* getRemoteIpAddr();
      string getRemIpAddrStr();
      const IpAddr* getLocalIpAddr();
      U32 getN3Requests();
      U16 getRemoteGtpcPort();
      U16 getLocalGtpcPort();
      U16 getLocalGtpcSendPort();
      U32 getDisplayRefreshTimer();
      U32 getT3Timer();
      U32 getScnRunInterval();
      const S8* getScnFile();
      U32 getCallRate();
      U32 getLogLevel();
      VOID setConfig (S32 numArgs, S8** cmdLineArgs) throw (ErrCodeEn);
      Time_t getSessionRatePeriod();
      U8 getNodeType();
      U8 getIfType();
      Counter getNumSessions();
      string getLogFile();
      BOOL   getTraceMsg();
      string getTraceMsgFile();
      string getImsi();
      VOID setImsi(S8 *pVal, U32 len);

   private:
      Config();
      RETVAL saveIp(string &ipStr, IpAddr *pIp);
      VOID setParameter(CmdLineParam *pParam) throw (ErrCodeEn);
      CmdLineOptEn getOptType(const string &opt);
      VOID getArgDetails(string &argStr, CmdLineParam *arg) throw(ErrCodeEn);

      U32                  m_ssnRate;     // no.of calls per sec
      IpAddr               locIpAddr;
      IpAddr               remIpAddr;
      U16                  locGtpcPort;
      U16                  locGtpcSndPort;
      U16                  remGtpcPort;
      U32                  t3Timer;
      U32                  n3Req;
      U32                  dispTimer;      // display refresh rate
      DisplayTargetEn      dispTarget;     // displa on screen or file        
      string               errFile;        // error log file
      string               scnFile;        // xml scenario file path
      string               m_logFile;      // log file path             
      string               dispTargetFile; // display redirected to this file
      U32                  m_scnRunIntvl;
      Time_t               m_ssnRatePeriod;
      U32                  m_logLevel;
      U8                   m_nodeType;
      U8                   m_ifType;
      Counter              m_maxSessions;
      string               m_remIpAddrStr;
      BOOL                 m_traceMsg;
      string               m_traceMsgFile;
      string               m_imsiStr;
};

#endif
