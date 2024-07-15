#include "DataSendingComponent.h"

// Nap includes
#include <entity.h>

// RTTI
RTTI_BEGIN_CLASS(nap::DataSendingComponent)
	RTTI_PROPERTY("OSCSender", &nap::DataSendingComponent::mOSCSender, nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("Data", &nap::DataSendingComponent::mData, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::DataSendingComponentInstance)
	RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

namespace nap
{

	bool DataSendingComponentInstance::init(utility::ErrorState& errorState)
	{
		mOSCSender = getComponent<DataSendingComponent>()->mOSCSender.get();
		mData = getComponent<DataSendingComponent>()->mData.get();
		
		return true;
	}

	
	// send out a vec3 as an OSC message
	static void sendOSC(OSCSender& sender, const std::string& address, const glm::vec3& value)
	{
		OSCEvent event(address);
		event.addValue<float>(value.x);
		event.addValue<float>(value.y);
		event.addValue<float>(value.z);
		sender.send(event);
	}

	
	// send out a float as an OSC message
	static void sendOSC(OSCSender& sender, const std::string& address, float value)
	{
		OSCEvent event(address);
		event.addValue<float>(value);
		sender.send(event);
	}

	
	// Helper function that converts a field name to an address.
	// It replaces '$' with the index if '$' was found. Otherwise it prefixes the address with '/sourceINDEX'.
	std::string fieldNameToAddress(const std::string& fieldName, int index)
	{
		std::string address = fieldName;

		// prefix with '/' if it doesn't have it
		if(address[0] != '/')
			address = "/" + address;

		// find '$' and replace it with the index. If '$' ws not found, prefix with '/sourceINDEX'
		size_t pos = address.find('$');
		if(address.find('$') == std::string::npos)
			address = "/source" + std::to_string(index) + address;
		else
			address.replace(pos, 1, std::to_string(index));
		
		return address;
	}
	
	
	void DataSendingComponentInstance::update(double deltaTime)
	{
		mTimer += deltaTime;
		
		if(mTimer > mOutputInterval)
		{
			mTimer = 0.f;
			
			// read all data and send it out as osc
			for(auto& it : mData->getFloatFields())
				for(int i = 0; i < it.second.size(); i++)
					sendOSC(*mOSCSender, fieldNameToAddress(it.first, i+1), it.second[i]);
			
			for(auto& it : mData->getVec3Fields())
				for(int i = 0; i < it.second.size(); i++)
					sendOSC(*mOSCSender, fieldNameToAddress(it.first, i+1), it.second[i]);
		}
	}

}
