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

#include <list>
#include <vector>
#include <map>

#include "types.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "gtp_macro.hpp"
#include "task.hpp"
#include "timer.hpp"
#include "transport.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "sim_cfg.hpp"
#include "procedure.hpp"
#include "gtp_stats.hpp"
#include "gtp_peer.hpp"
#include "scenario.hpp"
#include "tunnel.hpp"
#include "traffic.hpp"
#include "session.hpp"

static UeSessionMap s_ueSessionMap;
static U32          g_sessionId = 0;

/**
 * @brief
 *    Constructor
 *
 * @param pScn
 */
UeSession::UeSession(Scenario *pScn, GtpImsiKey imsi)
{
    m_pScn          = pScn;
    m_retryCnt      = 0;
    m_sessionId     = ++g_sessionId;
    m_t3time        = Config::getInstance()->getT3Timer();
    m_nodeType      = Config::getInstance()->getNodeType();
    m_n3req         = Config::getInstance()->getN3Requests();
    m_peerEp.ipAddr = Config::getInstance()->getRemoteIpAddr();
    m_peerEp.port   = Config::getInstance()->getRemoteGtpcPort();
    m_bitmask       = 0;
    m_imsiKey       = imsi;
    m_bearerVec.reserve(GTP_MAX_BEARERS);
    m_currProcItr = m_pScn->getFirstProcedure();

    for (U32 i = 0; i < GTP_MAX_BEARERS; i++)
    {
        m_bearerVec[i] = NULL;
    }

    LOG_DEBUG("Creating UE Session [%d]", m_sessionId);
}

/**
 * @brief
 *    Destructor for UeSession
 */
UeSession::~UeSession()
{
    s_ueSessionMap.erase(m_imsiKey);

    if (NULL != m_currProcCache.sentMsg)
        delete m_currProcCache.sentMsg;

    if (NULL != m_prevProcCache.sentMsg)
        delete m_prevProcCache.sentMsg;

    for (GtpcPdnLstItr pPdn = m_pdnLst.begin(); pPdn != m_pdnLst.end(); pPdn++)
    {
        /* delete the c-plane tunnels */
        GtpcTun *pCTun = (*pPdn)->pCTun;
        if (NULL != pCTun)
        {
            deleteCTun(pCTun);
            pCTun = NULL;
        }

        for (U32 i = 0; i < GTP_MAX_BEARERS; i++)
        {
            GtpBearer *bearer = m_bearerVec[i];
            if (NULL != bearer)
            {
                if (GSIM_CHK_BEARER_MASK((*pPdn)->bearerMask, bearer->getEbi()))
                {
                    GSIM_UNSET_BEARER_MASK(
                        (*pPdn)->bearerMask, bearer->getEbi());
                    delete bearer;
                }
            }
        }

        delete *pPdn;
    }

    LOG_DEBUG("Deleting UE Session [%d]", m_sessionId);
}

RETVAL UeSession::run(VOID *arg)
{
    RETVAL ret = ROK;

    LOG_TRACE("Running UeSession [%d]", m_sessionId);
    m_lastRunTime = m_currRunTime;
    m_currRunTime = getMilliSeconds();

    if (GSIM_CHK_MASK(m_bitmask, GSIM_UE_SSN_SCN_COMPLETE))
    {
        ret = handleDeadCall(arg);
    }
    else
    {
        if (NULL != arg)
        {
            LOG_TRACE("Processing Recv() Task");
            ret = handleRecv((UdpData_t *)arg);
        }
        else
        {
            if (PROC_TYPE_WAIT == (*m_currProcItr)->type())
            {
                LOG_TRACE("Processing Wait() Task");
                ret = handleWait();
            }
            else
            {
                LOG_TRACE("Processing Send() Task");
                ret = handleSend();
            }
        }
    }

    LOG_EXITFN(ret);
}

