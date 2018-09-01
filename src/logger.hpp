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

/* Logger functions for logging errors, function execution logs
 * hex dumps etc
 */

#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#define LOG_LVL_STR_MAX 16
#define LOG_BUF_MAX 1024

typedef enum {
    LOG_LVL_START,
    LOG_LVL_FATAL,
    LOG_LVL_WARN,
    LOG_LVL_ERROR,
    LOG_LVL_INFO,
    LOG_LVL_DEBUG,
    LOG_LVL_TRACE,
    LOG_LVL_END
} LogLevel_t;

#ifdef DEBUG
#define LOG_ENTERFN()                                                       \
    do                                                                      \
    {                                                                       \
        if (LOG_LVL_TRACE <= Logger::m_logLevel)                            \
        {                                                                   \
            Logger::log(LOG_LVL_TRACE, __FILE__, __LINE__, "Entering %s()", \
                __PRETTY_FUNCTION__);                                       \
        }                                                                   \
    } while (0)
#else
#define LOG_ENTERFN()
#endif

#ifdef DEBUG
#define LOG_EXITFN(_ret)                                                   \
    do                                                                     \
    {                                                                      \
        if (LOG_LVL_TRACE <= Logger::m_logLevel)                           \
        {                                                                  \
            Logger::log(LOG_LVL_TRACE, __FILE__, __LINE__, "Exiting %s()", \
                __PRETTY_FUNCTION__);                                      \
        }                                                                  \
        return _ret;                                                       \
    } while (0)
#else
#define LOG_EXITFN(_ret) return (_ret);
#endif

#ifdef DEBUG
#define LOG_EXITVOID()                                                     \
    do                                                                     \
    {                                                                      \
        if (LOG_LVL_TRACE == Logger::m_logLevel)                           \
        {                                                                  \
            Logger::log(LOG_LVL_TRACE, __FILE__, __LINE__, "Exiting %s()", \
                __PRETTY_FUNCTION__);                                      \
        }                                                                  \
        return;                                                            \
    } while (0)
#else
#define LOG_EXITVOID() return
#endif

#define LOG_TRACE_MSG(_n, _b, _l, _d, _ip, _port)    \
    do                                               \
    {                                                \
        if (Logger::m_traceMsgEnabled)               \
        {                                            \
            Logger::log(_n, _b, _l, _d, _ip, _port); \
        }                                            \
    } while (0)

#define LOG_ERROR(...)                                                   \
    {                                                                    \
        if (LOG_LVL_ERROR <= Logger::m_logLevel)                         \
        {                                                                \
            Logger::log(LOG_LVL_ERROR, __FILE__, __LINE__, __VA_ARGS__); \
        }                                                                \
    }

#ifdef DEBUG
#define LOG_DEBUG(...)                                                   \
    {                                                                    \
        if (LOG_LVL_DEBUG <= Logger::m_logLevel)                         \
        {                                                                \
            Logger::log(LOG_LVL_DEBUG, __FILE__, __LINE__, __VA_ARGS__); \
        }                                                                \
    }
#else
#define LOG_DEBUG(...)
#endif

#define LOG_TRACE(...)                                                   \
    {                                                                    \
        if (LOG_LVL_TRACE <= Logger::m_logLevel)                         \
        {                                                                \
            Logger::log(LOG_LVL_TRACE, __FILE__, __LINE__, __VA_ARGS__); \
        }                                                                \
    }

#define LOG_INFO(...)                                                   \
    {                                                                   \
        if (LOG_LVL_INFO <= Logger::m_logLevel)                         \
        {                                                               \
            Logger::log(LOG_LVL_INFO, __FILE__, __LINE__, __VA_ARGS__); \
        }                                                               \
    }

#define LOG_FATAL(...)                                                   \
    {                                                                    \
        if (LOG_LVL_FATAL <= Logger::m_logLevel)                         \
        {                                                                \
            Logger::log(LOG_LVL_FATAL, __FILE__, __LINE__, __VA_ARGS__); \
        }                                                                \
    }

#define LOG_WARN(...)                                                   \
    {                                                                   \
        if (LOG_LVL_WARN <= Logger::m_logLevel)                         \
        {                                                               \
            Logger::log(LOG_LVL_WARN, __FILE__, __LINE__, __VA_ARGS__); \
        }                                                               \
    }

class Logger
{
public:
    static LogLevel_t m_logLevel;

    /**
     * Logging function. This function is logger for gtp simulator for
     * debugging purposes
     */
    static VOID log(LogLevel_t logLvl, const char *fileName,
        const unsigned lineNum, const char *format, ...);

    /**
     * @brief
     *    Logging function for gtp message. This function is used to
     *    logging the GTP message hex dumps. This function is intented
     *    for creating log with GTP message transactions.
     *
     * @param pMsgName
     *    char pointer containing the name of GTP message sent or
     *    received
     * @param pGtpMsgHexDump
     *    Pointer to buffer containing the entire GTP message
     * @param msgLen
     *    Length of gtp message buffer
     * @param dir
     *    Message direction (in/out)
     * @param pIpAddr
     *    char pointer to ip address string from where the message is
     *    received or being sent
     * @param port
     *    Port number from where the gtp message is received or being sent
     *
     * @return
     *    No return value
     */
    static VOID log(const U8 *pMsgName, const U8 *pMsgBuf, U32 msgLen,
        MsgAction_t dir, const U8 *pIpAddr, U16 port);

    static VOID init(U32 level);

    static FILE *m_logFile;
    static FILE *m_traceMsgFile;
    static BOOL  m_traceMsgEnabled;
    static VOID  setLogLevel(LogLevel_t level);
};

#endif
