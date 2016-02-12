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

#include <iostream>
#include <vector>
#include <cstdlib>

using std::vector;

#include "types.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "timer.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"
#include "gtp_types.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "message.hpp"
#include "gtp_stats.hpp"
#include "sim_cfg.hpp"
#include "transport.hpp"
#include "task.hpp"
#include "traffic.hpp"
#include "keyboard.hpp"
#include "display.hpp"
#include "scenario.hpp"
#include "sim.hpp"

EXTERN VOID cleanupUeSessions();
class Simulator*  Simulator::pSim = NULL;

Simulator* Simulator::getInstance()
{
    if (NULL == pSim)
    {
        try
        {
            pSim = new Simulator();
            return pSim;
        }
        catch (S32 e)
        {
            return (Simulator *)NULL;
        }
    }
    else
    {
        return pSim;
    }
}

// Constructor
Simulator::Simulator()
{
   m_pScn = NULL;
}

// destructor 
Simulator::~Simulator()
{
   delete m_pScn;
}

VOID Simulator::run()
{
   LOG_ENTERFN();

   m_pScn = Scenario::getInstance();
   m_pScn->init(Config::getInstance()->getScnFile());

   // Initialing the Display task to display session statistics on terminal
   Display *pDisp = Display::getInstance();
   pDisp->init();

   /* Creates UDP sockets for listing of gtp messages */
   LOG_DEBUG("Initializing Transport connections");
   if (ROK != initTransport())
   {
      LOG_FATAL("Initializing Transport connections");
      LOG_EXITVOID();
   }

   // Initialing the Keyboard to process user inputs
   Keyboard *pKb = Keyboard::getInstance();
   pKb->init();

   if (SCN_TYPE_INITIATING == m_pScn->getScnType())
   {
      TrafficTask *pTTask = new TrafficTask;
      if (pTTask == NULL)
      {
         LOG_ERROR("Traffic Task Init");
      }
   }

   LOG_DEBUG("Generating Signalling traffic");
   genSignallingTraffic();

   pKb->abort();
   TaskMgr::deleteAllTasks();

   LOG_EXITVOID();
}

VOID Simulator::genSignallingTraffic()
{
   LOG_ENTERFN();

   for (;;)
   {
      if (KB_KEY_SIM_QUIT == Keyboard::key)
      {
         LOG_INFO("Exiting Simulator");
         break;
      }

      if (Keyboard::key != KB_KEY_PAUSE_TRAFFIC)
      {
         TaskMgr::resumePausedTasks();
      }

      TaskList *pRunningTasks = TaskMgr::getRunningTasks();
      TaskListItr itr = pRunningTasks->begin();
      while (itr != pRunningTasks->end())
      {
         Task *t = *itr;

         // increment the iterator here, because after the task is run
         // it will be paused state which will move the task from running
         // task list to paused task list.
         itr++;

         if (FALSE == t->run())
         {
            t->stop();
         }
      }

      TaskMgr::deleteStoppedTasks();

      // read the sockets for keyboard events and gtp messages
      socketPoll(1);
   }
 
   LOG_EXITVOID();
}





