/* Copyright (C) 2024 Casimir Geelhoed */

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
