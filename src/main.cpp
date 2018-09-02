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

#include "types.hpp"
#include "logger.hpp"
#include "error.hpp"
#include "help.hpp"
#include "macros.hpp"
#include "gtp_types.hpp"
#include "sim_cfg.hpp"
#include "task.hpp"
#include "sim.hpp"

#include <cxxopts.hpp>

#include <list>

int main(int argc, char **argv)
{
    cxxopts::Options options(argv[0], "LTE GTPv2-C Simulator");

    try
    {
        // clang-format off
        options.add_options()
            ("node", "Type of 3gpp Node for simulation",
             cxxopts::value<std::string>());
        options.add_options()
            ("num-sessions", "Maximum number of user (IMSI) sessions to be "\
             "created", cxxopts::value<std::uint32_t>());
        options.add_options()
             ("session-rate", "Number of UE sessions created per second "\
              "(rate period)", cxxopts::value<std::uint32_t>());
        options.add_options()
             ("rate-period", "Default is one second. This options allows "\
              "user to change the rate period",
              cxxopts::value<std::uint32_t>());
        options.add_options()
            ("local-ip", "Local IP Address at which the GTP simulator "\
             "will listen for GTPv2-C messages from peer entity");
             cxxopts::value<std::string>(),
        options.add_options()
            ("remote-ip", "Remote peer IP Address, GTP simulator sends all "\
             "initiating messages to this IP address",
             cxxopts::value<std::string>());
        options.add_options()
            ("local-port", "Local GTPv2-C listening port. "\
             "Default value is 2123.",
             cxxopts::value<std::uint16_t>());
        options.add_options()
            ("remote-port", "Remote peer UDP port number",
             cxxopts::value<std::uint16_t>());
        options.add_options()
            ("t3-timer", "GTP retransmission timer (T3 Timer)",
             cxxopts::value<std::uint32_t>());
        options.add_options()
            ("n3-requests", "GTP retransmission counter, "
            "number of times a GTP message be retransmitted",
             cxxopts::value<std::uint32_t>());
        options.add_options()
            ("disp-timer", "Interval in seconds the "
            "session statistics display screen be refreshed",
             cxxopts::value<std::uint32_t>());
        options.add_options()
            ("disp-target", "Scenario statistics display "
            "to be directed to a file or screen, default value is screen",
             cxxopts::value<std::uint32_t>());
        options.add_options()
            ("error-file", "File name where the error "
            "logs has to be written",
             cxxopts::value<std::string>());
        options.add_options()
            ("log-file", "File name where the debug logs has to be written",
             cxxopts::value<std::string>());
        options.add_options()
            ("log-level", "Logging level for debugging purposes",
             cxxopts::value<std::uint32_t>());
        options.add_options()
            ("scenario", "Scenario file", cxxopts::value<std::string>());
        options.add_options()
             ("help", "Print help and exit");
        // clang-format on

        auto results = options.parse(argc, argv);
        if (results.count("help"))
        {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        Config *pCfg = Config::getInstance();
        pCfg->setConfig(results);

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
        std::cout << "Error: " << e << std::endl;
        exit(e);
    }
    catch (GsimError e)
    {
        std::cout << e.what() << std::endl;
        exit(1);
    }
    catch (const cxxopts::OptionException &e)
    {
        std::cout << "error parsing options: " << e.what() << std::endl;
        std::cout << options.help() << std::endl;
        exit(1);
    }

    return 0;
}
