//
//  LuaScriptComponent.cpp
//  napswarm
//
//  Created by Casimir Geelhoed on 05/07/2024.
//

#include "LuaScriptComponent.h"

#include <entity.h>


// RTTI
RTTI_BEGIN_CLASS(nap::LuaScriptComponent)
    RTTI_PROPERTY("Script", &nap::LuaScriptComponent::mScript, nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("ParameterData", &nap::LuaScriptComponent::mParameterData, nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("OutputData", &nap::LuaScriptComponent::mOutputData, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::LuaScriptComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS


namespace nap
{

	LuaScriptComponentInstance::LuaScriptComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource)
	{
		mScript = getComponent<LuaScriptComponent>()->mScript.get();
		mOutputData = getComponent<LuaScriptComponent>()->mOutputData.get();
		mParameterData = getComponent<LuaScriptComponent>()->mParameterData.get();
	}
	
	
	bool LuaScriptComponentInstance::init(utility::ErrorState& errorState)
	{
		// reserve space for log messages
		mLogMessages.reserve(25);
		
		// get the Lua namespace
		auto luaNamespace = mScript->getNamespace();
		
		// expose ParameterData functions
		luaNamespace.addFunction("addVec3Parameter", [&](const std::string& name, float min, float max, glm::vec3 value) { mParameterData->addVec3Parameter(name, min, max, value); });
		luaNamespace.addFunction("addFloatParameter", [&](const std::string& name, float min, float max, float value) { mParameterData->addFloatParameter(name, min, max, value); });
		luaNamespace.addFunction("getFloat", [&](const std::string& name) { return mParameterData->getFloat(name); });
		luaNamespace.addFunction("getVec3", [&](const std::string& name) { return mParameterData->getVec3(name); });

		// expose OutputData functions
		luaNamespace.addFunction("addVec3Field", [&](const std::string& name) { mOutputData->addVec3Field(name); });
		luaNamespace.addFunction("addFloatField", [&](const std::string& name) { mOutputData->addFloatField(name); });
		luaNamespace.addFunction("setCount", [&](int count) { return mOutputData->setSize(count); });
		luaNamespace.addFunction("setVec3", [&](int index, const std::string& name, glm::vec3 value) { return mOutputData->setVec3(index, name, value); });
		luaNamespace.addFunction("setFloat", [&](int index, const std::string& name, float value) { return mOutputData->setFloat(index, name, value); });
		
		// expose log function
		luaNamespace.addFunction("log", [&](const std::string& message) { logMessage(message); });

		// call 'init'
		utility::ErrorState e;
		if(!mScript->callVoid("init", e))
			logMessage(e.toString());
		
		return true;
	}
	
	void LuaScriptComponentInstance::update(double deltaTime)
	{
		// call 'update'
		utility::ErrorState e;
		if(!mScript->callVoid("update", e, deltaTime))
			logMessage(e.toString());
	}
	
	
	void LuaScriptComponentInstance::logMessage(const std::string& message)
	{
		// log to console
		Logger::info(message);
		
		// add message to message queue
		mLogMessages.emplace_back(message);

		// remove first element when out of range
		if (mLogMessages.size() > 25)
			mLogMessages.erase(mLogMessages.begin());
	}

}
