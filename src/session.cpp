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
#include "message.hpp"
#include "gtp_stats.hpp"
#include "scenario.hpp"
#include "tunnel.hpp"
#include "session.hpp"

static UeSessionMap  s_ueSessionMap;
static U32           g_sessionId = 0;
static U32           s_seqNum = 0;

#define UE_SSN_SET_NEXT_TASK(_ueSsn)  ((_ueSsn)->m_currTaskIndx++)  
#define IS_SCN_COMPLETED() (m_currTaskIndx == m_pScn->m_msgVec.size())

PRIVATE U32 generateSeqNum()
{
   return ++s_seqNum;
}

/**
 * @brief
 *    Constructor
 *
 * @param pScn
 */
UeSession::UeSession(Scenario *pScn, GtpImsiKey imsi)
{
   m_pScn = pScn;
   m_nwDataArr = GtpcNwDataVec(pScn->m_msgVec.size());
   m_n3req = Config::getInstance()->getN3Requests();
   m_retryCnt = 0;
   m_t3time = Config::getInstance()->getT3Timer();
   m_sessionId = ++g_sessionId;
   m_nodeType = Config::getInstance()->getNodeType();
   m_peerEp.ipAddr = *(Config::getInstance()->getRemoteIpAddr());
   m_peerEp.port = Config::getInstance()->getRemoteGtpcPort();
   m_isWaiting = FALSE;
   m_currTaskIndx = 0;
   m_currSeqNum = 0;
   m_bitmask = 0;
   m_imsiKey = imsi;

   LOG_DEBUG("Creating UE Session [%d]", m_sessionId);
}


/**
 * @brief
 *    Destructor for UeSession
 */
UeSession::~UeSession()
{
   s_ueSessionMap.erase(m_imsiKey);

   for (U32 i = 0; i < m_nwDataArr.size(); i++)
   {
      delete m_nwDataArr[i];
      m_nwDataArr[i] = NULL;
   }

   for (GtpcPdnLstItr pPdn = m_pdnLst.begin(); pPdn != m_pdnLst.end(); pPdn++)
   {
      /* delete the c-plane tunnels */
      GtpcTun *pCTun = (*pPdn)->pCTun;
      if (NULL != pCTun)
      {
         deleteCTun(pCTun);
         pCTun = NULL;
      }
      
      GtpBearerLstItr bearer;
      for (bearer = m_bearerLst.begin(); bearer != m_bearerLst.end(); bearer++)
      {
         if (GSIM_CHK_BEARER_MASK((*pPdn)->bearerMask, (*bearer)->getEbi()))
         {
            GSIM_UNSET_BEARER_MASK((*pPdn)->bearerMask, (*bearer)->getEbi());
            delete *bearer;
         }
      }

      delete *pPdn;
   }

   LOG_DEBUG("Deleting UE Session [%d]", m_sessionId);
}

BOOL UeSession::run()
{
   RETVAL         ret = ROK;

   LOG_TRACE("Running UeSession [%d]", m_sessionId);

   m_lastRunTime = getMilliSeconds();
   resumeTask();

   if (IS_SCN_COMPLETED())
   {
      /* finished processing all messages in the scenario, delete task */
      LOG_DEBUG("Scenario end for UE, IMSI [%x%x%x%x%x%x%x%x]",\
            m_imsiKey.val[0], m_imsiKey.val[1], m_imsiKey.val[2],\
            m_imsiKey.val[3], m_imsiKey.val[4], m_imsiKey.val[5],\
            m_imsiKey.val[6], m_imsiKey.val[7]);

      Stats::incStats(GSIM_STAT_NUM_SESSIONS_SUCC);
      Stats::decStats(GSIM_STAT_NUM_SESSIONS);
      LOG_EXITFN(FALSE);
   }

   m_pCurrTask = m_pScn->m_msgVec[m_currTaskIndx];
   MsgTaskType_t taskType = m_pCurrTask->type();

   switch (taskType)
   {
      case MSG_TASK_SEND:
      {
         LOG_TRACE("Processing Send() Task");
         ret = procSend();
         break;
      }

      case MSG_TASK_WAIT:
      {
         LOG_TRACE("Processing Wait() Task");
         ret = procWait();
         break;
      }

      case MSG_TASK_RECV:
      {
         LOG_TRACE("Processing Recv() Task");
         ret = procRecv();
         break;
      }

      default:
      {
         ret = RFAILED;
         LOG_ERROR("Unhandled Task Type");
         break;
      }
   }
   
   if (ROK != ret)
   {
      LOG_ERROR("Terminating UE Session RUN");
      LOG_EXITFN(FALSE);
   }
   
   updateWakeupTime(taskType);
   UE_SSN_SET_NEXT_TASK(this);
   pauseTask();

   return TRUE;
}

