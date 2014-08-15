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

#ifndef _SIM_HPP_
#define _SIM_HPP_

using namespace std;

#include <cstdio>
#include <list>

class Simulator
{
   public:
      ~Simulator();
      static Simulator* getInstance();

      void run();
      void postEvent();

   private:
      Simulator();
      VOID genSignallingTraffic();

      static class Simulator  *pSim;
      class Scenario          *m_pScn;
};

#endif
