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

#include <sys/time.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "types.hpp"
#include "logger.hpp"
#include "timer.hpp"

static Time_t s_clockTick = 0;

/**
 * @brief
 *    returns time in milliseconds since epoch time
 */
Time_t getMilliSeconds()
{
    struct timespec sysTime;
    static Time_t   startTime = 0;

    clock_gettime(CLOCK_MONOTONIC_COARSE, &sysTime);
    Time_t usec = (Time_t)sysTime.tv_sec * 1000000LL + sysTime.tv_nsec / 1000LL;
    Time_t msec = usec / 1000;

    if (startTime == 0)
    {
        startTime = msec - 1;
    }

    msec        = msec - startTime;
    s_clockTick = msec;
    return msec;
}

VOID getTimeStr(S8 *pStr)
{
    LOG_ENTERFN();

    struct timeval ptime;
    struct tm *    ptm;

    gettimeofday(&ptime, NULL);
    ptm = localtime(&ptime.tv_sec);

    /* Format the date and time, year, hour, minute and seconds */
    strftime(pStr, GSIM_TIME_STR_MAX_LEN, "%a %b %d %Y %H:%M:%S", ptm);
    LOG_EXITVOID();
}

TaskList *TimeWheel::getPausedTaskList(Time_t time)
{
    U32 firstWheelSlot  = time % TW_ONE_SLOTS;
    U32 secondWheelSlot = (time / TW_ONE_SLOTS) % TW_TWO_SLOTS;
    U32 thirdWheelSlot  = (time / (TW_ONE_SLOTS * TW_TWO_SLOTS));

    if ((time / TW_ONE_SLOTS) == (wheelBase / TW_ONE_SLOTS))
    {
        return &wheelOne[firstWheelSlot];
    }
    else if ((time / (TW_ONE_SLOTS * TW_TWO_SLOTS)) ==
             (wheelBase / (TW_ONE_SLOTS * TW_TWO_SLOTS)))
    {
        return &wheelTwo[secondWheelSlot];
    }
    else if (thirdWheelSlot < TW_THREE_SLOTS)
    {
        return &wheelThree[thirdWheelSlot];
    }
    else
    {
        LOG_ERROR("Attempted to schedule a task too far in the future");
        return NULL;
    }
}

/**
 * @brief
 *    Iterate through our sorted set of paused tasks, removing those that
 *    should no longer be paused, and adding them to the run queue.
 *
 * @return
 */
S32 TimeWheel::resumePausedTasks()
{
    int found = 0;

    /* This while loop counts up from the wheelBase (i.e. the time
     * this function last ran) to the current scheduler time (i.e. clock_tick).
     */
    while (wheelBase < s_clockTick)
    {
        int slot1 = wheelBase % TW_ONE_SLOTS;

        /* If slot1 is 0 (i.e. wheelBase is a multiple of 4096ms),
         * we need to repopulate the first timer wheel with the
         * contents of the first available slot of the second wheel. */
        if (slot1 == 0)
        {
            /* slot2 represents the slot in the second timer wheel
             * containing the tasks for the next ~4s. So when
             * wheelBase is 4096, wheel2[1] will be moved into wheel 1,
             * when wheelBase of 8192 wheel2[2] will be moved into
             * wheel 1, etc. */
            int slot2 = (wheelBase / TW_ONE_SLOTS) % TW_TWO_SLOTS;

            /* If slot2 is also zero, we must migrate tasks from slot3 into
             * slot2.
             */
            if (slot2 == 0)
            {
                /* Same logic above, except that each slot of wheel3
                   contains the next 69 minutes of tasks, enough to
                   completely fill wheel 2. */
                int slot3 = ((wheelBase / TW_ONE_SLOTS) / TW_TWO_SLOTS);
                for (TaskListItr l3it = wheelThree[slot3].begin();
                     l3it != wheelThree[slot3].end(); l3it++)
                {
                    /* Migrate this task to wheel two. */
                    (*l3it)->recalcWheel();
                }

                wheelThree[slot3].clear();
            }

            /* Repopulate wheel 1 from wheel 2 (which will now be full
             * of the tasks pulled from wheel 3, if that was
             * necessary)
             */
            for (TaskListItr l2it = wheelTwo[slot2].begin();
                 l2it != wheelTwo[slot2].end(); l2it++)
            {
                /* Migrate this task to wheel one. */
                (*l2it)->recalcWheel();
            }

            wheelTwo[slot2].clear();
        }

        /* Move tasks from the current slot of wheel 1 (i.e. the tasks
         * scheduled to fire in the 1ms interval represented by wheelBase)
         * onto a run queue.
         */
        found += wheelOne[slot1].size();
        for (TaskListItr it = wheelOne[slot1].begin();
             it != wheelOne[slot1].end(); it++)
        {
            (*it)->setRunning();
            count--;
        }

        wheelOne[slot1].clear();
        wheelBase++;
    }

    return found;
}

/**
 * @brief Adds a task into correct timewheel
 *
 * @param task
 */
VOID TimeWheel::addTask(Task *task)
{
    Time_t wake = task->wake();

    /* if wakeup time is less than wheelbase add the task into runnable list */
    if (wake < wheelBase)
    {
        task->setRunning();
        return;
    }

    TaskList *  pauseList  = getPausedTaskList(wake);
    TaskListItr itr        = pauseList->insert(pauseList->end(), task);
    task->m_pausedTaskList = pauseList;
    task->m_pausedTaskItr  = itr;
    count++;
}

/**
 * @brief Removes the task from Paused Task List of the timewheel
 *
 * @param task
 */
VOID TimeWheel::removeTask(Task *task)
{
    TaskList *list = task->m_pausedTaskList;
    list->erase(task->m_pausedTaskItr);
    count--;
}

TimeWheel::TimeWheel()
{
    count     = 0;
    wheelBase = s_clockTick;
}

Counter TimeWheel::size()
{
    return count;
}
