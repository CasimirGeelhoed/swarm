/* Copyright (C) 2024 Casimir Geelhoed */

#pragma once

// Nap includes
#include <component.h>
#include <nap/resourceptr.h>
#include <componentptr.h>

#include <oscevent.h>
#include <oscinputcomponent.h>

#include "ParameterData.h"

namespace nap
{
	
	class DataReceivingComponentInstance;
	
	/**
	 * Component that receives input OSC and sets the @ParameterData accordingly.
	 */
	class NAPAPI DataReceivingComponent : public Component
	{
		RTTI_ENABLE(Component)
		DECLARE_COMPONENT(DataReceivingComponent, DataReceivingComponentInstance)
		
	public:
		DataReceivingComponent() : Component() { }
		
		ComponentPtr<OSCInputComponent> mOSCInputComponent; ///< The OSC input component.
		ResourcePtr<ParameterData> mParameterData; ///< The parameters.
	};
	
	
	class NAPAPI DataReceivingComponentInstance : public ComponentInstance
	{
		RTTI_ENABLE(ComponentInstance)
	public:
		DataReceivingComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource) { }
		
		bool init(utility::ErrorState& errorState) override;
		
	private:
		nap::Slot<const OSCEvent&> mMessageReceivedSlot  = { this, &DataReceivingComponentInstance::onMessageReceived };
		
		void onMessageReceived(const OSCEvent& event);
		
		ComponentInstancePtr<OSCInputComponent> mOSCInputComponent = { this, &DataReceivingComponent::mOSCInputComponent };
		ParameterData* mParameterData = nullptr;
	};
	
}
