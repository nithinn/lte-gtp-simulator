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

#include "types.hpp"
#include "logger.hpp"
#include "timer.hpp"
#include "task.hpp"

static TaskId_t   s_taskId = 0;
TaskList          g_runningTasks;
TaskList          g_allTasks;
TimeWheel         pausedTasks;

Task::Task()
{
   m_allTaskItr = g_allTasks.insert(g_allTasks.end(), this);
   m_runningTaskItr = g_runningTasks.insert(g_runningTasks.end(), this);
   m_taskState = TASK_STATE_RUNNING;
   m_id = ++s_taskId;
}

VOID Task::stop()
{
   if (TASK_STATE_RUNNING == m_taskState)
   {
      g_runningTasks.erase(m_runningTaskItr);
   }
   else if (TASK_STATE_PAUSED == m_taskState)
   {
      pausedTasks.removeTask(this);
   }

   m_taskState = TASK_STATE_STOPPED;
}

VOID Task::abort()
{
   this->stop();
   g_allTasks.erase(m_allTaskItr);
   delete this;
}

VOID Task::pause()
{
   if (TASK_STATE_RUNNING == m_taskState)
   {
      g_runningTasks.erase(m_runningTaskItr);
      m_taskState = TASK_STATE_PAUSED;
      pausedTasks.addTask(this);
   }
   else if (TASK_STATE_STOPPED == m_taskState)
   {
      m_taskState = TASK_STATE_PAUSED;
      pausedTasks.addTask(this);
   }
}

VOID Task::resumeTask()
{
   if (TASK_STATE_PAUSED == m_taskState)
   {
      pausedTasks.removeTask(this);
      m_runningTaskItr = g_runningTasks.insert(g_runningTasks.end(), this);
   }
   else if (TASK_STATE_STOPPED == m_taskState)
   {
      m_runningTaskItr = g_runningTasks.insert(g_runningTasks.end(), this);
   }

   m_taskState = TASK_STATE_RUNNING;
}

TaskList* TaskMgr::getRunningTasks()
{
   return &g_runningTasks;
}

VOID Task::addToRunQueue()
{
   m_runningTaskItr = g_runningTasks.insert(g_runningTasks.end(), this);
   m_taskState = TASK_STATE_RUNNING;
}

TaskList* TaskMgr::getAllTasks()
{
   return &g_allTasks;
}

VOID TaskMgr::resumePausedTasks()
{
#if 0
   Time_t currTime = getMilliSeconds();
   TaskList *pPausedTasks = getPausedTasks();
   TaskListItr itr = pPausedTasks->begin();

   while (itr != pPausedTasks->end())
   {
      Task *t = *itr;
      itr++;

      if (t->wake() <= currTime)
      {
         t->resumeTask();
      }
   }
#endif
   pausedTasks.resumePausedTasks();
}

VOID TaskMgr::deleteAllTasks()
{
   TaskList *pTasks = getAllTasks();
   TaskListItr itr = pTasks->begin();

   while (itr != pTasks->end())
   {
      Task *tmp = *itr;
      itr++;
      tmp->abort();
   }
}

VOID Task::recalcWheel()
{
   pausedTasks.addTask(this);
}
