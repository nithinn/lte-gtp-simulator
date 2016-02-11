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

#include <exception>

#include "types.hpp"
#include "logger.hpp"
#include "error.hpp"
#include "transport.hpp"
#include "sim_cfg.hpp"
#include "keyboard.hpp" 

EXTERN RETVAL setupStdinSock();

class Keyboard *Keyboard::m_pKb = NULL;
KeyboardKey_t Keyboard::key = KB_KEY_INVALID;

Keyboard* Keyboard::getInstance()
{
   try
   {
      if (NULL == m_pKb)
      {
         m_pKb = new Keyboard;
      }
   }
   catch (std::exception &e)
   {
      LOG_FATAL("Memory allocation failure, Keyboard");
      throw ERR_KEYBOARD_INIT;
   }

   return m_pKb;
}


/**
 * @brief
 *    Processes the keyboard input from user
 *
 * @param 
 */
VOID Keyboard::processKey
(
S32   kbInput  
)
{
   LOG_ENTERFN();

   switch(kbInput)
   {
      case 'p':
      {
         LOG_DEBUG("Pausing Traffic");
         key = KB_KEY_PAUSE_TRAFFIC;
         break;
      }
      case 'c':
      {
         LOG_DEBUG("Resuming Traffic");
         key = KB_KEY_INVALID;
         break;
      }
      case 'q':
      {
         key = KB_KEY_SIM_QUIT;
         break;
      }
      case '+':
      {
         Config::getInstance()->incrRate(1);
         break;
      }
      case '-':
      {
         Config::getInstance()->decrRate(1);
         break;
      }
      case '*':
      {
         Config::getInstance()->incrRate(10);
         break;
      }
      case '/':
      {
         Config::getInstance()->decrRate(10);
         break;
      }
      default:
      {
         LOG_DEBUG("Invalid Keyboard Input");
         break;
      }
   }

   LOG_EXITVOID();
}

/**
 * @brief
 *    Creates Keyboard socket
 */
VOID Keyboard::init()
{
   LOG_ENTERFN();

   LOG_DEBUG("Setting up STDIN Socket for keyboard");
   if (ROK != setupStdinSock())
   {
      LOG_FATAL("Setting up STDIN Socket for keyboard");
      throw ERR_KEYBOARD_INIT;
   }

   LOG_EXITFN();
}

VOID Keyboard::abort()
{
   LOG_ENTERFN();

   delete this;

   LOG_EXITVOID();
}