RETVAL UeSession::handleSend()
{
    LOG_ENTERFN();

    RETVAL     ret      = ROK;
    Procedure *currProc = *m_currProcItr;

    if (GSIM_CHK_MASK(this->m_bitmask, GSIM_UE_SSN_WAITING_FOR_RSP))
    {
        LOG_DEBUG("Processing Request Timeout")
        ret = handleOutReqTimeout();
        if (ERR_MAX_RETRY_EXCEEDED == ret)
        {
            currProc->m_initial->m_numTimeOut++;
            Stats::incStats(GSIM_STAT_NUM_SESSIONS_FAIL);
            delete m_currProcCache.sentMsg;
            m_currProcCache.sentMsg = NULL;

            /* request retry exceeded n3-requests. terminate the
             * UE session Task
             */
            ret = ROK_OVER;
        }
    }
    else if (GSIM_CHK_MASK(this->m_bitmask, GSIM_UE_SSN_SEND_RSP))
    {
        GtpMsg *gtpMsg = currProc->m_trigMsg->getGtpMsg();
        ret            = handleOutRspMsg(gtpMsg);
        if (ROK != ret)
        {
            /* sending a response message failed, terminate the Task */
            LOG_ERROR("Sending response message to peer, Error [%d]", ret);
            ret = ROK_OVER;
        }
    }
    else
    {
        /* a request task gets over only when a timeout occurs after
         * maximum n3-request retris, or when an associated response
         * message is received. so after processing sending the request
         * out do not finish the task
         */
        GtpMsg *gtpMsg = currProc->m_initial->getGtpMsg();
        ret            = handleOutReqMsg(gtpMsg);
        if (ROK == ret)
        {
            /* update the wakeup time and pause this task until then,
             * for retransmissing the request message
             */
            m_wakeTime = m_currRunTime + m_t3time;
            pause();
        }
        else
        {
            /* sending a request message failed, terminate task */
            LOG_ERROR("Sending request message to peer, Error [%d]", ret);
            ret = ROK_OVER;
        }
    }

    LOG_EXITFN(ret);
}

RETVAL UeSession::handleOutReqMsg(GtpMsg *gtpMsg)
{
    LOG_ENTERFN();

    RETVAL     ret      = ROK;
    GtpcPdn *  pPdn     = NULL;
    Procedure *currProc = *m_currProcItr;

    if (GTPC_MSG_CS_REQ == gtpMsg->type())
    {
        LOG_DEBUG("Creating PDN Connection");
        Stats::incStats(GSIM_STAT_NUM_SESSIONS_CREATED);
        Stats::incStats(GSIM_STAT_NUM_SESSIONS);
        pPdn = createPdn();
        m_pdnLst.push_back(pPdn);
        m_pCurrPdn = pPdn;
    }
    else
    {
        pPdn = m_pCurrPdn;
    }

    LOG_DEBUG("Storing OUT Message");
    createBearers(pPdn, gtpMsg, 0);

    LOG_DEBUG("Encoding OUT Message");
    m_currProcCache.seqNumber = generateSeqNum(&m_peerEp, GTP_MSG_CAT_REQ);
    m_currProcCache.reqType   = gtpMsg->type();
    UdpData_t *pNwData        = new UdpData_t;
    encGtpcOutMsg(pPdn, gtpMsg, &pNwData->buf, &m_peerEp);

    /* initial message, send the message over default send socket */
    m_retryCnt      = 0;
    pNwData->connId = 0;
    pNwData->peerEp = m_peerEp;

    LOG_DEBUG("Sending GTPC Message [%s]", gtpGetMsgName(msgType));
    Buffer *buf = new Buffer(pNwData->buf);
    sendMsg(pNwData->connId, &pNwData->peerEp, buf);
    currProc->m_initial->m_numSnd++;
    m_currProcCache.sentMsg = pNwData;
    GSIM_SET_MASK(this->m_bitmask, GSIM_UE_SSN_WAITING_FOR_RSP);

    LOG_EXITFN(ret);
}

