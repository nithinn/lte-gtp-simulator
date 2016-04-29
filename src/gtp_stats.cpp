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

#include <iostream>

using namespace std;
#include <vector>
#include <list>

#include "pugixml.hpp"
using namespace pugi;

#include "types.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "procedure.hpp"
#include "task.hpp"
#include "scenario.hpp"
#include "gtp_stats.hpp"

#define PRINT_DOUBLE_LINE(_out) \
do \
{\
   fprintf(_out, "==============================================================================\n");\
} while (0)

#define PRINT_LINE(_out) \
do \
{\
   fprintf(_out, "------------------------------------------------------------------------------\n");\
} while (0)

#define PRINT_DIRECTION(_out, _act) \
do \
{\
   if (MSG_ACTION_SEND == _act)\
      fprintf(_out, "  --->  ");\
   else\
      fprintf(_out, "  <---  ");\
} while (0)

// GTP Statistics counters
static Counter  s_gsimStats[GSIM_STAT_MAX];
static Stats   *s_pStats = NULL;

/**
 * Constructor
 */
Stats::Stats()
{
   // by default stats will be displayed on stdout, unless
   // specified by a file using the command line option --display-output
   for (Counter i = 0; i < GSIM_STAT_MAX; i++)
   {
      s_gsimStats[i] = 0;
   }
}


/**
 * @brief
 *    Creates the singleton instance of Stats object
 *
 * @return
 *    returns Stats object pointer
 */
Stats* Stats::getInstance()
{
   if (NULL == s_pStats)
   {
      try
      {
         s_pStats = new Stats;
         return s_pStats;
      }
      catch (S32 e)
      {
         return (Stats *)NULL;
      }
   }

   return s_pStats;
}



/**
 * @brief
 *    retrieves the GTP statistics
 *
 * @param statsType
 *    type of GTP statistics
 *
 * @return
 *    GTP statistics counter value
 */
Counter Stats::getStats(GtpStat_t  statsType)
{
   return s_gsimStats[statsType];
}

/**
 * @brief
 *    Increments GTP statistics
 *
 * @param statsType
 *    type of GTP statistics
 *
 * @return
 *    GTP statistics counter value
 */
VOID Stats::incStats(GtpStat_t statsType)
{
   ++s_gsimStats[statsType];
}

VOID Stats::decStats(GtpStat_t statsType)
{
   --s_gsimStats[statsType];
}



