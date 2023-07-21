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
        
        void initData(std::vector<std::string> vec3Fields, std::vector<std::string> floatFields, int size)
        {
            mData->initFields(vec3Fields, floatFields, size);
        }

        void setFloat(int index, std::string fieldName, float value)
        {
            mData->setFloat(index, fieldName, value);
        }
        
        void setVec3(int index, std::string fieldName, glm::vec3 value)
        {
            mData->setVec3(index, fieldName, value);
        }


    private:
        OutputData* mData;
    };
        
}