/**
 * @brief Request timeout handler. If maximum retries exceeds returns
 *    failure, so that the session is terminated. otherwise resends
 *    the request message and sets the next wakeup time
 *
 * @return
 */
RETVAL UeSession::handleOutReqTimeout()
{
    LOG_ENTERFN();

    RETVAL     ret      = ROK;
    Procedure *currProc = *m_currProcItr;

    /* Recived task is run because GTP-C message request timedout
     * waiting for a response, retransmit the request message
     */
    if (m_retryCnt >= m_n3req)
    {
        delete m_currProcCache.sentMsg;
        m_currProcCache.sentMsg = NULL;
        LOG_DEBUG("Maximum Retries reached");
        ret = ERR_MAX_RETRY_EXCEEDED;
    }
    else
    {
        /* we have already processed this message, the task is run again
         * after retransmission timeout expiry
         */
        LOG_DEBUG("Retransmissing GTP Message");
        Buffer *buf = new Buffer(m_currProcCache.sentMsg->buf);
        sendMsg(m_currProcCache.sentMsg->connId,
            &m_currProcCache.sentMsg->peerEp, buf);

        currProc->m_initial->m_numSndRetrans++;
        m_retryCnt++;

        // if response is not received within T3 timer expiry
        // wakeup and retransmit request message
        m_wakeTime = m_currRunTime + m_t3time;
        pause();
    }

    LOG_EXITFN(ret);
}

RETVAL UeSession::handleOutRspMsg(GtpMsg *gtpMsg)
{
    LOG_ENTERFN();

    GtpcPdn *  pPdn     = m_pCurrPdn;
    Procedure *currProc = *m_currProcItr;

    LOG_DEBUG("Encoding OUT Message");
    UdpData_t *pNwData = new UdpData_t;
    encGtpcOutMsg(
        pPdn, currProc->m_trigMsg->getGtpMsg(), &pNwData->buf, &m_peerEp);

    /* send the response/triggered message over the same socket
     * over which the request/command is received
     */
    pNwData->connId = m_currProcCache.connId;
    pNwData->peerEp = pPdn->pCTun->m_peerEp;

    LOG_DEBUG("Sending GTPC Message [%s]", gtpGetMsgName(msgType));
    Buffer *buf = new Buffer(pNwData->buf);
    sendMsg(pNwData->connId, &pNwData->peerEp, buf);
    currProc->m_trigMsg->m_numSnd++;

    delete m_prevProcCache.sentMsg;
    m_prevProcCache.sentMsg = pNwData;
    m_prevProcCache.rspType = gtpMsg->type();
    m_prevProcItr           = m_currProcItr;
    GSIM_SET_MASK(this->m_bitmask, GSIM_UE_SSN_PREV_PROC_PRES);
    GSIM_UNSET_MASK(this->m_bitmask, GSIM_UE_SSN_SEND_RSP);

    if (m_pScn->isScenarioEnd(m_currProcItr))
    {
        handleCompletedTask();
        LOG_EXITFN(ROK);
    }

    m_currProcItr = m_pScn->getNextProcedure(m_currProcItr);
    this->stop();

    LOG_EXITFN(ROK);
}

RETVAL UeSession::handleRecv(UdpData_t *data)
{
    LOG_ENTERFN();

    RETVAL ret = ROK;

    /* Receive task is run because a GTPC message is received for this
     * session
     */
    GtpMsg           gtpMsg(&data->buf);
    GtpMsgCategory_t msgCat = gtpMsg.category();

    if (msgCat == GTP_MSG_CAT_REQ)
    {
        LOG_DEBUG("Processing Incoming Request message");
        ret = handleIncReqMsg(&gtpMsg, data);
        if (ROK != ret)
        {
            LOG_ERROR("Processing Incoming Request Message, Error [%d]", ret);
        }
    }
    else if (msgCat == GTP_MSG_CAT_RSP)
    {
        LOG_DEBUG("Processing Incoming Response message");
        ret = handleIncRspMsg(&gtpMsg, data);
        if (ROK != ret)
        {
            LOG_ERROR("Processing Incoming Response Message, Error [%d]", ret);
        }
    }

    delete data;
    LOG_EXITFN(ret);
}

