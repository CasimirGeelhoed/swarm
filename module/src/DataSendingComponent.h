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
    /**
     * Component that reads out all @OutputData and sends it out over OSC.
     */
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
        
        void setOutputRate(double outputRate) { mOutputInterval = 1.0 / outputRate; }
        
    private:
        
        double mOutputInterval = 0.5f; // number of seconds before next output
        double mTimer = 0.f;
        
        OSCSender* mOSCSender = nullptr;
        OutputData* mData = nullptr;
    };
        
}
