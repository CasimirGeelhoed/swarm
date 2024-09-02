/*
 This file is part of Swarm, a free scriptable OSC controller with 3D visualisation.
 Copyright (C) 2024 Casimir Geelhoed
 
 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or any later version.
 
 This program is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of  MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along with
 this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Local Includes
#include "swarmapp.h"

// Nap includes
#include <apprunner.h>
#include <nap/logger.h>
#include <guiappeventhandler.h>

// Main loop
int main(int argc, char *argv[])
{
    // Create core
    nap::Core core;

    // Create the application runner, based on the app to run
	// and event handler that is used to forward information into the app.
    nap::AppRunner<nap::CoreApp, nap::GUIAppEventHandler> app_runner(core);

    // Start running
    nap::utility::ErrorState error;
    if (!app_runner.start(error))
    {
        nap::Logger::fatal("error: %s", error.toString().c_str());
        return -1;
    }

    // Return if the app ran successfully
    return app_runner.exitCode();
}