RETVAL UeSession::handleIncReqMsg(GtpMsg *rcvdReq, UdpData_t *rcvdData)
{
    LOG_ENTERFN();

    if (isExpectedReq(rcvdReq))
    {
        (*m_currProcItr)->m_initial->m_numRcv++;
    }
    else if (isPrevProcReq(rcvdReq))
    {
        /* resend the response message */
        Buffer *buf = new Buffer(m_prevProcCache.sentMsg->buf);
        sendMsg(m_prevProcCache.sentMsg->connId,
            &m_prevProcCache.sentMsg->peerEp, buf);
        (*m_prevProcItr)->m_initial->m_numRcvRetrans++;
        (*m_prevProcItr)->m_trigMsg->m_numSndRetrans++;
        this->stop();
        LOG_EXITFN(ROK);
    }
    else
    {
        (*m_currProcItr)->m_initial->m_numUnexp++;
        this->stop();
        LOG_EXITFN(ROK);
    }

    GtpcPdn *pdn = NULL;
    if (GTPC_MSG_CS_REQ == rcvdReq->type())
    {
        LOG_DEBUG("Creating PDN Connection");
        Stats::incStats(GSIM_STAT_NUM_SESSIONS_CREATED);
        Stats::incStats(GSIM_STAT_NUM_SESSIONS);
        pdn        = createPdn();
        m_pCurrPdn = pdn;
        m_pdnLst.push_back(pdn);
    }
    else
    {
        pdn = m_pCurrPdn;
    }

    m_currProcCache.connId    = rcvdData->connId;
    m_currProcCache.seqNumber = rcvdReq->seqNumber();
    m_currProcCache.reqType   = rcvdReq->type();
    m_prevProcCache.connId    = m_currProcCache.connId;
    m_prevProcCache.seqNumber = m_currProcCache.seqNumber;
    m_prevProcCache.reqType   = m_currProcCache.reqType;

    updatePeerSeqNumber(&rcvdData->peerEp, m_currProcCache.seqNumber);
    decAndStoreGtpcIncMsg(pdn, rcvdReq, &rcvdData->peerEp);

    /* run the procedure again to send the response */
    GSIM_SET_MASK(this->m_bitmask, GSIM_UE_SSN_SEND_RSP);
    this->run();

    LOG_EXITFN(ROK);
}

BOOL UeSession::isExpectedRsp(GtpMsg *rspMsg)
{
    LOG_ENTERFN();

    BOOL       expected       = FALSE;
    Procedure *currProc       = *m_currProcItr;
    GtpMsg *   expectedRspMsg = currProc->m_trigMsg->getGtpMsg();

    if ((expectedRspMsg->type() == rspMsg->type()) &&
        (m_currProcCache.seqNumber == rspMsg->seqNumber()))
    {
        expected = TRUE;
    }

    LOG_EXITFN(expected);
}

BOOL UeSession::isExpectedReq(GtpMsg *reqMsg)
{
    LOG_ENTERFN();

    BOOL       expected = FALSE;
    Procedure *currProc = *m_currProcItr;

    GtpMsg *expectedReqMsg = currProc->m_initial->getGtpMsg();
    if ((expectedReqMsg->type() == reqMsg->type()) &&
        (m_currProcCache.seqNumber < reqMsg->seqNumber()))
    {
        expected = TRUE;
    }

    LOG_EXITFN(expected);
}

BOOL UeSession::isPrevProcRsp(GtpMsg *rspMsg)
{
    LOG_ENTERFN();

    BOOL prevProcRsp = FALSE;

    if ((GSIM_CHK_MASK(m_bitmask, GSIM_UE_SSN_PREV_PROC_PRES)) &&
        (m_prevProcCache.rspType == rspMsg->type()) &&
        (m_prevProcCache.seqNumber == rspMsg->seqNumber()))
    {
        prevProcRsp = TRUE;
    }

    LOG_EXITFN(prevProcRsp);
}

