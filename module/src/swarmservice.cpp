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
    RTTI_PROPERTY("Gnomon", &nap::SwarmServiceConfiguration::mGnomon, nap::rtti::EPropertyMetaData::Default)
    RTTI_PROPERTY("CircleGrid", &nap::SwarmServiceConfiguration::mCircleGrid, nap::rtti::EPropertyMetaData::Default)
    RTTI_PROPERTY("Shadows", &nap::SwarmServiceConfiguration::mShadows, nap::rtti::EPropertyMetaData::Default)
    RTTI_PROPERTY("DarkMode", &nap::SwarmServiceConfiguration::mDarkMode, nap::rtti::EPropertyMetaData::Default)
    RTTI_PROPERTY("Labels", &nap::SwarmServiceConfiguration::mLabels, nap::rtti::EPropertyMetaData::Default)
    RTTI_PROPERTY("OSCOutputAddress", &nap::SwarmServiceConfiguration::mOSCOutputAddress, nap::rtti::EPropertyMetaData::Default)
    RTTI_PROPERTY("OSCOutputPort", &nap::SwarmServiceConfiguration::mOSCOutputPort, nap::rtti::EPropertyMetaData::Default)
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
