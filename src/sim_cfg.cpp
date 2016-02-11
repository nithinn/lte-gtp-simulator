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

#include <arpa/inet.h>
#include <stdlib.h>
#include <list>

#include "types.hpp"
#include "logger.hpp"
#include "error.hpp"
#include "macros.hpp"
#include "gtp_types.hpp"
#include "sim_cfg.hpp"

static Config *pCfg = NULL;
static S8 DFLT_IMSI[] = "112233445566778";

CmdLineParam gCfgOptioTable[OPT_MAX] = 
{
    {OPT_INV,           "", ""},
    {OPT_NODE,          "node", ""},
    {OPT_NUM_CALLS,     "num-sessions", ""},
    {OPT_CALL_RATE,     "session-rate", ""},
    {OPT_RATE_PERIOD,   "rate-period", ""},
    {OPT_LOC_IP_ADDR,   "local-ip", ""},   
    {OPT_REM_IP_ADDR,   "remote-ip", ""},   
    {OPT_LOC_PORT,      "local-port", ""},
    {OPT_REM_PORT,      "remote-port", ""},
    {OPT_T3_TIMER,      "t3-timer", ""},
    {OPT_N3_REQ,        "n3-requests", ""},
    {OPT_DISP_TIMER,    "disp-timer", ""},
    {OPT_DISP_OPT,      "disp-target", ""},
    {OPT_ERR_FILE,      "error-file", ""},
    {OPT_LOG_FILE,      "log-file", ""},
    {OPT_LOG_LEVEL,     "log-level", ""},
    {OPT_TRACE_MSG,     "trace-msg", ""}
};


// returns the Config class singleton instance ptr
Config* Config::getInstance()
{
    if (NULL == pCfg)
    {
        try
        {
            pCfg = new Config;
            return pCfg;
        }
        catch (S32 e)
        {
            return (Config *)NULL;
        }
    }
    else
    {
        return pCfg;
    }
}

/**
 * @brief 
 *    Constructor for Config class
 */
Config::Config()
{
    MEMSET((VOID *)&(locIpAddr), 0, sizeof(IpAddr));
    MEMSET((VOID *)&(remIpAddr), 0, sizeof(IpAddr));
    m_imsiStr.assign(DFLT_IMSI, STRLEN(DFLT_IMSI));
    S8 tmp[GSIM_TRACE_MSG_FILE_NAME_LEN] = {'\0'};
    m_maxSessions                        = 0;
    locGtpcPort                          = DFLT_GTPC_PORT;
    remGtpcPort                          = DFLT_GTPC_PORT;
    locGtpcSndPort                       = DFLT_GTPC_SEND_PORT;
    t3Timer                              = DFLT_T3_TIMER;
    n3Req                                = DFLT_N3_REQUESTS;
    dispTimer                            = DFLT_DISP_REFRESH_TIMER;
    dispTarget                           = DISP_TARGET_SCREEN;
    m_ssnRatePeriod                      = DFLT_SESSION_RATE_PERIOD;
    m_ssnRate                            = DFLT_SESSION_RATE;
    m_scnRunIntvl                        = 1000;
    m_logLevel                           = LOG_LVL_ERROR;
    m_ifType                             = GTP_IF_INF_INV;
    m_nodeType                           = EPC_NODE_INV;
    m_traceMsg                           = FALSE;
    pid_t   pid                          = getpid();
    sprintf(tmp, "%d.txt", pid);
    m_traceMsgFile                       = tmp;
    errFile                              = "";
    scnFile                              = "";
    m_logFile                            = "";
    dispTargetFile                       = "";
}

// Destructor
Config::~Config()
{
}

VOID Config::setNoOfCalls(U32 n)
{
    pCfg->m_maxSessions = n;
}

VOID Config::setCallRate(U32 n)
{
    pCfg->m_ssnRate = n;
}

VOID Config::setRatePeriod(U32 n)
{
   pCfg->m_ssnRatePeriod = n;
}

VOID Config::setLocalIpAddr(string ip) throw (ErrCodeEn)
{
    RETVAL ret = ROK;

    ret = saveIp(ip, &(pCfg->locIpAddr));
    if (RFAILED == ret)
    {
        LOG_FATAL("Invalid Local IP Address")
        throw ERR_INV_CMD_LINE_PARAM;
    }
}

