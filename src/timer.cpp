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
   struct timeval sysTime;

   gettimeofday(&sysTime, NULL);
   Time_t usec = (Time_t)sysTime.tv_sec * 1000000LL + sysTime.tv_usec;
   Time_t msec = usec / 1000LL;

   s_clockTick = msec;
   return msec;
}

VOID getTimeStr(S8 *pStr)
{
   LOG_ENTERFN();

   struct timeval ptime;
   struct tm* ptm;

   gettimeofday(&ptime, NULL);
   ptm = localtime (&ptime.tv_sec);

   /* Format the date and time, year, hour, minute and seconds */
   strftime (pStr, GSIM_TIME_STR_MAX_LEN, "%a %b %d %Y %H:%M:%S", ptm);
   LOG_EXITVOID();
}
