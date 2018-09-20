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
#include <unistd.h>
#include <stdlib.h>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

#include "types.hpp"
#include "logger.hpp"
#include "error.hpp"
#include "macros.hpp"
#include "help.hpp"
#include "gtp_types.hpp"
#include "sim_cfg.hpp"

static Config *pCfg        = NULL;
static S8      DFLT_IMSI[] = "112233445566778";

// returns the Config class singleton instance ptr
Config *Config::getInstance()
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
    S8 tmp[DFLT_TRACE_MSG_FILE_NAME_LEN] = {'\0'};
    m_maxSessions                        = 0;
    locGtpcPort                          = DFLT_GTPC_PORT;
    remGtpcPort                          = DFLT_GTPC_PORT;
    t3Timer                              = DFLT_T3_TIMER;
    n3Req                                = DFLT_N3_REQUESTS;
    dispTimer                            = DFLT_DISP_REFRESH_TIMER;
    dispTarget                           = DISP_TARGET_SCREEN;
    m_ssnRatePeriod                      = DFLT_SESSION_RATE_PERIOD;
    m_ssnRate                            = DFLT_SESSION_RATE;
    m_deadCallWait                       = DFLT_DEAD_CALL_WAIT;
    m_scnRunIntvl                        = 1000;
    m_logLevel                           = LOG_LVL_ERROR;
    m_ifType                             = GTP_IF_INF_INV;
    m_nodeType                           = EPC_NODE_INV;
    m_traceMsg                           = FALSE;
    pid_t pid                            = getpid();
    m_localIpAddrStr                     = DFLT_LOCAL_IP_ADDR;

    saveIp(m_localIpAddrStr, &locIpAddr);

    /* Default log file name */
    m_logFile = std::to_string(getpid()) + ".txt";

    sprintf(tmp, "%d.txt", pid);
    m_traceMsgFile = tmp;
    errFile        = "";
    scnFile        = "";
    dispTargetFile = "";
}

// Destructor
Config::~Config()
{
}

/**
 * @brief
 *    GNU Style command line options parsing
 *
 * @param numArgs
 *
 */
