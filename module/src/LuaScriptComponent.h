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
	 * Component that allows a @LuaScript to control @ParameterData and @OutputData.
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
		 */
		bool loadScript(const std::string& path, utility::ErrorState& errorState);
		
		/**
		 * Returns the received Lua log messages.
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
