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

#include <nap/resource.h>

#include <glm/glm.hpp>

namespace nap {
	
	/**
	 * Resource that holds all data to be sent out. Data fields are created and set by the python script and read out by the monitor and OSC sender.
	 */
	class NAPAPI OutputData : public Resource {
		RTTI_ENABLE(Resource)
		
	public:
		OutputData() : Resource() { }
		
		/**
		 * Sets the number of output values per field.
		 */
		void setSize(int count);
		
		/**
		 * Adds a vec3 field.
		 */
		void addVec3Field(const std::string& fieldName);
		
		/**
		 * Adds a float field.
		 */
		void addFloatField(const std::string& fieldName);
		
		/**
		 * Safely set float value of field at index.
		 * Returns false if unsuccesfully set.
		 * TODO: could be optimised.
		 */
		bool setFloat(int index, const std::string& fieldName, float value);
		
		/**
		 * Safely set vec3 value of field at index.
		 * Returns false if unsuccesfully set.
		 * TODO: could be optimised.
		 */
		bool setVec3(int index, const std::string& fieldName, glm::vec3 value);
		
		/**
		 * 'Safe' getter for vec3 field. Returns reference to an empty vec3 vector if not found.
		 */
		const std::vector<glm::vec3>& getVec3Field(const std::string& name);
		
		/**
		 * 'Safe' getter for float field. Returns reference to an empty float vector if not found.
		 */
		const std::vector<float>& getFloatField(const std::string& name);
		
		const std::map<std::string, std::vector<glm::vec3>>& getVec3Fields(){ return mVec3Fields; }
		
		const std::map<std::string, std::vector<float>>& getFloatFields(){ return mFloatFields; }
		
		void clear() { mVec3Fields.clear(); mFloatFields.clear(); }
		
	private:
		int mSize = 0;
		
		std::map<std::string, std::vector<glm::vec3>> mVec3Fields;
		std::map<std::string, std::vector<float>> mFloatFields;
		
		std::vector<glm::vec3> mEmptyVec3Vector = {};
		std::vector<float> mEmptyFloatVector = {};
	};
	
}
