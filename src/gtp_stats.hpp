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

// Maintains the statistics of all GTP messages sent or received
// and session based statistics, unexpected messages received, errors etc

#ifndef _GTP_STATS_HPP_
#define _GTP_STATS_HPP_

/**
 * Counter for GTP message type statistics
 * GTP sessions
 */
typedef enum
{
   GSIM_STAT_INV,

   GSIM_STAT_SESSION_COUNTERS,
   GSIM_STAT_NUM_SESSIONS_CREATED,
   GSIM_STAT_NUM_SESSIONS,
   GSIM_STAT_NUM_SESSIONS_SUCC,
   GSIM_STAT_NUM_SESSIONS_FAIL,
   GSIM_STAT_UNEXCEPTED_MSG_RECD,

   GSIM_STAT_MAX
} GtpStat_t;

/**
 * Statistics Class
 * Singleton instance of this class is created 
 * Maintains statistics of all GTP messages, session statistics etc
 * Display Statistics in a file or terminal-screen
 */
class Stats
{
   public:

   void static incStats(GtpStat_t   statType);
   void static decStats(GtpStat_t   statType);

   /**
    * Get the GTP statistics counter values
    */
   Counter static getStats(GtpStat_t statType);

   /**
    * Destructor
    */
   ~Stats();

   /**
    * Initializes with message types involved in a scenario
    */
   VOID init(JobSequence *pMsgVec);

   /**
    * Interface to get singleton Instance
    */
   static Stats* getInstance();

   private:

      /**
       * Constructor
       */
      Stats();
};


#endif /* _GTP_STATS_HPP_ */
