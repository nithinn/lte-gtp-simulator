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

#include <iostream>

#include "types.hpp"
#include "logger.hpp"
#include "help.hpp"

const char *gHelpStr[15] = {
   "--node: Type of the LTE Core network node to be simulated",
   "--num-sessions: Maximum number of user (IMSI) sessions to be created",
   "--session-rate: Number of UE sessions created per second (rate period)",
   "--rate-period: Default is one second. This options allows user to "\
      "change the rate period",
   "--local-ip: Local IP Address at which the GTP simulator will listen "\
      "for GTPv2-C messages from peer entity",
   "--remote-ip: Remote peer IP Address, GTP simulator sends all "\
      "initiating messages to this IP address",
   "--local-port: Local GTPv2-C listening port. Default value is 2123.",
   "--remote-port: Remote peer UDP port number",
   "--t3-timer: GTP retransmission timer (T3 Timer)",
   "--n3-requests: GTP retransmission counter, number of times a GTP "\
      "message be retransmitted",
   "--disp-timer: Interval in seconds the session statistics display "\
      "screen be refreshed",
   "--disp-target: Scenario statistics display to be directed to a file "\
      "or screen, default value is screen",
   "--error-file: File name where the error logs has to be written",
   "--log-file: File name where the debug logs has to be written",
   "--log-level: Logging level for debugging purposes, default value = 1"};

PUBLIC VOID displayHelp()
{
   std::cout << std::endl;
   std::cout << "Usage: gsim [options] <scenario_file>";
   std::cout << std::endl;
   std::cout << std::endl;

   for (int i = 0; i < 15; i++)
   {
      std::cout << gHelpStr[i] << std::endl;
      std::cout << std::endl;
   }
}