BOOL UeSession::isPrevProcReq(GtpMsg *reqMsg)
{
    LOG_ENTERFN();

    BOOL prevProcReq = FALSE;

    if ((GSIM_CHK_MASK(m_bitmask, GSIM_UE_SSN_PREV_PROC_PRES)) &&
        (m_prevProcCache.reqType == reqMsg->type()) &&
        (m_prevProcCache.seqNumber == reqMsg->seqNumber()))
    {
        prevProcReq = TRUE;
    }

    LOG_EXITFN(prevProcReq);
}

PUBLIC RETVAL UeSession::handleIncRspMsg(GtpMsg *rspMsg, UdpData_t *rcvdData)
{
    LOG_ENTERFN();

    Procedure *currProc = *m_currProcItr;

    if (isExpectedRsp(rspMsg))
    {
        LOG_DEBUG("Expected response message received");

        currProc->m_trigMsg->m_numRcv++;

        m_prevProcCache.connId    = rcvdData->connId;
        m_prevProcCache.seqNumber = m_currProcCache.seqNumber;
        m_prevProcCache.reqType   = m_currProcCache.reqType;
        m_prevProcCache.rspType   = rspMsg->type();
        GSIM_SET_MASK(this->m_bitmask, GSIM_UE_SSN_PREV_PROC_PRES);
        m_prevProcItr = m_currProcItr;

        decAndStoreGtpcIncMsg(m_pCurrPdn, rspMsg, &rcvdData->peerEp);
        GSIM_UNSET_MASK(this->m_bitmask, GSIM_UE_SSN_WAITING_FOR_RSP);

        delete m_currProcCache.sentMsg;
        m_currProcCache.sentMsg = NULL;

        if (m_pScn->isScenarioEnd(m_currProcItr))
        {
            handleCompletedTask();
        }
        else
        {
            m_currProcItr = m_pScn->getNextProcedure(m_currProcItr);
        }
    }
    else if (isPrevProcRsp(rspMsg))
    {
        /* may be a retransmitted response for previous procedure */
        LOG_DEBUG("Response Message for previous procedure received");
        (*m_prevProcItr)->m_trigMsg->m_numRcvRetrans++;
    }
    else
    {
        /* unexpecte response message received */
        LOG_DEBUG("Unexpected response Message received");
        currProc->m_trigMsg->m_numUnexp++;
    }

    LOG_EXITFN(ROK);
}

RETVAL UeSession::handleWait()
{
    LOG_ENTERFN();

    Procedure *currProc = *m_currProcItr;

    /* pause the task until wake up time */
    m_wakeTime = m_currRunTime + currProc->m_wait->wait();
    pause();

    m_prevProcItr = m_currProcItr;
    m_currProcItr = m_pScn->getNextProcedure(m_currProcItr);

    LOG_EXITFN(ROK);
}

/**
 * @brief
 *    Creates a new UE Session with imsi = imsiKey
 *    Used when the session is created by this simulator entity
 *
 * @param imsiKey
 * @param pGtpMsg
 *
 * @return
 */
UeSession *UeSession::createUeSession(GtpImsiKey imsiKey)
{
    U8 *pImsi = imsiKey.val;

    Scenario * pScn   = Scenario::getInstance();
    UeSession *pUeSsn = new UeSession(pScn, imsiKey);
    s_ueSessionMap.insert(UeSessionMapPair(imsiKey, pUeSsn));

    LOG_ERROR("Creating UE Session [%x%x%x%x%x%x%x%x]", pImsi[0], pImsi[1],
        pImsi[2], pImsi[3], pImsi[4], pImsi[5], pImsi[6], pImsi[7]);

    return pUeSsn;
}

/**
 * @brief
 *    returns the ue session given the control plane teid
 *
 * @param teid
 *
 * @return
 */