VOID Config::setRemoteIpAddr(string ip) throw (ErrCodeEn)
{
   RETVAL ret = ROK;

   m_remIpAddrStr = ip;
   ret = saveIp(ip, &(pCfg->remIpAddr));
   if (RFAILED == ret)
   {
      LOG_FATAL("Invalid Remote IP Address")
         throw ERR_INV_CMD_LINE_PARAM;
   }
}

string Config::getRemIpAddrStr()
{
   return m_remIpAddrStr;
}

VOID Config::setLocalGtpcPort(U16 port)
{
    if (0 == port)
    {
        LOG_FATAL("Invalid Local Gtpc Port");
        LOG_INFO("Local Gtpc Port is Default GTPC Port 2123");
    }
    else
    {
        pCfg->locGtpcPort = port;
    }
}

VOID Config::setLocalGtpcSendPort(U16 port)
{
    if (0 == port)
    {
        LOG_FATAL("Invalid Local Gtpc Send Port");
        LOG_INFO("Local Gtpc Send Port is Default GTPC send "\
            "Port 2124");
    }
    else
    {
        pCfg->locGtpcSndPort = port;
    }
}

VOID Config::setRemoteGtpcPort(U16 port)
{
    if (0 == port)
    {
        LOG_FATAL("Invalid Remote Gtpc Port");
        LOG_INFO("Remote Gtpc Port is Default GTPC Port 2123");
    }
    else
    {
        pCfg->remGtpcPort = port;
    }
}

VOID Config::setT3TimerSeconds(U32 val)
{
    if (0 == val)
    {
        LOG_FATAL("Invalid T3 Timer value");
        LOG_INFO("T3 Timer is default value of 3 seconds");
    }
    else
    {
        pCfg->t3Timer = val;
    }
}

VOID Config::setN3Requests(U32 n)
{
#if 0
    if (0 == n)
    {
        LOG_FATAL("Invalid N3 Requests value");
        LOG_INFO("N3 Requests is default value of 3");
    }
    else
    {
    }
#else
        pCfg->n3Req = n;
#endif
}

U32 Config::getN3Requests()
{
   return n3Req;
}

VOID Config::setDisplayRefreshTimer(U32 n)
{
    if (0 == n)
    {
        LOG_FATAL("Invalid Display Refresh timer value");
        LOG_INFO("Display refresh timer is default value of "\
            "5 seconds");
        pCfg->dispTimer = DFLT_DISP_REFRESH_TIMER;
    }
    else
    {
        pCfg->dispTimer = n;
    }
}


U32 Config::getDisplayRefreshTimer()
{
    return pCfg->dispTimer;
}


U32 Config::getT3Timer()
{
    return pCfg->t3Timer;
}

const S8* Config::getScnFile()
{
   return pCfg->scnFile.c_str();
}

VOID Config::setDisplayTarget(DisplayTargetEn target)
{
    if ((DISP_TARGET_NONE < target) && (target < DISP_TARGET_MAX))
    {
        pCfg->dispTarget = target;
    }
    else
    {
        LOG_FATAL("Invalid Display Target");
        LOG_INFO("Display is set to default target: SCREEN");
    }
}

VOID Config::setErrorFile(string filename) throw (ErrCodeEn)
{
    if (filename.size() == 0)
    {
        LOG_FATAL("Invalid Error file");
        throw ERR_INV_CMD_LINE_PARAM;
    }
    else
    {
        pCfg->errFile = filename;
    }
}

VOID Config::setScenarioFile(S8* filename) throw (ErrCodeEn)
{
   if (NULL == filename || STRLEN(filename) == 0)
   {
      LOG_FATAL("Invalid Scenario file");
      throw ERR_INV_CMD_LINE_PARAM;
   }
   
   if (0 == STRNCMP(filename, "--", 2))
   {
      throw ERR_INV_CMD_LINE_PARAM;
   }

   pCfg->scnFile.assign(filename);
}

VOID Config::setLogFile(string filename) throw (ErrCodeEn)
{
    if (filename.size() == 0)
    {
        LOG_FATAL("Invalid Logfile file");
        throw ERR_INV_CMD_LINE_PARAM;
    }
    else
    {
        pCfg->m_logFile = filename;
    }
}

