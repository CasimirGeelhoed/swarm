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
