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
     * Component that allows a LuaScript to control ParameterData and OutputData.
     */
    class NAPAPI LuaScriptComponent : public Component
    {
        RTTI_ENABLE(Component)
        DECLARE_COMPONENT(LuaScriptComponent, LuaScriptComponentInstance)
        
    public:
        LuaScriptComponent() : Component() { }
        
        nap::ResourcePtr<LuaScript> mScript; ///< Property: pointer to the Lua script.
        nap::ResourcePtr<ParameterData> mParameterData; ///< Property: pointer to the parameters.
        nap::ResourcePtr<OutputData> mOutputData; ///< Property: pointer to the output data.
        
    private:
    };

    
    class NAPAPI LuaScriptComponentInstance : public ComponentInstance
    {
        RTTI_ENABLE(ComponentInstance)
    public:
        LuaScriptComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource)
        {
            mScript = getComponent<LuaScriptComponent>()->mScript.get();
            mOutputData = getComponent<LuaScriptComponent>()->mOutputData.get();
            mParameterData = getComponent<LuaScriptComponent>()->mParameterData.get();
        }
        
		
        bool init(utility::ErrorState& errorState) override
		{
			// reserve space for log messages
			mLogMessages.reserve(25);
			
			
			// get the Lua namespace
			auto luaNamespace = mScript->getNamespace();
			
			// expose ParameterData & OutputData functions using lambdas
			luaNamespace.addFunction("addVec3Parameter", [&](const std::string& name, float min, float max, glm::vec3 value) { mParameterData->addVec3Parameter(name, min, max, value); });
			luaNamespace.addFunction("addFloatParameter", [&](const std::string& name, float min, float max, float value) { mParameterData->addFloatParameter(name, min, max, value); });
			luaNamespace.addFunction("getFloat", [&](const std::string& name) { return mParameterData->getFloat(name); });
			luaNamespace.addFunction("getVec3", [&](const std::string& name) { return mParameterData->getVec3(name); });

			luaNamespace.addFunction("addVec3Field", [&](const std::string& name) { mOutputData->addVec3Field(name); });
			luaNamespace.addFunction("addFloatField", [&](const std::string& name) { mOutputData->addFloatField(name); });
			
			luaNamespace.addFunction("setCount", [&](int count) { return mOutputData->setSize(count); });
			
			luaNamespace.addFunction("setVec3", [&](int index, const std::string& name, glm::vec3 value) { return mOutputData->setVec3(index, name, value); });
			luaNamespace.addFunction("setFloat", [&](int index, const std::string& name, float value) { return mOutputData->setFloat(index, name, value); });
			
			luaNamespace.addFunction("log", [&](const std::string& message) { logMessage(message); });

			// call 'init'
			utility::ErrorState e;
			bool returnValue = false; // TODO: support void functions!
			if(!mScript->call<bool>("init", e, returnValue))
				logMessage(e.toString());
			
			
			return true;
		}
		
        virtual void update(double deltaTime) override
		{
			utility::ErrorState e;
			bool returnValue = false;
			if(!mScript->call<bool>("update", e, returnValue, deltaTime))
				logMessage(e.toString());

		}
		
		const std::vector<std::string>& getLogMessages()
		{
			return mLogMessages;
		}
		
    private:
		
		void logMessage(const std::string& message)
		{
			// log to console
			Logger::info(message);
			
			// add message to message queue
			mLogMessages.emplace_back(message);

			// remove first element when out of range
			if (mLogMessages.size() > 25)
				mLogMessages.erase(mLogMessages.begin());
		}

        LuaScript* mScript = nullptr;
        OutputData* mOutputData = nullptr;
        ParameterData* mParameterData = nullptr;
		
        std::vector<std::string> mLogMessages; ///< Holds the received log messages.
		
    };
        
}
