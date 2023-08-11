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

    void DataSendingComponentInstance::update(double deltaTime)
    {
        // read all data and send it out as osc
        for(auto& it : mData->getFloatFields())
            for(int i = 0; i < it.second.size(); i++)
                sendOSC(*mOSCSender, "/source" + std::to_string(i+1) + "/" + it.first, it.second[i]);
        
        for(auto& it : mData->getVec3Fields())
        {
            for(int i = 0; i < it.second.size(); i++)
            {
                sendOSC(*mOSCSender, "/source" + std::to_string(i+1) + "/" + it.first, it.second[i]);
                sendOSC(*mOSCSender, "/source" + std::to_string(i+1) + "/" + it.first + "/x", it.second[i].x);
                sendOSC(*mOSCSender, "/source" + std::to_string(i+1) + "/" + it.first + "/y", it.second[i].y);
                sendOSC(*mOSCSender, "/source" + std::to_string(i+1) + "/" + it.first + "/z", it.second[i].z);
            }
        }
    }

}
