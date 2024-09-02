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

#include "LuaScriptComponent.h"

#include <entity.h>

#include <utility/fileutils.h>

#include <glm/gtc/matrix_transform.hpp>

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
		
		static glm::vec3 add(const glm::vec3& l, const glm::vec3& r)
		{
			return l + r;
		}
		
		static glm::vec3 sub(const glm::vec3& l, const glm::vec3& r)
		{
			return l - r;
		}
		
		static glm::vec3 div(const glm::vec3& v, float scalar)
		{
			return v / scalar;
		}
		
		static glm::vec3 mul(lua_State* L)
		{
			// Check the number of arguments passed from Lua.
			if (lua_gettop(L) != 2)
			{
				luaL_error(L, "Invalid arguemnts: two arguments expected.");
				return glm::vec3();
			}
			
			glm::vec3 result;
			
			// Check if the first argument is a float and the second is a vec3.
			if (lua_isnumber(L, 1) && lua_isuserdata(L, 2))
			{
				float scalar = static_cast<float>(lua_tonumber(L, 1));
				glm::vec3* vec = luabridge::detail::Userdata::get<glm::vec3>(L, 2, false);
				return scalar * (*vec);
			}
			// Check if the first argument is a vec3 and the second is a float.
			else if (lua_isuserdata(L, 1) && lua_isnumber(L, 2))
			{
				glm::vec3* vec = luabridge::detail::Userdata::get<glm::vec3>(L, 1, false);
				float scalar = static_cast<float>(lua_tonumber(L, 2));
				return (*vec) * scalar;
			}
			
			luaL_error(L, "Invalid arguments: expected (number, vec3) or (vec3, number).");
			return glm::vec3();
		}
		
		static bool equal(const glm::vec3& l, const glm::vec3& r)
		{
			return l == r;
		}
		
		static glm::vec3 normalize(const glm::vec3& vec)
		{
			return glm::normalize(vec);
		}
		
		static float distance(const glm::vec3& l, const glm::vec3& r)
		{
			return glm::distance(l, r);
		}
		
		static glm::vec3 cross(const glm::vec3& l, const glm::vec3& r)
		{
			return glm::cross(l, r);
		}
		
		static float dot(const glm::vec3& l, const glm::vec3& r)
		{
			return glm::dot(l, r);
		}
		
		static float length(const glm::vec3& vec)
		{
			return glm::length(vec);
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
		.addProperty("x", &VecHelper::get<0>, &VecHelper::set<0>)
		.addProperty("y", &VecHelper::get<1>, &VecHelper::set<1>)
		.addProperty("z", &VecHelper::get<2>, &VecHelper::set<2>)
		.addFunction("__add", &VecHelper::add)
		.addFunction("__sub", &VecHelper::sub)
		.addFunction("__mul", &VecHelper::mul)
		.addFunction("__div", &VecHelper::div)
		.addFunction("__eq", &VecHelper::equal)
		.endClass();

		// Get the Lua namespace.
		auto luaNamespace = mScript->getNamespace();
		
		// Expose vec3 functions.
		luaNamespace.addFunction("normalize", &VecHelper::normalize);
		luaNamespace.addFunction("distance", &VecHelper::distance);
		luaNamespace.addFunction("cross", &VecHelper::cross);
		luaNamespace.addFunction("dot", &VecHelper::dot);
		luaNamespace.addFunction("length", &VecHelper::length);

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
		
		// Call 'update' if the script is valid.
		if(mScript->mValid)
		{
			utility::ErrorState e;
			if(!mScript->callVoid("update", e, deltaTime))
				logMessage(e.toString());
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
