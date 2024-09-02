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

#include "DataReceivingComponent.h"

// Nap includes
#include <entity.h>

// RTTI
RTTI_BEGIN_CLASS(nap::DataReceivingComponent)
	RTTI_PROPERTY("OSCInputComponent", &nap::DataReceivingComponent::mOSCInputComponent, nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("ParameterData", &nap::DataReceivingComponent::mParameterData, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::DataReceivingComponentInstance)
	RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

namespace nap
{

	bool DataReceivingComponentInstance::init(utility::ErrorState& errorState)
	{
		mParameterData = getComponent<DataReceivingComponent>()->mParameterData.get();
		
		mOSCInputComponent->getMessageReceived()->connect(mMessageReceivedSlot);
		
		return true;
	}


	// Converts int / float / double arguments to a float value. If unsuccesful, return 0.
	float argumentToFloat(const OSCArgument& argument)
	{
		if (argument.isFloat())
			return argument.asFloat();
		else if (argument.isDouble())
			return argument.asDouble();
		else if (argument.isInt())
			return argument.asInt();
		return false;
	}


	void DataReceivingComponentInstance::onMessageReceived(const OSCEvent& event)
	{
		// TODO: is this simple parsing completely fool-proof?
		// TODO: log wrong OSC input.
		if(event.getCount() == 1)
			mParameterData->setFloat(event.getAddress(), argumentToFloat(*event.getArgument(0)));
		else if(event.getCount() == 3)
			mParameterData->setVec3(event.getAddress(), glm::vec3(argumentToFloat(*event.getArgument(0)), argumentToFloat(*event.getArgument(1)), argumentToFloat(*event.getArgument(2))));
	}

}
