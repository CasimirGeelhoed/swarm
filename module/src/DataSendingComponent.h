/* Copyright (C) 2024 Casimir Geelhoed */

#pragma once

// Nap includes
#include <component.h>
#include <nap/resourceptr.h>

#include <oscsender.h>

#include "OutputData.h"

namespace nap
{
	
	class DataSendingComponentInstance;
	
	/**
	 * Component that reads out all @OutputData and sends it out over OSC.
	 */
	class NAPAPI DataSendingComponent : public Component
	{
		RTTI_ENABLE(Component)
		DECLARE_COMPONENT(DataSendingComponent, DataSendingComponentInstance)
		
	public:
		DataSendingComponent() : Component() { }
		
		ResourcePtr<OSCSender> mOSCSender; ///< Property: 'OSCSender' The OSC Sender.
		ResourcePtr<OutputData> mData; ///< Property: 'Data' The data set to output.
	};
	
	
	class NAPAPI DataSendingComponentInstance : public ComponentInstance
	{
		RTTI_ENABLE(ComponentInstance)
	public:
		DataSendingComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource) { }
		
		bool init(utility::ErrorState& errorState) override;
		
		void update(double deltaTime) override;
		
		/**
		 * Sets the OSC output rate.
		 * @param outputRate the desired OSC output rate in times per second
		 */
		void setOutputRate(double outputRate) { mOutputInterval = 1.0 / outputRate; }
		
	private:
		
		double mOutputInterval = 0.5f; // number of seconds before next output
		double mTimer = 0.f;
		
		OSCSender* mOSCSender = nullptr;
		OutputData* mData = nullptr;
	};
	
}
