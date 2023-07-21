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

namespace nap
{
	bool swarmService::init(nap::utility::ErrorState& errorState)
	{
		//Logger::info("Initializing swarmService");
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
