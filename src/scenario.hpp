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

#ifndef _SCENARIO_HPP_
#define _SCENARIO_HPP_

typedef enum
{
   SCN_TYPE_INVALID,
   SCN_TYPE_INITIATING, /**< First action in scenario is send a message */
   SCN_TYPE_WAITING,    /**< First action in scenario is recv a message */
   SCN_TYPE_MAX
} ScenarioType_t;

class Scenario
{
   public:
      ~Scenario();

      static class Scenario* getInstance();
      ScenarioType_t getScnType();
      BOOL           run();
      VOID           init(const S8 *pScnFile) throw (ErrCodeEn);
      VOID           shutdown();
      GtpIfType_t    ifType();

      JobSequence    m_jobSeq;
      ProcSequence   procSeq;

   private:
      Scenario();

      static class Scenario   *m_pMainScn;
      U32            m_lastRunTime;
      U32            m_scnRunIntvl;
      ScenarioType_t m_scnType;
      GtpIfType_t    m_ifType;
};

#endif
