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

#ifndef __TASK_HPP__
#define __TASK_HPP__

class Task;
typedef std::list<Task *>     TaskList;
typedef TaskList::iterator    TaskListItr;
typedef U32                   TaskId_t;

typedef enum
{
   TASK_STATE_INVALID,
   TASK_STATE_RUNNING,
   TASK_STATE_PAUSED,
   TASK_STATE_STOPPED,
   TASK_STATE_MAX
} TaskState_t;

class TaskMgr
{
   public:
      static TaskList* getRunningTasks();
      static TaskList* getPausedTasks();
      static TaskList* getAllTasks();
      static VOID resumePausedTasks();
      static VOID deleteStoppedTasks();
      static VOID deleteAllTasks();
};


/* All following processing in GTP simulator are treated as tasks:
 * Display - periodic display of statistics
 * Load generation
 */
class Task
{
   public:
      Task();

      virtual ~Task() {};

      virtual BOOL run() = 0;

      virtual VOID abort();

      virtual VOID stop();

   protected:
      TaskId_t        m_id;

      /* Put us to sleep (we must be running). */
      VOID pauseTask();

      /* Wake this up, if we are not already awake. */
      VOID resumeTask();

      /* When should this Task wake up? */
      virtual Time_t wake() = 0;

      /* Is this Task paused or running? */
      BOOL running;

   private:

      TaskListItr       m_allTaskItr;
      TaskListItr       m_runningTaskItr;
      TaskListItr       m_pausedTaskItr;
      TaskState_t       m_taskState;
      
      friend class TaskMgr;
};

#endif