UeSession *UeSession::getUeSession(GtpTeid_t teid)
{
    LOG_ENTERFN();

    UeSession *pUeSession = NULL;
    GtpcTun *  pCTun      = findCTun(teid);

    if (NULL != pCTun)
    {
        pUeSession = pCTun->m_pUeSession;
    }

    LOG_EXITFN(pUeSession);
}

UeSession *UeSession::getUeSession(GtpImsiKey imsiKey)
{
    LOG_ENTERFN();

    UeSession *pUeSession = NULL;

    UeSessionMapItr itr = s_ueSessionMap.find(imsiKey);
    if (itr != s_ueSessionMap.end())
    {
        pUeSession = itr->second;
    }

    LOG_EXITFN(pUeSession);
}

GtpcPdn *UeSession::createPdn()
{
    LOG_ENTERFN();

    GtpcPdn *pPdn = NULL;

    try
    {
        pPdn             = new GtpcPdn;
        pPdn->pUeSession = this;

        LOG_DEBUG("Creating GTP-C Tunnel");
        pPdn->pCTun = createCTun(pPdn);
    }
    catch (std::exception &e)
    {
        LOG_FATAL("Memory allocation GtpcPdn");
        throw ERR_PDN_CREATION;
    }

    LOG_EXITFN(pPdn);
}

/**
 * @brief Creates bearer contexts by parsing Bearer Context IE of GTP
 *    message
 *
 * @param pPdn PDN which the bearers are associated
 * @param pGtpMsg Gtp Message
 * @param instance instance of the Bearer Context ID
 */
VOID UeSession::createBearers(
    GtpcPdn *pPdn, GtpMsg *pGtpMsg, GtpInstance_t instance)
{
    LOG_ENTERFN();

    if (pGtpMsg->type() == GTPC_MSG_CS_REQ)
    {
        // U32 bearerCnt = pGtpMsg->getIeCount(GTP_IE_BEARER_CNTXT, instance);
        U32 bearerCnt = pGtpMsg->getBearersToCreate();
        for (U32 i = 1; i <= bearerCnt; i++)
        {
            GtpIe *pIe = pGtpMsg->getIe(GTP_IE_BEARER_CNTXT, instance, i);
            GtpBearerContext *bearerCntxt =
                dynamic_cast<GtpBearerContext *>(pIe);
            GtpEbi_t ebi = bearerCntxt->getEbi();

            GtpBearer *pBearer = new GtpBearer(pPdn, ebi);
            GSIM_SET_BEARER_MASK(pPdn->bearerMask, ebi);
            m_bearerVec[GTP_BEARER_INDEX(ebi)] = pBearer;
        }
    }

    LOG_EXITVOID();
}

VOID UeSession::decAndStoreGtpcIncMsg(
    GtpcPdn *pPdn, GtpMsg *pGtpMsg, const IPEndPoint *pPeerEp)
{
    LOG_ENTERFN();

    try
    {
        pGtpMsg->decode();
        GtpMsgType_t rcvdMsgTye = pGtpMsg->type();
        if (rcvdMsgTye == GTPC_MSG_CS_REQ || rcvdMsgTye == GTPC_MSG_CS_RSP)
        {
            GtpFteid *pFteid =
                dynamic_cast<GtpFteid *>(pGtpMsg->getIe(GTP_IE_FTEID, 0, 1));
            pPdn->pCTun->m_remTeid = pFteid->getTeid();
        }

        pPdn->pCTun->m_peerEp.ipAddr.ipAddrType = IP_ADDR_TYPE_V4;
        pPdn->pCTun->m_peerEp.port              = pPeerEp->port;
        pPdn->pCTun->m_peerEp.ipAddr            = pPeerEp->ipAddr;

        if (pGtpMsg->type() == GTPC_MSG_CS_REQ)
        {
            createBearers(pPdn, pGtpMsg, 0);
        }
    }
    catch (ErrCodeEn &e)
    {
        LOG_ERROR("Decoding of GTP message failed, Error Code [%d]", e);
    }

    LOG_EXITVOID();
}

