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

#ifndef __SESSION_HPP__
#define __SESSION_HPP__

class Scenario;
class UeSession;

#define GSIM_SET_BEARER_MASK(_b, _e) GSIM_SET_MASK((_b), (1 << (_e)))
#define GSIM_UNSET_BEARER_MASK(_b, _e) GSIM_UNSET_MASK((_b), (1 << (_e)))
#define GSIM_CHK_BEARER_MASK(_b, _e) GSIM_CHK_MASK((_b), (1 << (_e)))

struct CompareImsiKey
{
   bool operator() (const GtpImsiKey& left, const GtpImsiKey& right) const
   {
      return (MEMCMP(left.val, right.val, left.len) < 0);
   }
};

typedef std::map<GtpImsiKey, UeSession*, CompareImsiKey> UeSessionMap;
typedef std::pair<GtpImsiKey, UeSession*>                UeSessionMapPair;
typedef UeSessionMap::iterator                           UeSessionMapItr;

class GtpcPdn
{
   public:
      GtpcPdn()
      {
         pCTun      = NULL;
         pUeSession = NULL;
         bearerMask = 0;
      }

      GtpcTun     *pCTun;  /* control plane tunnel for this PDN connection 
                            * On S11 and S4 interface this will point to same
                            * object
                            */

      UeSession   *pUeSession;

      U32         bearerMask; /* bitmask represents a bearer
                               * for e.g. bearer-id = 6, 6th lsb will be
                               * set
                               */
};

class GtpBearer
{
   private:
      BOOL     m_isDefBearer;
      U8       m_ebi;
      GtpcPdn  *m_pPdn;
      GtpuTun  *m_pUTun;     /* not applicable for S11, S4, S10 interfaces */

   public:
      ~GtpBearer();
      GtpBearer(GtpcPdn*, GtpEbi_t);

      GtpEbi_t  getEbi() {return m_ebi;}
      GtpTeid_t localTeid() {return m_pUTun->localTeid();}
      VOID      setDfltBearer(BOOL b) {m_isDefBearer = b;}

};

typedef std::list<GtpcPdn *>        GtpcPdnLst;
typedef GtpcPdnLst::iterator        GtpcPdnLstItr;

typedef std::list<GtpBearer *>      GtpBearerLst;
typedef GtpBearerLst::iterator      GtpBearerLstItr;

typedef struct
{
   GtpMsgType_t   reqType;
   GtpSeqNumber_t seqNumber;
   U32            taskIndx;
} LastRcvdReq;

typedef struct
{
   GtpSeqNumber_t    seqNumber;
   GtpMsgType_t      reqType;
   GtpMsgType_t      rspType;
   TransConnId       connId;
   UdpData_t         *sentMsg;
   MsgTask           *procTask;
} Procedure_t;

class UeSession: public Task
{
   public:
      UeSession(Scenario *pScn, GtpImsiKey);
      ~UeSession();

      RETVAL              run(VOID *arg = NULL);  

      inline Time_t wake() { return m_wakeTime; }

      static UeSession* createUeSession(GtpImsiKey);
      static UeSession* getUeSession(GtpTeid_t);
      static UeSession* getUeSession(GtpImsiKey);
      static GtpcTun*   getCTun(GtpTeid_t teid);
      VOID              deleteTunnel(GtpTeid_t teid);
      GtpcPdn*          createPdn();
      VOID              deletePdn();
      GtpcPdnLst*       getPdnList();
      VOID              storeRcvdMsg(UdpData_t*);
      GtpImsiKey        m_imsiKey;

   private:
#define GSIM_UE_SSN_WAITING_FOR_RSP       (1 << 0)
#define GSIM_UE_SSN_SCN_COMPLETE          (1 << 1)
      U32               m_bitmask;
      U32               m_n3req;
      Time_t            m_t3time;
      Time_t            m_lastRunTime;
      U32               m_retryCnt;
      U32               m_sessionId;
      IPEndPoint        m_peerEp;
      EpcNodeType_t     m_nodeType; 
      GtpcPdnLst        m_pdnLst;     
      GtpBearerLst      m_bearerLst;
      Time_t            m_wakeTime;
      UdpData_t         *m_pSentNwData;
      UdpData_t         *m_pRcvdNwData;
      GtpcPdn           *m_pCurrPdn;
      Scenario          *m_pScn;

      U32               m_currTaskIndx;
      MsgTask           *m_currTask;
      GtpSeqNumber_t    m_currSeqNum;
      TransConnId       m_currConnId;
      GtpMsgType_t      m_currReqType;
      Procedure_t       m_prevProc;
      Time_t            m_deadCallWait;

      BOOL              isExpectedRsp(GtpMsg *rspMsg);
      BOOL              isExpectedReq(GtpMsg *rspMsg);
      BOOL              isPrevProcRsp(GtpMsg *rspMsg);
      BOOL              isPrevProcReq(GtpMsg *rspMsg);
      VOID              createBearers(GtpcPdn *pPdn, GtpMsg  *pGtpMsg,\
                              GtpInstance_t instance);
      VOID              encGtpcOutMsg(GtpcPdn *pPdn, GtpMsg *pGtpMsg,\
                              Buffer *pBuf, IPEndPoint *ep);
      VOID              decAndStoreGtpcIncMsg(GtpcPdn*, GtpMsg*,\
                              const IPEndPoint*);
      GtpBearer*        getBearer(GtpEbi_t ebi);
      RETVAL            procSend();
      RETVAL            procWait();
      RETVAL            handleRecv(UdpData_t* data);
      RETVAL            procIncReqMsg(GtpMsg *pGtpMsg, UdpData_t *rcvdData);
      RETVAL            procIncRspMsg(GtpMsg *pGtpMsg, UdpData_t *rcvdData);
      GtpcTun*          createCTun(GtpcPdn *pPdn);
      RETVAL            procOutRspMsg(GtpMsg *gtpMsg);
      RETVAL            procOutReqMsg(GtpMsg *gtpMsg);
      RETVAL            procOutReqTimeout();
      MsgTaskType_t     nextTaskType();
      RETVAL            handleDeadCall(VOID *arg);
};

EXTERN UeSession* getUeSession(const U8* pImsi);
EXTERN VOID       cleanupUeSessions();
EXTERN GtpcTun*   getS11S4CTun(UeSession *pUeSession);
PUBLIC VOID       printAllUes();

#endif
