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

#pragma once

// Nap includes
#include <component.h>
#include <componentptr.h>
#include <nap/resourceptr.h>

#include <LuaScript.h>
#include <OutputData.h>
#include <ParameterData.h>

namespace nap
{
	
	class LuaScriptComponentInstance;
	
	/**
	 * Component that allows a @LuaScript to control @ParameterData and @OutputData. The Lua script can be (re)loaded at runtime.
	 */
	class NAPAPI LuaScriptComponent : public Component
	{
		RTTI_ENABLE(Component)
		DECLARE_COMPONENT(LuaScriptComponent, LuaScriptComponentInstance)
		
	public:
		LuaScriptComponent() : Component() { }
		
		nap::ResourcePtr<ParameterData> mParameterData; ///< Property: pointer to the parameters.
		nap::ResourcePtr<OutputData> mOutputData; ///< Property: pointer to the output data.
		
	private:
	};
	
	
	class NAPAPI LuaScriptComponentInstance : public ComponentInstance
	{
		RTTI_ENABLE(ComponentInstance)
	public:
		LuaScriptComponentInstance(EntityInstance& entity, Component& resource);
		
		// Inhereted from ComponentInstance
		bool init(utility::ErrorState& errorState) override;
		virtual void update(double deltaTime) override;
		
		/**
		 * Instantiates a LuaScript based on a given path. Called from the App.
		 * @param path the path of the Lua script
		 * @param errorState contains the error if loading the script failed
		 * @return whether it succeeded to load the script
		 */
		bool loadScript(const std::string& path, utility::ErrorState& errorState);
		
		/**
		 * Returns the received Lua log messages.
		 * @return the latest log messages.
		 */
		const std::vector<std::string>& getLogMessages() { return mLogMessages; }
		
	private:
		void logMessage(const std::string& message);
				
		std::unique_ptr<LuaScript> mScript = nullptr;
		
		OutputData* mOutputData = nullptr;
		ParameterData* mParameterData = nullptr;
		
		std::vector<std::string> mLogMessages; ///< Holds the received log messages.
		
		uint64_t mLastModTime = 0;
		
	};
	
}