VOID UeSession::encGtpcOutMsg(
    GtpcPdn *pPdn, GtpMsg *pGtpMsg, Buffer *pGtpBuf, IPEndPoint *peerEp)
{
    LOG_ENTERFN();

    U8  buf[GTP_MSG_BUF_LEN];
    U32 len = 0;

    /* Modify the header parameters dynamically */
    GtpMsgHdr msgHdr;
    msgHdr.teid = pPdn->pCTun->m_remTeid;
    msgHdr.seqN = m_currProcCache.seqNumber;
    GSIM_SET_MASK(msgHdr.pres, GTP_MSG_HDR_TEID_PRES);
    GSIM_SET_MASK(msgHdr.pres, GTP_MSG_HDR_SEQ_PRES);
    pGtpMsg->setMsgHdr(&msgHdr);

    GtpMsgType_t msgType = pGtpMsg->type();
    if (GTPC_MSG_CS_REQ == msgType)
    {
        pGtpMsg->setImsi(&m_imsiKey);

        RETVAL ret = pGtpMsg->setSenderFteid(
            pPdn->pCTun->m_locTeid, &pPdn->pCTun->m_localEp.ipAddr);
        if (ROK != ret)
        {
            LOG_ERROR("Encoding of sender Fteid Failed");
            throw ret;
        }
    }
    else if (GTPC_MSG_CS_RSP == msgType)
    {
        RETVAL ret = pGtpMsg->setSenderFteid(
            pPdn->pCTun->m_locTeid, &pPdn->pCTun->m_localEp.ipAddr);
        if (ROK != ret)
        {
            LOG_ERROR("Encoding of sender Fteid Failed");
            throw ret;
        }
    }

    /* Modify the GTP-U TEID in all the bearers */
    U32 bearerCnt = pGtpMsg->getIeCount(GTP_IE_BEARER_CNTXT, 0);
    for (U32 i = 1; i <= bearerCnt; i++)
    {
        GtpIe *           pIe = pGtpMsg->getIe(GTP_IE_BEARER_CNTXT, 0, i);
        GtpBearerContext *bearerCntxt = dynamic_cast<GtpBearerContext *>(pIe);
        GtpBearer *       pBearer     = this->getBearer(bearerCntxt->getEbi());
        bearerCntxt->setGtpuTeid(pBearer->localTeid(), 0);
    }

    MEMSET(buf, 0, GTP_MSG_BUF_LEN);
    pGtpMsg->encode(buf, &len);

    BUFFER_CPY(pGtpBuf, buf, len);

    LOG_EXITVOID();
}

RETVAL UeSession::handleDeadCall(VOID *arg)
{
    LOG_ENTERFN();

    RETVAL ret = ROK;

    if (NULL == arg)
    {
        /* Session run invoked because of deadcall timer expiry */
        if (m_currRunTime >= m_wakeTime)
        {
            ret = ROK_OVER;
            Stats::decStats(GSIM_STAT_NUM_DEADCALLS);
        }
    }
    else
    {
        /* A retransmitted message is received for a session whose scenario
         * is already completed
         */
        UdpData_t *data = (UdpData_t *)arg;
        GtpMsg     rcvdMsg(&data->buf);

        if (isPrevProcReq(&rcvdMsg))
        {
            /* resend the request response */
            Buffer *buf = new Buffer(m_prevProcCache.sentMsg->buf);
            sendMsg(m_prevProcCache.sentMsg->connId,
                &m_prevProcCache.sentMsg->peerEp, buf);
            (*m_prevProcItr)->m_initial->m_numRcvRetrans++;
            (*m_prevProcItr)->m_trigMsg->m_numSndRetrans++;
        }
        else if (isPrevProcRsp(&rcvdMsg))
        {
            (*m_prevProcItr)->m_trigMsg->m_numRcvRetrans++;
        }
        else
        {
            (*m_prevProcItr)->m_initial->m_numUnexp++;
        }

        m_wakeTime = m_currRunTime + (m_wakeTime - m_lastRunTime);
        pause();
        delete data;
    }

    LOG_EXITFN(ret);
}