VOID Config::setDisplayTargetFile(string filename) throw (ErrCodeEn)
{
    if (filename.size() == 0)
    {
        LOG_FATAL("Invalid Display target file");
        throw ERR_INV_CMD_LINE_PARAM;
    }
    else
    {
        if (DISP_TARGET_FILE == pCfg->dispTarget)
        {
            pCfg->dispTargetFile = filename;
        }
        else
        {
            LOG_FATAL("Dispay Target is not matching");
            throw ERR_INV_CMD_LINE_PARAM;
        }
    }
}

RETVAL Config::saveIp(string &ipStr, IpAddr *pIp)
{
   RETVAL  ret = ROK;

   pIp->ipAddrType = IP_ADDR_TYPE_INV;
   if (ipStr.find(':') < ipStr.size())
   {
      ret = inet_pton(AF_INET6, ipStr.c_str(),
            (VOID *)(pIp->u.ipv6Addr.addr));
      if (0 == ret)
      {
         LOG_FATAL("Invalid IPv6 Address");
         ret = RFAILED;
      }
      else
      {
         pIp->u.ipv6Addr.len = ipStr.size();
         pIp->ipAddrType = IP_ADDR_TYPE_V6;
         ret = ROK;
      }
   }
   else
   {
      ret = inet_pton(AF_INET, ipStr.c_str(),
            (VOID *)&(pIp->u.ipv4Addr.addr));
      if (0 == ret)
      {
         LOG_FATAL("Invalid IPv4 Address");
         ret = RFAILED;
      }
      else
      {
         pIp->u.ipv4Addr.addr = ntohl(pIp->u.ipv4Addr.addr);
         pIp->ipAddrType = IP_ADDR_TYPE_V4;
         ret = ROK;
      }
   }

   return ret;
}

VOID Config::setParameter(CmdLineParam *pParam) throw (ErrCodeEn)
{
   try
   {
      switch (pParam->type)
      {
         case OPT_NUM_CALLS:
         {
            this->setNoOfCalls((U32)atoi(pParam->value.c_str()));
            break;
         }

         case OPT_CALL_RATE:
         {
            this->setCallRate((U32)atoi(pParam->value.c_str()));
            break;
         }

         case OPT_RATE_PERIOD:
         {
            this->setRatePeriod((U32)atoi(pParam->value.c_str()));
            break;
         }

         case OPT_LOC_IP_ADDR:
         {
            this->setLocalIpAddr(pParam->value);
            break;
         }

         case OPT_REM_IP_ADDR:
         {
            this->setRemoteIpAddr(pParam->value);
            break;
         }

         case OPT_LOC_PORT:
         {
            this->setLocalGtpcPort((U16)atoi(pParam->value.c_str()));
            break;
         }

         case OPT_REM_PORT:
         {
            this->setRemoteGtpcPort((U16)atoi(pParam->value.c_str()));
            break;
         }

         case OPT_T3_TIMER:
         {
            this->setT3TimerSeconds((U32)atoi(pParam->value.c_str()));
            break;
         }

         case OPT_N3_REQ:
         {
            this->setN3Requests((U32)atoi(pParam->value.c_str()));
            break;
         }

         case OPT_DISP_TIMER:
         {
            this->setDisplayRefreshTimer((U32)atoi(pParam->value.c_str()));
            break;
         }

         case OPT_DISP_OPT:
         {
            this->setDisplayTarget((DisplayTargetEn)atoi(\
                     pParam->value.c_str()));
            break;
         }

         case OPT_ERR_FILE:
         {
            this->setErrorFile(pParam->value);
            break;
         }

         case OPT_LOG_FILE:
         {
            this->setLogFile(pParam->value);
            break;
         }

         case OPT_NODE:
         {
            break;
         }

         case OPT_LOG_LEVEL:
         {
            this->setLogLevel(pParam->value);
            break;
         }

         case OPT_TRACE_MSG:
         {
            this->setTraceMsg(TRUE);
         }

         default:
         {
            LOG_FATAL("Unhandled Command Line Option "\
                  "type [%d]", pParam->type);
            throw ERR_INV_CMD_LINE_PARAM;
            break;
         }
      }
   }
   catch (ErrCodeEn err)
   {
      throw err;
   }
}

