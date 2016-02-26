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

/* Heirarchical Timer Wheel Implementation: George Varghese & Tony Lauck
 * Paper
 */

/* Implementation of 3 Tier Heirarchical Time Wheel */

#ifndef _TIMER_CPP_
#define _TIMER_CPP_

using namespace std;

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "types.hpp"
#include "error.hpp"
#include "task.hpp"

#define GSIM_TIME_STR_MAX_LEN    32
#define TW_ONE_SLOTS             (1 << 12)
#define TW_TWO_SLOTS             (1 << 10)
#define TW_THREE_SLOTS           (1 << 10)

/* resolution of time wheel is milli-seconds */
class TimeWheel
{
   public:
      TimeWheel();

      void addTask(Task* t);
      void removeTask(Task* t);
      void wakeupTask();
      S32 resumePausedTasks();
      Counter size();

   private:
      Time_t   wheelBase;
      Counter  count;

      /* wheel one has 1 ^ 12 millisecond slots */
      TaskList wheelOne[TW_ONE_SLOTS];

      /* wheel two has 1 ^ 12 to 1 ^ 22 milli-second slots */
      TaskList wheelTwo[TW_TWO_SLOTS];

      /* wheel two has 1 ^ 22 to 1 ^ 32 milli-second slots */
      TaskList wheelThree[TW_THREE_SLOTS];

      TaskList *getPausedTaskList(Time_t time);
};

Time_t getMilliSeconds();
VOID getTimeStr(S8 *pStr);
#endif
