//
//  LuaScriptComponent.cpp
//  napswarm
//
//  Created by Casimir Geelhoed on 05/07/2024.
//

#include "LuaScriptComponent.h"

#include <entity.h>

#include <utility/fileutils.h>


// RTTI
RTTI_BEGIN_CLASS(nap::LuaScriptComponent)
	RTTI_PROPERTY("ParameterData", &nap::LuaScriptComponent::mParameterData, nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("OutputData", &nap::LuaScriptComponent::mOutputData, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::LuaScriptComponentInstance)
	RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS


namespace nap
{

	struct VecHelper
	{
		template <unsigned index>
		static float get (glm::vec3 const* vec)
		{
			return (*vec)[index];
		}
		
		template <unsigned index>
		static void set (glm::vec3* vec, float value)
		{
			(*vec)[index] = value;
		}
		
		static glm::vec3 add(const glm::vec3& l, const glm::vec3& r) {
			return l + r;
		}
		
		static glm::vec3 sub(const glm::vec3& l, const glm::vec3& r) {
			return l - r;
		}
		
		static glm::vec3 mul(const glm::vec3& v, float scalar) {
			return v * scalar;
		}
		
		static glm::vec3 div(const glm::vec3& v, float scalar) {
			return v / scalar;
		}
		
	};
	
	
	LuaScriptComponentInstance::LuaScriptComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource)
	{
		mOutputData = getComponent<LuaScriptComponent>()->mOutputData.get();
		mParameterData = getComponent<LuaScriptComponent>()->mParameterData.get();
	}


	bool LuaScriptComponentInstance::init(utility::ErrorState& errorState)
	{
		// Reserve space for log messages.
		mLogMessages.reserve(25);
		
		return true;
	}

	
	bool LuaScriptComponentInstance::loadScript(const std::string& path, utility::ErrorState& errorState)
	{
		// Clear data.
		mOutputData->clear();
		mParameterData->clear();
		
		// Clear old log messages.
		mLogMessages.clear();
		
		// Make resource, set mScript back to nullptr if it fails.
		mScript = std::make_unique<LuaScript>();
		mScript->mPath = path;
		if(!mScript->init(errorState))
		{
			logMessage("Script not found: " + path);
			mScript = nullptr;
			return false;
		}
				
		// Bind the vec3 type.
		mScript->getNamespace().beginClass<glm::vec3>("vec3")
		.addConstructor<void(*)(float, float, float)>()
		.addProperty ("x", &VecHelper::get<0>, &VecHelper::set<0>)
		.addProperty ("y", &VecHelper::get<1>, &VecHelper::set<1>)
		.addProperty ("z", &VecHelper::get<2>, &VecHelper::set<2>)
		.addFunction("__add", &VecHelper::add)
		.addFunction("__sub", &VecHelper::sub)
		.addFunction("__mul", &VecHelper::mul)
		.addFunction("__div", &VecHelper::div)
		.endClass();

		// Get the Lua namespace.
		auto luaNamespace = mScript->getNamespace();

		// Expose ParameterData functions.
		luaNamespace.addFunction("addVec3Parameter", [&](const std::string& name, float min, float max, glm::vec3 value) { mParameterData->addVec3Parameter(name, min, max, value); });
		luaNamespace.addFunction("addFloatParameter", [&](const std::string& name, float min, float max, float value) { mParameterData->addFloatParameter(name, min, max, value); });
		luaNamespace.addFunction("getFloat", [&](const std::string& name) { return mParameterData->getFloat(name); });
		luaNamespace.addFunction("getVec3", [&](const std::string& name) { return mParameterData->getVec3(name); });
		
		// Expose OutputData functions.
		luaNamespace.addFunction("addVec3Field", [&](const std::string& name) { mOutputData->addVec3Field(name); });
		luaNamespace.addFunction("addFloatField", [&](const std::string& name) { mOutputData->addFloatField(name); });
		luaNamespace.addFunction("setCount", [&](int count) { return mOutputData->setSize(count); });
		luaNamespace.addFunction("setVec3", [&](int index, const std::string& name, glm::vec3 value) { return mOutputData->setVec3(index, name, value); });
		luaNamespace.addFunction("setFloat", [&](int index, const std::string& name, float value) { return mOutputData->setFloat(index, name, value); });
		
		// Expose log function.
		luaNamespace.addFunction("log", [&](const std::string& message) { logMessage(message); });
		
		// Load script.
		utility::ErrorState e;
		if(!mScript->load(e))
		{
			logMessage(e.toString());
			return false;
		}

		// Call script's 'init' function.
		if(!mScript->callVoid("init", e))
		{
			logMessage(e.toString());
			return false;
		}
		
		return true;
		
	}
	
	
	void LuaScriptComponentInstance::update(double deltaTime)
	{
		if(!mScript)
			return;
		
		if(!mScript->mValid)
			return;
		
		// Call 'update'.
		utility::ErrorState e;
		if(!mScript->callVoid("update", e, deltaTime))
			logMessage(e.toString());
		
		// Check for real-time edit to script.
		uint64 modTime;
		bool canGetModTime = utility::getFileModificationTime(mScript->mPath, modTime);
		if (canGetModTime && modTime != mLastModTime)
		{
			// a real-time edit has happened, reload the script
			mLastModTime = modTime;
			utility::ErrorState e;
			std::string pathCopy = mScript->mPath;
			loadScript(pathCopy, e);
		}

	}


	void LuaScriptComponentInstance::logMessage(const std::string& message)
	{
		// Log to console.
		Logger::info(message);
		
		// Add message to message queue.
		mLogMessages.emplace_back(message);
		
		// Remove first element when out of range.
		if (mLogMessages.size() > 25)
			mLogMessages.erase(mLogMessages.begin());
	}

}
