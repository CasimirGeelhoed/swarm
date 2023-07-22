#pragma once

// Nap includes
#include <component.h>
#include <nap/resourceptr.h>

#include <oscsender.h>

#include "OutputData.h"

namespace nap
{
    
    class DataSendingComponentInstance;

    
    // TODO: adjustable output rate?
    class NAPAPI DataSendingComponent : public Component
    {
        RTTI_ENABLE(Component)
        DECLARE_COMPONENT(DataSendingComponent, DataSendingComponentInstance)
        
    public:
        DataSendingComponent() : Component() { }
        
        ResourcePtr<OSCSender> mOSCSender; ///< The OSC Sender.
        ResourcePtr<OutputData> mData; ///< The data set.
    };


    // TODO: create adjustable translations of name&index -> address, to be compatible with different software.
    class NAPAPI DataSendingComponentInstance : public ComponentInstance
    {
        RTTI_ENABLE(ComponentInstance)
    public:
        DataSendingComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource) { }
        
        bool init(utility::ErrorState& errorState) override;
        
        void update(double deltaTime) override;
        
    private:
        OSCSender* mOSCSender = nullptr;
        OutputData* mData = nullptr;
    };
        
}
