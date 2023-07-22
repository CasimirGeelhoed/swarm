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


    void DataReceivingComponentInstance::onMessageReceived(const OSCEvent&)
    {
        // TODO: ..
        std::cout << "check " << std::endl;
    }




}
