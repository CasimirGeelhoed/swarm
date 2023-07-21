#pragma once

// Nap includes
#include <component.h>
#include <nap/resourceptr.h>

#include <OutputData.h>

namespace nap
{
    
    class DataControlComponentInstance;
    
    
    class NAPAPI DataControlComponent : public Component
    {
        RTTI_ENABLE(Component)
        DECLARE_COMPONENT(DataControlComponent, DataControlComponentInstance)
        
    public:
        DataControlComponent() : Component() { }
        
        nap::ResourcePtr<OutputData> mData;
        
    private:
    };

    
    class NAPAPI DataControlComponentInstance : public ComponentInstance
    {
        RTTI_ENABLE(ComponentInstance)
    public:
        DataControlComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource)
        {
            mData = getComponent<DataControlComponent>()->mData.get();
        }
        
        // Initialize the component
        bool init(utility::ErrorState& errorState) override;
        
        void resizeData(int size)
        {
            mData->resize(size);
        }
        
        void setData(int index, glm::vec3 value)
        {
            mData->setData(index, value);
        }

    private:
        OutputData* mData;
    };
        
}
