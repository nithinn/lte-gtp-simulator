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

#include "thread.hpp"

CThread::CThread()
{
    pthread_attr_init(&threadAttr);
}

S32 CThread::start(VOID* arg)
{
   userArg = arg;
   return pthread_create(&threadId, &threadAttr, CThread::entryPoint,
        (VOID *)this);
}

VOID CThread::execute()
{
   run(userArg);
}

/* pthread create entry point function */
VOID* CThread::entryPoint(VOID * pThis)
{
   ((CThread*)pThis)->execute();
   return NULL;
}

