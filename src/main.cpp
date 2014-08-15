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
#include "error.hpp"
#include "sim_cfg.hpp"
#include "task.hpp"
#include "sim.hpp"

int main(int argc, char** argv)
{
   try
   {
      Config  *pCfg = Config::getInstance();
      pCfg->setConfig(argc, argv);

      Logger::init(pCfg->getLogLevel());

      LOG_INFO("Starting simulator");
      Simulator *pGtpSim = Simulator::getInstance();
      pGtpSim->run();
      delete pGtpSim;
      delete pCfg;
      LOG_INFO("Stopping simulator");
   }
   catch (ErrCodeEn e)
   {
      LOG_ERROR("Invalid Command Line options");
      return e;
   }

   return 0;
}