GtpBearer *UeSession::getBearer(GtpEbi_t ebi)
{
    LOG_ENTERFN();

    GtpBearer *pBearer = m_bearerVec[GTP_BEARER_INDEX(ebi)];

    LOG_EXITFN(pBearer);
}

GtpcPdnLst *UeSession::getPdnList()
{
    return &m_pdnLst;
}

/**
 * @brief Contructor
 *
 * @param pPdn
 * @param ebi
 */
GtpBearer::GtpBearer(GtpcPdn *pPdn, GtpEbi_t ebi)
{
    m_pPdn  = pPdn;
    m_ebi   = ebi;
    m_pUTun = new GtpuTun;
}

/**
 * @brief Destructor
 */
GtpBearer::~GtpBearer()
{
    delete m_pUTun;
}

PUBLIC VOID cleanupUeSessions()
{
    UeSessionMapItr ueItr = s_ueSessionMap.begin();
    for (; ueItr != s_ueSessionMap.end(); ueItr++)
    {
        delete ueItr->second;
    }
}

PUBLIC GtpcTun *getS11S4CTun(UeSession *pUeSession)
{
    LOG_ENTERFN();

    GtpcPdnLst *pPdnLst = pUeSession->getPdnList();
    GtpcTun *   pCTun   = NULL;

    for (GtpcPdnLstItr b = pPdnLst->begin(); b != pPdnLst->end(); b++)
    {
        pCTun = (*b)->pCTun;
    }

    LOG_EXITFN(pCTun);
}

GtpcTun *UeSession::createCTun(GtpcPdn *pPdn)
{
    LOG_ENTERFN();

    GtpcTun *pCTun = NULL;

    try
    {
        GtpIfType_t ifType = m_pScn->ifType();
        if (GTP_IF_S11_C_MME == ifType)
        {
            pCTun = getS11S4CTun(pPdn->pUeSession);
            if (NULL != pCTun)
            {
                /* S11/S4 tunnel already exists, increase the reference count */
                pCTun->m_refCount++;
            }
            else
            {
                /* This is the first C tun over S11/S4 interface, so create
                 * new C tunnel
                 */
                pCTun               = new GtpcTun();
                pCTun->m_pPdn       = pPdn;
                pCTun->m_pUeSession = pPdn->pUeSession;
            }
        }
        else
        {
            pCTun               = new GtpcTun();
            pCTun->m_pPdn       = pPdn;
            pCTun->m_pUeSession = pPdn->pUeSession;
        }
    }
    catch (exception &e)
    {
        throw ERR_CTUN_CREATION;
    }

    LOG_EXITFN(pCTun);
}

VOID UeSession::handleCompletedTask()
{
    LOG_ENTERFN();

    /* finished processing all messages in the scenario, delete task */
    LOG_DEBUG("Scenario end for UE, IMSI [%x%x%x%x%x%x%x%x]", m_imsiKey.val[0],
        m_imsiKey.val[1], m_imsiKey.val[2], m_imsiKey.val[3], m_imsiKey.val[4],
        m_imsiKey.val[5], m_imsiKey.val[6], m_imsiKey.val[7]);

    Stats::incStats(GSIM_STAT_NUM_SESSIONS_SUCC);
    Stats::decStats(GSIM_STAT_NUM_SESSIONS);

    /* the scenario for this UE session is complete, wait for deal-call
     * timer expiry to cleanup the sessions. This is required to handle
     * any delayed or retransmitted response or request messages
     */
    Stats::incStats(GSIM_STAT_NUM_DEADCALLS);
    GSIM_SET_MASK(m_bitmask, GSIM_UE_SSN_SCN_COMPLETE);
    m_wakeTime = m_lastRunTime + Config::getInstance()->getDeadCallWait();
    pause();

    LOG_EXITVOID();
}