VOID UeSession::updateWakeupTime(MsgTaskType_t taskType)
{
   LOG_ENTERFN();

   m_wakeTime = 0;

   switch (taskType)
   {
      case MSG_TASK_WAIT:
      {
         m_wakeTime = m_lastRunTime + m_wait;
         break;
      }
      case MSG_TASK_RECV:
      {
         if (GSIM_CHK_MASK(this->m_bitmask, GSIM_UE_SSN_WAITING_FOR_RSP))
         {
            // if response is not received within T3 timer expiry
            // wakeup and retransmit request message
            m_wakeTime = m_lastRunTime + m_t3time;
         }

         break;
      }
      case MSG_TASK_SEND:
      {
         m_wakeTime = m_lastRunTime + m_t3time;
         break;
      }
      default:
      {
         break;
      }
   }

   LOG_EXITVOID();
}

RETVAL UeSession::procSend()
{
   LOG_ENTERFN();

   GtpcPdn  *pPdn = NULL;

   try
   {
      GtpMsgType_t msgType = m_pCurrTask->getGtpMsg()->type();

      if (GTPC_MSG_CS_REQ == msgType)
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
      storeGtpcOutMsg(pPdn, m_pCurrTask->getGtpMsg());

      LOG_DEBUG("Encoding OUT Message");
      GtpcNwData     *pNwData = new GtpcNwData;
      encGtpcOutMsg(pPdn, m_pCurrTask->getGtpMsg(), &pNwData->gtpcMsgBuf);

      GtpMsgCategory_t msgCat = gtpGetMsgCategory(msgType);
      if (GTP_MSG_CAT_INITIAL == msgCat)
      {
         /* initial message, send the message over default send socket */
         pNwData->connId = 0;
         pNwData->peerEp = m_peerEp;
      }
      else
      {
         /* send the response/triggered message over the same socket
          * over which the request/command is received
          */
         pNwData->connId = m_nwDataArr[m_lastReqIndx]->connId;
         pNwData->peerEp = pPdn->pCTun->m_peerEp;
      }

      LOG_DEBUG("Sending GTPC Message [%s]", gtpGetMsgName(msgType));
      sendMsg(pNwData->connId, &pNwData->peerEp, &pNwData->gtpcMsgBuf);
      m_nwDataArr[m_currTaskIndx] = pNwData;
      if (GTP_MSG_CAT_INITIAL == msgCat)
      {
         m_lastReqIndx = m_currTaskIndx;
         GSIM_SET_MASK(this->m_bitmask, GSIM_UE_SSN_WAITING_FOR_RSP);
      }

      m_pCurrTask->m_numSnd++;
   }
   catch (ErrCodeEn &e)
   {
      LOG_ERROR("GTP send() processing, error [%d]", e);
      return e;
   }

   LOG_EXITFN(ROK);
}

