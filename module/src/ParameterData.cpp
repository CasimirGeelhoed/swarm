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

#include "ParameterData.h"

RTTI_BEGIN_CLASS(nap::ParameterData)
RTTI_END_CLASS

namespace nap {

	void ParameterData::addVec3Parameter(const std::string& name, float min, float max, glm::vec3 value)
	{
		auto param = std::make_unique<ParameterVec3>();
		param->mID = name;
		param->mName = name;
		param->mValue = value;
		param->mMinimum = min;
		param->mMaximum = max;
		mVec3Parameters.emplace(name, std::move(param));
	}


	void ParameterData::addFloatParameter(const std::string& name, float min, float max, float value)
	{
		auto param = std::make_unique<ParameterFloat>();
		param->mID = name;
		param->mName = name;
		param->mValue = value;
		param->mMinimum = min;
		param->mMaximum = max;
		mFloatParameters.emplace(name, std::move(param));
	}


	bool ParameterData::setFloat(const std::string& name, float value)
	{
		auto it = mFloatParameters.find(name);
		if(it == mFloatParameters.end())
			return false;
		
		it->second->setValue(value);
		return true;
	}


	bool ParameterData::setVec3(const std::string& name, glm::vec3 value)
	{
		auto it = mVec3Parameters.find(name);
		if(it == mVec3Parameters.end())
			return false;
		
		it->second->setValue(value);
		return true;
	}


	float ParameterData::getFloat(const std::string& name)
	{
		auto it = mFloatParameters.find(name);
		if(it == mFloatParameters.end())
			return 0.f;
		
		return it->second->mValue;
	}


	glm::vec3 ParameterData::getVec3(const std::string& name)
	{
		auto it = mVec3Parameters.find(name);
		if(it == mVec3Parameters.end())
			return glm::vec3(0.f);
		
		return it->second->mValue;
	}

}
