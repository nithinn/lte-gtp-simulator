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

#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__

typedef enum
{
   KB_KEY_INVALID,
   KB_KEY_PAUSE_TRAFFIC,
   KB_KEY_SIM_QUIT,
   KB_KEY_SIM_MAX
} KeyboardKey_t;

class Keyboard
{
   public:
      static Keyboard* getInstance();
      static KeyboardKey_t key;
      VOID   processKey(S32 kbInput);
      VOID   init();
      VOID   abort();
      
   private:
      static class Keyboard   *m_pKb;
};

#endif
