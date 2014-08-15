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

#ifndef _TIMER_CPP_
#define _TIMER_CPP_

using namespace std;

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "types.hpp"
#include "error.hpp"
#include "thread.hpp"

#define GSIM_TIME_STR_MAX_LEN    40
#define LOCK_INTERRUPTS()
#define UNLOCK_INTERRUPTS()
#define STW_RESOLUTION         ( 100 ) 

typedef enum {
    RC_STW_OK = 0,
    RC_STW_NULL_NAME,
    RC_STW_NULL_FV,
    RC_STW_NULL_WHEEL,
    RC_STW_NULL_TMR,
    RC_STW_INVALID_WHEEL,
    RC_STW_INVALID_WHEEL_SIZE,
    RC_STW_INVALID_GRANULARITY,
    RC_STW_NO_RESOURCES,
} RC_STW_t;


/*
 * range of valid wheel sizes
 */
#define STW_MIN_WHEEL_SIZE    (   10 )
#define STW_MAX_WHEEL_SIZE    ( 1000 )

/*
 * Granularity of a timer tick in milliseconds   
 */
#define STW_MIN_GRANULARITY   (   1  )
#define STW_MAX_GRANULARITY   ( 1000 )

typedef U32     TimerId;
typedef VOID (*TimerCallback)(VOID *);

typedef struct _TimerData
{
    public:
        BOOL        timerState;             // timer running or not
        U32         timerId;                // unique id of the timer started
        U32         rotation_count;
        U32         delay;                  // initial delay
        U32         periodic_delay;         // auto-restart if > 0
        VOID        *userData;              // user's data passed in callback
                                            // function when the timer expires

        VOID        (*callBack)(VOID *);    // callback function to be invoked
                                            // when timer exipres.

        // constructor
        _TimerData()
        {
            rotation_count = 0;
            delay = 0;
            periodic_delay = 0;
            callBack = NULL;
        }

    private:
        list<_TimerData>::iterator itr;
} TimerData;

typedef list<TimerData *>   TimerDataList;

class Timer: public CThread
{
    public:
        Timer(string whellName, U32 wheelSize, U32 timerGranularity)
            throw (ErrCodeEn);
        ~Timer();
        VOID init();
        RETVAL startTimer(TimerData *pTmr, U32 delay,
                U32 periodic_delay, TimerCallback  pCbk, void  *pUserData);
        BOOL timerRunning(TimerData *pTmr);
        RC_STW_t stopTimer(TimerData *pTmr);

    private:
        string  timerWheelName;
        U32     wheel_size;
        U32     spoke_index;         /* mod index around wheel */
        U32     m_ticks;               /* absolute ticks */
        U32     m_granularity;       /* millisecond per tick */

        // few book keeping parameters to help engineer the wheel
        U32     timer_hiwater_mark;
        U32     timer_active;
        U32     timer_cancelled;
        U32     timer_expired;
        U32     timer_starts;

        vector<TimerDataList>  spokes;

        VOID tmrEnqueue (TimerData *tmr, U32 delay);
        VOID timerTick ();
        VOID run(VOID *pArg);
};

Time_t getMilliSeconds();
VOID getTimeStr(S8 *pStr);
#endif