RETVAL UeSession::procRecv()
{
   LOG_ENTERFN();

   RETVAL ret = ROK;

   if (GSIM_CHK_MASK(this->m_bitmask, GSIM_UE_SSN_GTPC_MSG_RCVD))
   {
      /* Receive task is run because a GTPC message is received for this
       * session
       */
      GtpMsg gtpMsg(&m_pRcvdNwData->gtpcMsgBuf);
      GtpMsgCategory_t msgCat = gtpGetMsgCategory(gtpMsg.type());

      if (msgCat == GTP_MSG_CAT_INITIAL)
      {
         LOG_DEBUG("Processing Incoming Request message");
         ret = procIncReqMsg(&gtpMsg);
         if (ROK != ret)
         {
            LOG_ERROR("Processing Incoming Request Message, Error [%d]", ret);
         }

      }
      else if (msgCat == GTP_MSG_CAT_TRIG_RSP)
      {
         LOG_DEBUG("Processing Incoming Response message");
         ret = procIncRspMsg(&gtpMsg);
         if (ROK != ret)
         {
            LOG_ERROR("Processing Incoming Response Message, Error [%d]", ret);
         }

         /* only message to be sent will be stored in m_nwDataArr
          * received response messages is deleted after processing
          */
         GSIM_UNSET_MASK(this->m_bitmask, GSIM_UE_SSN_WAITING_FOR_RSP);
         delete m_pRcvdNwData;
         m_pRcvdNwData = NULL;
      }
      
      GSIM_UNSET_MASK(this->m_bitmask, GSIM_UE_SSN_GTPC_MSG_RCVD);
   }
   else
   {
      /* Recived task is run because GTP-C message request timedout
       * waiting for a response, retransmit the request message
       */
      if (m_retryCnt >= m_n3req)
      {
         LOG_DEBUG("Maximum Retries reached");

         m_lastReqIndx = 0;
         GSIM_UNSET_MASK(this->m_bitmask, GSIM_UE_SSN_WAITING_FOR_RSP);
         m_pScn->m_msgVec[m_lastReqIndx]->m_numTimeOut++;
         Stats::incStats(GSIM_STAT_NUM_SESSIONS_FAIL);
         ret = ERR_MAX_RETRY_EXCEEDED;
      }

      if (GSIM_CHK_MASK(this->m_bitmask, GSIM_UE_SSN_WAITING_FOR_RSP))
      {
         /* we have already processed this message, the task is run again
          * after retransmission timeout expiry
          */
         LOG_DEBUG("Retransmissing GTP Message");
         sendMsg(m_nwDataArr[m_lastReqIndx]->connId,\
               &m_nwDataArr[m_lastReqIndx]->peerEp,\
               &m_nwDataArr[m_lastReqIndx]->gtpcMsgBuf);
         m_pScn->m_msgVec[m_lastReqIndx]->m_numSndRetrans++;
         m_retryCnt++;
         ret = ROK;
      }
   }

   LOG_EXITFN(ret);
}

RETVAL UeSession::procIncReqMsg(GtpMsg *pGtpMsg)
{
   LOG_ENTERFN();

   GtpMsg *pExpctdGtpMsg = m_pCurrTask->getGtpMsg();

   GtpMsgType_t rcvdMsgType = pGtpMsg->type();
   LOG_DEBUG("Received Message: [%s]", gtpGetMsgName(rcvdMsgType));
   if (rcvdMsgType == pExpctdGtpMsg->type())
   {
      GtpSeqNumber_t rcvdSeqNum = pGtpMsg->seqNumber();
      if ((GSIM_CHK_MASK(this->m_bitmask, GSIM_UE_SSN_SEQ_NUM_SET)) && \
          (m_currSeqNum >= rcvdSeqNum))
      {
         m_pCurrTask->m_numRcvRetrans++;

         /* TODO: compare req type, ip, port, teid and seq number
          * of this req message with preivously received messgaes
          * if there is a match send the corresponding response
          */

         delete m_pRcvdNwData;
         m_pRcvdNwData = NULL;
         LOG_DEBUG("Retransmitted GTPC Messsage Received");
         LOG_EXITFN(ERR_RETRANS_GTPC_MSG_RCVD);
      }
      else
      {
         m_lastReqIndx = m_currTaskIndx;
         m_nwDataArr[m_currTaskIndx] = m_pRcvdNwData;
         m_pCurrTask->m_numRcv++;
         m_currSeqNum = rcvdSeqNum;
         GSIM_SET_MASK(this->m_bitmask, GSIM_UE_SSN_SEQ_NUM_SET);
      }
   }

   pGtpMsg->decode();
   GtpcPdn  *pPdn = NULL;
   if (GTPC_MSG_CS_REQ == rcvdMsgType)
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

   decAndStoreGtpcIncMsg(pPdn, pGtpMsg, &m_pRcvdNwData->peerEp);

   LOG_EXITFN(ROK);
}