VOID Config::setConfig(cxxopts::ParseResult options)
{
    if (options.count("node"))
    {
        auto value = options["node"].as<std::string>();
        setNodeType(value);
    }
    else
    {
        throw GsimError("Mandatory argument 'node' is missing");
    }

    if (options.count("scenario"))
    {
        auto value = options["scenario"].as<std::string>();
        setScenarioFile(value);
    }
    else
    {
        throw GsimError("Mandatory argument 'scenario' is missing");
    }

    if (options.count("iftype"))
    {
        auto value = options["iftype"].as<std::string>();
        setIfType(value);
    }
    else
    {
        throw GsimError("Mandatory argument 'iftype' is missing");
    }

    if (options.count("local-ip"))
    {
        auto value = options["local-ip"].as<std::string>();
        setLocalIpAddr(value);
    }

    if (options.count("local-port"))
    {
        auto value = options["local-port"].as<std::uint16_t>();
        setLocalGtpcPort(value);
    }

    if (options.count("remote-ip"))
    {
        auto value = options["remote-ip"].as<std::string>();
        setRemoteIpAddr(value);
    }

    if (options.count("remote-port"))
    {
        auto value = options["remote-port"].as<std::uint16_t>();
        setRemoteGtpcPort(value);
    }

    if (options.count("num-sessions"))
    {
        auto value = options["num-sessions"].as<std::uint32_t>();
        setNoOfCalls(value);
    }

    if (options.count("session-rate"))
    {
        auto value = options["session-rate"].as<std::uint32_t>();
        setCallRate(value);
    }

    if (options.count("rate-period"))
    {
        auto value = options["rate-period"].as<std::uint32_t>();
        setRatePeriod(value);
    }

    if (options.count("t3-timer"))
    {
        auto value = options["t3-timer"].as<std::uint32_t>();
        setT3TimerSeconds(value);
    }

    if (options.count("n3-requests"))
    {
        auto value = options["n3-requests"].as<std::uint32_t>();
        setN3Requests(value);
    }

    if (options.count("disp-timer"))
    {
        auto value = options["disp-timer"].as<std::uint32_t>();
        setDisplayRefreshTimer(value);
    }

    if (options.count("disp-target"))
    {
        auto value = options["disp-target"].as<std::uint32_t>();
        setDisplayTarget((DisplayTargetEn)value);
    }

    if (options.count("error-file"))
    {
        auto value = options["error-file"].as<std::string>();
        setErrorFile(value);
    }

    if (options.count("log-file"))
    {
        auto value = options["log-file"].as<std::string>();
        setLogFile(value);
    }

    if (options.count("log-level"))
    {
        auto value = options["log-level"].as<std::uint32_t>();
        setLogLevel(value);
    }
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

VOID Config::setLocalIpAddr(string ip)
{
    RETVAL ret = ROK;

    ret = saveIp(ip, &(pCfg->locIpAddr));
    if (RFAILED == ret)
    {
        throw GsimError("Invalid Local IP Address");
    }

    m_localIpAddrStr = ip;
}

VOID Config::setRemoteIpAddr(string ip)
{
    RETVAL ret = ROK;

    ret = saveIp(ip, &(pCfg->remIpAddr));
    if (RFAILED == ret)
    {
        throw GsimError("Invalid Remote IP Address");
    }

    m_remIpAddrStr = ip;
}

string Config::getRemIpAddrStr()
{
    return m_remIpAddrStr;
}

string Config::getLocalIpAddrStr()
{
    return m_localIpAddrStr;
}

VOID Config::setLocalGtpcPort(U16 port)
{
    if (0 == port)
    {
        throw GsimError("Invalid Local Gtpc Port");
    }
    else
    {
        pCfg->locGtpcPort = port;
    }
}

VOID Config::setRemoteGtpcPort(U16 port)
{
    if (0 == port)
    {
        throw GsimError("Invalid Remote Gtpc Port");
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
        throw GsimError("Invalid T3 Timer value");
        LOG_INFO("T3 Timer is default value of 3 seconds");
    }
    else
    {
        pCfg->t3Timer = val;
    }
}

VOID Config::setN3Requests(U32 n)
{
    pCfg->n3Req = n;
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
        LOG_INFO(
            "Display refresh timer is default value of "
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

const S8 *Config::getScnFile()
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
        throw GsimError("Invalid Display Target");
        LOG_INFO("Display is set to default target: SCREEN");
    }
}

VOID Config::setErrorFile(string filename) throw(ErrCodeEn)
{
    if (filename.size() == 0)
    {
        throw GsimError("Invalid Error file");
    }
    else
    {
        pCfg->errFile = filename;
    }
}

VOID Config::setScenarioFile(std::string filename) throw(ErrCodeEn)
{
    pCfg->scnFile.assign(filename.c_str());
}

VOID Config::setLogFile(string filename) throw(ErrCodeEn)
{
    if (filename.size())
    {
        pCfg->m_logFile = filename;
    }
}

VOID Config::setDisplayTargetFile(string filename)
{
    if (filename.size() == 0)
    {
        throw GsimError("Invalid Display target file");
    }
    else
    {
        if (DISP_TARGET_FILE == pCfg->dispTarget)
        {
            pCfg->dispTargetFile = filename;
        }
        else
        {
            throw GsimError("Dispay Target is not matching");
        }
    }
}

RETVAL Config::saveIp(string &ipStr, IpAddr *pIp)
{
    RETVAL ret = ROK;

    pIp->ipAddrType = IP_ADDR_TYPE_INV;
    if (ipStr.find(':') < ipStr.size())
    {
        ret =
            inet_pton(AF_INET6, ipStr.c_str(), (VOID *)(pIp->u.ipv6Addr.addr));
        if (0 == ret)
        {
            throw GsimError("Invalid IPv6 Address");
            ret = RFAILED;
        }
        else
        {
            pIp->u.ipv6Addr.len = ipStr.size();
            pIp->ipAddrType     = IP_ADDR_TYPE_V6;
            ret                 = ROK;
        }
    }
    else
    {
        ret =
            inet_pton(AF_INET, ipStr.c_str(), (VOID *)&(pIp->u.ipv4Addr.addr));
        if (0 == ret)
        {
            throw GsimError("Invalid IPv4 Address");
            ret = RFAILED;
        }
        else
        {
            pIp->u.ipv4Addr.addr = ntohl(pIp->u.ipv4Addr.addr);
            pIp->ipAddrType      = IP_ADDR_TYPE_V4;
            ret                  = ROK;
        }
    }

    return ret;
}

IpAddr Config::getRemoteIpAddr()
{
    return this->remIpAddr;
}

const IpAddr *Config::getLocalIpAddr()
{
    return &(this->locIpAddr);
}

U16 Config::getRemoteGtpcPort()
{
    return remGtpcPort;
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

VOID Config::setLogLevel(std::uint32_t level)
{
    m_logLevel = level;
}

U32 Config::getLogLevel()
{
    return m_logLevel;
}

EpcNodeType_t Config::getNodeType()
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

Time_t Config::getDeadCallWait()
{
    return m_deadCallWait;
}

void Config::setNodeType(std::string node)
{
    if (!STRCASECMP(node.c_str(), "MME"))
    {
        m_nodeTypStr = "MME";
        m_nodeType   = EPC_NODE_MME;
    }
    else if (!STRCASECMP(node.c_str(), "SGW"))
    {
        m_nodeTypStr = "SGW";
        m_nodeType   = EPC_NODE_SGW;
    }
    else if (!STRCASECMP(node.c_str(), "PGW"))
    {
        m_nodeTypStr = "PGW";
        m_nodeType   = EPC_NODE_PGW;
    }
    else if (!STRCASECMP(node.c_str(), "SGSN"))
    {
        m_nodeTypStr = "SGSN";
        m_nodeType   = EPC_NODE_SGSN;
    }
    else
    {
        LOG_ERROR("Invalid node type parameter");
    }
}

std::string Config::getNodeTypeStr()
{
    return m_nodeTypStr;
}

void Config::setIfType(std::string ifType)
{
    if (ifType == "s11mme")
    {
        m_ifType = GTP_IF_S11_C_MME;
    }
    else if (ifType == "s11sgw")
    {
        m_ifType = GTP_IF_S11_C_SGW;
    }
    else if (ifType == "s5s8sgw")
    {
        m_ifType = GTP_IF_S5S8_C_SGW;
    }
    else if (ifType == "s5s8pgw")
    {
        m_ifType = GTP_IF_S5S8_C_PGW;
    }
    else
    {
        std::string log = "Invalid interface type " + ifType;
        throw GsimError(log);
    }
}
