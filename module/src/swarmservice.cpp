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

// Local Includes
#include "swarmservice.h"

// External Includes
#include <nap/core.h>
#include <nap/resourcemanager.h>
#include <nap/logger.h>
#include <iostream>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::swarmService)
	RTTI_CONSTRUCTOR(nap::ServiceConfiguration*)
RTTI_END_CLASS

RTTI_BEGIN_CLASS(nap::SwarmServiceConfiguration)
	RTTI_PROPERTY("ScriptPath", &nap::SwarmServiceConfiguration::mScriptPath, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("OSCInputPort", &nap::SwarmServiceConfiguration::mOSCInputPort, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("OSCOutputAddress", &nap::SwarmServiceConfiguration::mOSCOutputAddress, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("OSCOutputPort", &nap::SwarmServiceConfiguration::mOSCOutputPort, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("OSCRate", &nap::SwarmServiceConfiguration::mOSCRate, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("Gnomon", &nap::SwarmServiceConfiguration::mGnomon, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("Shadows", &nap::SwarmServiceConfiguration::mShadows, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("CircleGrid", &nap::SwarmServiceConfiguration::mCircleGrid, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("DarkMode", &nap::SwarmServiceConfiguration::mDarkMode, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("CapFPS", &nap::SwarmServiceConfiguration::mCapFPS, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("EditParameters", &nap::SwarmServiceConfiguration::mEditParameters, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("Labels", &nap::SwarmServiceConfiguration::mLabels, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("SelectedData", &nap::SwarmServiceConfiguration::mSelectedData, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS


namespace nap
{
	
	rtti::TypeInfo SwarmServiceConfiguration::getServiceType() const
	{
		return RTTI_OF(swarmService);
	}
	
	
	bool swarmService::init(nap::utility::ErrorState& errorState)
	{
		return true;
	}
	
	
	void swarmService::update(double deltaTime)
	{
	}
	
	
	void swarmService::getDependentServices(std::vector<rtti::TypeInfo>& dependencies)
	{
	}
	
	
	void swarmService::shutdown()
	{
	}
	
}