PUBLIC RETVAL UeSession::procIncRspMsg(GtpMsg *pGtpMsg)
{
   LOG_ENTERFN();

   RETVAL   ret = ROK;
   GtpMsg   *pExpctdGtpMsg = m_pCurrTask->getGtpMsg();

   GtpMsgType_t rcvdMsgType = pGtpMsg->type();
   if (rcvdMsgType != pExpctdGtpMsg->type())
   {
      LOG_DEBUG("Unexpected Response Message Received");
      m_pCurrTask->m_numUnexp++;
      LOG_EXITFN(ERR_UNEXPECTED_GTPC_MSG_RCVD);
   }

   LOG_DEBUG("Response Message Received");
   m_pCurrTask->m_numRcv++;

   /* decode the response message and store the bearer and tunnel
    * details */
   pGtpMsg->decode();
   decAndStoreGtpcIncMsg(m_pCurrPdn, pGtpMsg, &m_pRcvdNwData->peerEp);

   LOG_EXITFN(ret);
}


RETVAL UeSession::procWait()
{
   LOG_ENTERFN();

   LOG_DEBUG("Executing Wait");

   m_isWaiting = TRUE;
   m_wait = m_pCurrTask->wait();

   LOG_EXITFN(ROK);
}

VOID UeSession::storeRcvdMsg(UdpData *rcvdData)
{
   LOG_ENTERFN();

   m_pRcvdNwData = new GtpcNwData;
   BUFFER_CPY(&m_pRcvdNwData->gtpcMsgBuf, rcvdData->buf.pVal,\
         rcvdData->buf.len);
   m_pRcvdNwData->peerEp = rcvdData->peerEp;
   m_pRcvdNwData->connId = rcvdData->connId;
   GSIM_SET_MASK(this->m_bitmask, GSIM_UE_SSN_GTPC_MSG_RCVD);

   LOG_EXITVOID();
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
UeSession* UeSession::createUeSession
(
GtpImsiKey  imsiKey
)
{
   U8    *pImsi = imsiKey.val;

   Scenario *pScn = Scenario::getInstance();
   UeSession *pUeSsn = new UeSession(pScn, imsiKey);
   s_ueSessionMap.insert(UeSessionMapPair(imsiKey, pUeSsn));

   LOG_INFO("Creating UE Session [%x%x%x%x%x%x%x%x]",\
         pImsi[0], pImsi[1], pImsi[2], pImsi[3], pImsi[4], pImsi[5],\
         pImsi[6], pImsi[7]);

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
UeSession* UeSession::getUeSession(GtpTeid_t teid)
{
   LOG_ENTERFN();

   UeSession   *pUeSession = NULL;
   GtpcTun     *pCTun = findCTun(teid);

   if (NULL != pCTun)
   {
      pUeSession = pCTun->m_pUeSession;
   }

   LOG_EXITFN(pUeSession);
}

UeSession* UeSession::getUeSession(GtpImsiKey imsiKey)
{
   LOG_ENTERFN();

   UeSession *pUeSession = NULL;

   UeSessionMapItr   itr = s_ueSessionMap.find(imsiKey);
   if (itr != s_ueSessionMap.end())
   {
      pUeSession = itr->second;
   }

   LOG_EXITFN(pUeSession);
}

GtpcPdn* UeSession::createPdn()
{
   LOG_ENTERFN();

   GtpcPdn *pPdn = NULL;

   try
   {
      pPdn = new GtpcPdn;
      pPdn->pUeSession = this;

      LOG_DEBUG("Creating GTP-C Tunnel");
      pPdn->pCTun = createCTun(pPdn);
   }
   catch (exception &e)
   {
      LOG_FATAL("Memory allocation GtpcPdn");
      throw ERR_PDN_CREATION;
   }

   LOG_EXITFN(pPdn);
}

VOID UeSession::storeGtpcOutMsg\
(
GtpcPdn     *pPdn,
GtpMsg      *pGtpMsg
)
{
   LOG_ENTERFN();

   if (pGtpMsg->type() == GTPC_MSG_CS_REQ)
   {
      U32 bearerCnt = pGtpMsg->getIeCount(GTP_IE_BEARER_CNTXT, 0);
      for (U32 i = 1; i <= bearerCnt; i++)
      {
         GtpIe *pIe = pGtpMsg->getIe(GTP_IE_BEARER_CNTXT, 0, i);
         GtpBearerContext *pBearerCntxt = dynamic_cast<GtpBearerContext *>(pIe);

         GtpEbi_t ebi = pBearerCntxt->getEbi();

         GtpBearer *pBearer = new GtpBearer(pPdn, ebi);
         GSIM_SET_BEARER_MASK(pPdn->bearerMask, ebi);
         m_bearerLst.push_back(pBearer);
      }
   }

   LOG_EXITVOID();
}

VOID UeSession::decAndStoreGtpcIncMsg
(
GtpcPdn           *pPdn,
GtpMsg            *pGtpMsg,
const IPEndPoint  *pPeerEp
)
{
   LOG_ENTERFN();

   GtpMsgType_t rcvdMsgTye = pGtpMsg->type();
   if (rcvdMsgTye == GTPC_MSG_CS_REQ || rcvdMsgTye == GTPC_MSG_CS_RSP)
   {
      GtpFteid *pFteid = dynamic_cast<GtpFteid *>\
            (pGtpMsg->getIe(GTP_IE_FTEID, 0, 1));
      pPdn->pCTun->m_remTeid = pFteid->getTeid();
   }

   pPdn->pCTun->m_peerEp.ipAddr.ipAddrType = IP_ADDR_TYPE_V4;
   pPdn->pCTun->m_peerEp.port = pPeerEp->port;
   pPdn->pCTun->m_peerEp.ipAddr = pPeerEp->ipAddr;

   if (pGtpMsg->type() == GTPC_MSG_CS_REQ)
   {
      U32 bearerCnt = pGtpMsg->getIeCount(GTP_IE_BEARER_CNTXT, 0);
      for (U32 i = 1; i <= bearerCnt; i++)
      {
         GtpIe *pIe = pGtpMsg->getIe(GTP_IE_BEARER_CNTXT, 0, i);
         GtpBearerContext *pBearerCntxt = dynamic_cast<GtpBearerContext *>(pIe);

         GtpEbi_t ebi = pBearerCntxt->getEbi();

         GtpBearer *pBearer = new GtpBearer(pPdn, ebi);
         GSIM_SET_BEARER_MASK(pPdn->bearerMask, ebi);
         pBearer->setDfltBearer(TRUE);
         m_bearerLst.push_back(pBearer);
      }
   }

   LOG_EXITVOID();
}

VOID UeSession::encGtpcOutMsg(GtpcPdn *pPdn, GtpMsg *pGtpMsg,\
   Buffer *pGtpBuf)
{
   LOG_ENTERFN();

   U8  buf[GTP_MSG_BUF_LEN];
   U32 len = 0;

   /* Modify the header parameters dynamically */
   GtpMsgHdr msgHdr;
   msgHdr.teid = pPdn->pCTun->m_remTeid;
   msgHdr.seqN = generateSeqNum();
   GSIM_SET_MASK(msgHdr.pres, GTP_MSG_HDR_TEID_PRES);
   GSIM_SET_MASK(msgHdr.pres, GTP_MSG_HDR_SEQ_PRES);
   pGtpMsg->setMsgHdr(&msgHdr);

   GtpMsgType_t msgType = pGtpMsg->type();
   if (GTPC_MSG_CS_REQ == msgType)
   {
      pGtpMsg->setImsi(&m_imsiKey);

      RETVAL ret = pGtpMsg->setSenderFteid(pPdn->pCTun->m_locTeid, \
            &pPdn->pCTun->m_localEp.ipAddr);
      if (ROK != ret)
      {
         LOG_ERROR("Encoding of sender Fteid Failed");
         throw ret;
      }
   }

   if (GTPC_MSG_CS_RSP == msgType)
   {
      RETVAL ret = pGtpMsg->setSenderFteid(pPdn->pCTun->m_locTeid, \
            &pPdn->pCTun->m_localEp.ipAddr);
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
      GtpIe *pIe = pGtpMsg->getIe(GTP_IE_BEARER_CNTXT, 0, i);
      GtpBearerContext *pBearerCntxt = dynamic_cast<GtpBearerContext *>(pIe);

      GtpBearer *pBearer = this->getBearer(pBearerCntxt->getEbi());
      pBearerCntxt->setGtpuTeid(pBearer->localTeid(), 0);
   }

   MEMSET(buf, 0, GTP_MSG_BUF_LEN);
   pGtpMsg->encode(buf, &len);

   BUFFER_CPY(pGtpBuf, buf, len);

   LOG_EXITVOID();
}

GtpBearer* UeSession::getBearer(GtpEbi_t ebi)
{
   LOG_ENTERFN();

   GtpBearer *pBearer = NULL;
   for (GtpBearerLstItr b = m_bearerLst.begin(); b != m_bearerLst.end(); b++)
   {
      if ((*b)->getEbi() == ebi)
      {
         pBearer = *b;
         break;
      }
   }

   LOG_EXITFN(pBearer);
}

GtpcPdnLst* UeSession::getPdnList()
{
   return &m_pdnLst;
}

/**
 * @brief Contructor
 *
 * @param pPdn
 * @param ebi
 */
GtpBearer::GtpBearer(GtpcPdn  *pPdn, GtpEbi_t ebi)
{
   m_pPdn = pPdn;
   m_ebi = ebi;
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

PUBLIC GtpcTun* getS11S4CTun(UeSession *pUeSession)
{
   LOG_ENTERFN();

   GtpcPdnLst  *pPdnLst = pUeSession->getPdnList();
   GtpcTun     *pCTun = NULL;
   
   for (GtpcPdnLstItr b = pPdnLst->begin(); b != pPdnLst->end(); b++)
   {
      pCTun = (*b)->pCTun;
   }

   LOG_EXITFN(pCTun);
}

GtpcTun* UeSession::createCTun(GtpcPdn *pPdn)
{
   LOG_ENTERFN();

   GtpcTun     *pCTun = NULL;

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
            pCTun = new GtpcTun();
            pCTun->m_pPdn = pPdn;
            pCTun->m_pUeSession = pPdn->pUeSession;
         }
      }
      else
      {
         pCTun = new GtpcTun();  
         pCTun->m_pPdn = pPdn;
         pCTun->m_pUeSession = pPdn->pUeSession;
      }
   }
   catch (exception &e)
   {
      throw ERR_CTUN_CREATION;
   }

   LOG_EXITFN(pCTun);
}
