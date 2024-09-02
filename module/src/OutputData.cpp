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

#include "OutputData.h"

RTTI_BEGIN_CLASS(nap::OutputData)
RTTI_END_CLASS

namespace nap {

	void OutputData::setSize(int count)
	{
		mSize = count;
		
		for(auto& f : mVec3Fields)
			f.second.resize(count);
		for(auto& f : mFloatFields)
			f.second.resize(count);
	}


	void OutputData::addVec3Field(const std::string& fieldName)
	{
		mVec3Fields.emplace(fieldName, std::vector<glm::vec3>(mSize));
	}


	void OutputData::addFloatField(const std::string& fieldName)
	{
		mFloatFields.emplace(fieldName, std::vector<float>(mSize));
	}


	bool OutputData::setFloat(int index, const std::string& fieldName, float value)
	{
		index--;
		if(index < 0 || index >= mSize)
			return false;
		
		auto it = mFloatFields.find(fieldName);
		if(it == mFloatFields.end())
			return false;
		
		it->second[index] = value;
		return true;
	}


	bool OutputData::setVec3(int index, const std::string& fieldName, glm::vec3 value)
	{
		index--;
		if(index < 0 || index >= mSize)
			return false;
		
		auto it = mVec3Fields.find(fieldName);
		if(it == mVec3Fields.end())
			return false;
		
		it->second[index] = value;
		return true;
	}


	const std::vector<glm::vec3>& OutputData::getVec3Field(const std::string& name)
	{
		auto it = mVec3Fields.find(name);
		if(it == mVec3Fields.end())
			return mEmptyVec3Vector;
		
		return it->second;
	}


	const std::vector<float>& OutputData::getFloatField(const std::string& name)
	{
		auto it = mFloatFields.find(name);
		if(it == mFloatFields.end())
			return mEmptyFloatVector;
		
		return it->second;
	}

}