VOID Config::getArgDetails(string &arg, CmdLineParam *pParam) 
        throw(ErrCodeEn)
{
   S32     eqPos = 0; /* index of = */
   string  tmpArg = arg;

   if (0 != arg.compare(0, 2, "--"))
   {
      throw ERR_INV_CMD_LINE_PARAM;
   }

   arg.erase(0, 2); // skipping -- before the cmd line option

   /* get opt=value part */
   eqPos = arg.find("=");
   pParam->option = arg.substr(0, eqPos);
   pParam->type = getOptType(pParam->option);
   if (OPT_INV == pParam->type)
   {
      LOG_FATAL("Invalid Command Line option [--%s]",\
            pParam->option.c_str());
      throw ERR_INV_CMD_LINE_PARAM;
   }

   pParam->value = arg.substr(eqPos + 1);
}

CmdLineOptEn Config::getOptType(const string &opt)
{
    for (U32 i = 0; i < OPT_MAX; i++)
    {
        if (0 == gCfgOptioTable[i].option.compare(opt))
        {
            return gCfgOptioTable[i].type;
        }
    }

    return OPT_INV;
}


/**
 * @brief 
 *    GNU Style command line options parsing
 *
 * @param numArgs
 * @param cmdLineArgs
 *
 * @throw ErrCodeEn
 */
VOID Config::setConfig(S32 numArgs, S8** cmdLineArgs) throw (ErrCodeEn)
{
   CmdLineParam    arg;
   S32             argCnt = 1;
   string          argStr;

   if (numArgs < 2)
   {
      throw ERR_INV_CMD_LINE_PARAM;
   }
   else
   {
      try
      {
         while (argCnt < numArgs)
         {
            if (argCnt == (numArgs - 1))
            {
               /* the last argument should be scenario file */
               setScenarioFile(cmdLineArgs[argCnt]);
            }
            else
            {
               argStr = cmdLineArgs[argCnt];

               getArgDetails(argStr, &arg);
               setParameter(&arg);
            }

            argCnt++;
         }
      }
      catch (ErrCodeEn errCode)
      {
         LOG_FATAL("Parsing Command Line options failed");
         throw ERR_INV_CMD_LINE_PARAM;
      }
   }
}

const IpAddr* Config::getRemoteIpAddr()
{
    return &(this->remIpAddr);
}

const IpAddr* Config::getLocalIpAddr()
{
    return &(this->locIpAddr);
}

U16 Config::getRemoteGtpcPort()
{
    return remGtpcPort;
}

U16 Config::getLocalGtpcSendPort()
{
    return locGtpcSndPort;
}


U16 Config::getLocalGtpcPort()
{
    return locGtpcPort;
}

U32 Config::getScnRunInterval()
{
   return m_scnRunIntvl;
}

U32 Config::getCallRate()
{
   return m_ssnRate;
}

Time_t Config::getSessionRatePeriod()
{
   return m_ssnRatePeriod;
}

VOID Config::setLogLevel(string level)
{
   m_logLevel = (LogLevel_t)atoi(level.c_str());
}

U32 Config::getLogLevel()
{
   return m_logLevel;
}

U8 Config::getNodeType()
{
   return m_nodeType;
}

U8 Config::getIfType()
{
   return m_ifType;
}

Counter Config::getNumSessions()
{
   return m_maxSessions;
}

string Config::getLogFile()
{
   return m_logFile;
}

VOID Config::setTraceMsg(BOOL b)
{
   m_traceMsg = b;
}

BOOL Config::getTraceMsg()
{
   return m_traceMsg;
}

VOID Config::setTraceMsgFile(string fileName)
{
   m_traceMsgFile = fileName;
}

string Config::getTraceMsgFile()
{
   return m_traceMsgFile;
}

string Config::getImsi()
{
   return m_imsiStr;
}

VOID Config::setImsi(S8 *pVal, U32 len)
{
   m_imsiStr.assign(pVal, len);
}

VOID Config::incrRate(U32 value)
{
   if (m_ssnRate + value < DFLT_MAX_SESSION_RATE)
   {
      m_ssnRate += value;
   }
   else
   {
      LOG_ERROR("Maximum Session Rate Breached");
   }
}

VOID Config::decrRate(U32 value)
{
   if ((m_ssnRate > value) && (m_ssnRate - value >= DFLT_MIN_SESSION_RATE))
   {
      m_ssnRate -= value;
   }
   else
   {
      LOG_ERROR("Minimum Session Rate Breached");
   }
}
