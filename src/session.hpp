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
   BOOL  operator() (const GtpImsiKey& left, const GtpImsiKey& right) const
   {
      return (BOOL)MEMCMP(left.val, right.val, left.len);
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

#if 0
class GtpcTun
{
   public:
      GtpcTun(GtpcPdn *pPdn, GtpTeid_t);

      GtpTeid_t   m_locTeid;
      GtpTeid_t   m_remTeid;

      IPEndPoint  m_localEp;
      IPEndPoint  m_peerEp;

      UeSession   *m_pUeSession;
      GtpcPdn     *m_pPdn;

      U32         m_refCount; /* On S11 and S4 interface the multiple PDNs
                               * will point to same GtpcTun object, while
                               * while assigning the cTun to a particular PDN
                               * and when deleting check the refcount
                               */
   friend class UeSession;
};
#endif

/**
 * @struct GTP-C message received from peer or sent to the peer
 */
struct GtpcNwData
{
   Buffer         gtpcMsgBuf;
   IPEndPoint     peerEp;
   TransConnId    connId;

   ~GtpcNwData()
   {
      delete[] gtpcMsgBuf.pVal;
   }
};

/**< stores the Message sent and received in a send and receive MsgTsk
 * respectively
 */
typedef vector<GtpcNwData *>  GtpcNwDataVec;

class UeSession: public Task
{
   public:
      UeSession(Scenario *pScn, GtpImsiKey);
      ~UeSession();

      BOOL              run();  
      Time_t            wake();
      static UeSession* createUeSession(GtpImsiKey);
      static UeSession* getUeSession(GtpTeid_t);
      static UeSession* getUeSession(GtpImsiKey);
      static GtpcTun*   getCTun(GtpTeid_t teid);
      VOID              deleteTunnel(GtpTeid_t teid);
      GtpcPdn*          createPdn();
      VOID              deletePdn();
      GtpcPdnLst*       getPdnList();
      VOID              storeRcvdMsg(Buffer*, TransConnId, IPEndPoint);

   private:
#define GSIM_UE_SSN_WAITING_FOR_RSP       (1 << 0)
#define GSIM_UE_SSN_GTPC_MSG_RCVD         (1 << 1)
#define GSIM_UE_SSN_SEQ_NUM_SET           (1 << 2)
      U32               m_bitmask;
      GtpImsiKey        m_imsiKey;
      U32               m_n3req;
      Time_t            m_t3time;
      Time_t            m_lastRunTime;
      U32               m_retryCnt;
      U32               m_sessionId;
      Time_t            m_wait;
      BOOL              m_isWaiting;
      IPEndPoint        m_peerEp;
      EpcNodeType_E     m_nodeType; 
      GtpcPdnLst        m_pdnLst;     
      GtpBearerLst      m_bearerLst;
      GtpSeqNumber_t    m_currSeqNum;
      U32               m_currTaskIndx;
      U32               m_lastReqIndx;
      GtpcNwDataVec     m_nwDataArr;
      GtpcNwData        *m_pRcvdNwData;
      MsgTask           *m_pCurrTask;
      GtpcPdn           *m_pCurrPdn;
      Scenario          *m_pScn;

      VOID              storeGtpcOutMsg(GtpcPdn *pPdn, GtpMsg  *pGtpMsg);
      VOID              encGtpcOutMsg(GtpcPdn *pPdn, GtpMsg *pGtpMsg,\
                              Buffer *pBuf);
      VOID              decAndStoreGtpcIncMsg(GtpcPdn*, GtpMsg*,\
                              const IPEndPoint*);
      GtpBearer*        getBearer(GtpEbi_t ebi);
      RETVAL            procSend();
      RETVAL            procWait();
      RETVAL            procRecv();
      RETVAL            procIncReqMsg(GtpMsg *pGtpMsg);
      RETVAL            procIncRspMsg(GtpMsg *pGtpMsg);
      VOID              deleteUeSession();
      GtpcTun*          createCTun(GtpcPdn *pPdn);
};

EXTERN UeSession* getUeSession(const U8* pImsi);
EXTERN VOID       cleanupUeSessions();
EXTERN GtpcTun*   getS11S4CTun(UeSession *pUeSession);

#endif
