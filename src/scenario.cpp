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

#include "pugixml.hpp"
using namespace pugi;

#include <vector>
#include <list>

#include "types.hpp"
#include "error.hpp"
#include "macros.hpp"
#include "logger.hpp"
#include "gtp_types.hpp"
#include "gtp_util.hpp"
#include "gtp_if.hpp"
#include "gtp_ie.hpp"
#include "gtp_msg.hpp"
#include "message.hpp"
#include "task.hpp"
#include "sim_cfg.hpp"
#include "scenario.hpp"

class Scenario* Scenario::m_pMainScn = NULL;  
EXTERN VOID parseXmlScenario(const S8*, MsgVec*) throw (ErrCodeEn);

Scenario* Scenario::getInstance()
{
   try
   {
      if (NULL == m_pMainScn)
      {
         m_pMainScn = new Scenario;
      }
   }
   catch (std::exception &e)
   {
      LOG_FATAL("Memory allocation failure, Scenario");
      throw ERR_MEMORY_ALLOC;
   }

   return m_pMainScn;
}

Scenario::Scenario()
{
   m_scnRunIntvl = Config::getInstance()->getScnRunInterval();
   m_ifType = (GtpIfType_t)Config::getInstance()->getIfType();
}

Scenario::~Scenario()
{
   for (MsgVecItr itr = m_msgVec.begin(); itr != m_msgVec.end(); itr++)
   {
      delete *itr;
   }
}


/**
 * @brief
 *    Constructor
 *    Creates the complete scenario by reading the xml scenario file
 *
 * @param pScnFile
 *    Name of xml file containing the scenario
 *    
 */
VOID Scenario::init(const S8 *pScnFile) throw (ErrCodeEn)
{
   try
   {
      parseXmlScenario(pScnFile, &m_msgVec);  
   }
   catch (ErrCodeEn &e)
   {
      LOG_FATAL("Xml Parsing failed");
      throw e;
   }

   MsgTask *pFirstTask = m_msgVec[0];
   if (pFirstTask->type() == MSG_TASK_SEND)
   {
      m_scnType = SCN_TYPE_INITIATING;
   }
   else
   {
      m_scnType = SCN_TYPE_WAITING;
   }
}


/**
 * @brief
 *    Executes the scenario
 */
BOOL Scenario::run()
{
   return true;
}

ScenarioType_t Scenario::getScnType()
{
   return m_scnType;
}

VOID Scenario::shutdown()
{
   LOG_ENTERFN();

   delete this;

   LOG_EXITVOID();
}

GtpIfType_t Scenario::ifType()
{
   return m_ifType;
}
