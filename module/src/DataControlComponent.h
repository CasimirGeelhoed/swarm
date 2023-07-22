#pragma once

// Nap includes
#include <component.h>
#include <nap/resourceptr.h>

#include <OutputData.h>
#include <ParameterData.h>

namespace nap
{
    
    class DataControlComponentInstance;
    
    
    /**
     * Component that interfaces with the 'output' data and the parameters. Is controlled by the python script.
     */
    class NAPAPI DataControlComponent : public Component
    {
        RTTI_ENABLE(Component)
        DECLARE_COMPONENT(DataControlComponent, DataControlComponentInstance)
        
    public:
        DataControlComponent() : Component() { }
        
        nap::ResourcePtr<ParameterData> mParameterData; ///< Property: pointer to the parameters.
        nap::ResourcePtr<OutputData> mOutputData; ///< Property: pointer to the output data.
        
    private:
    };

    
    class NAPAPI DataControlComponentInstance : public ComponentInstance
    {
        RTTI_ENABLE(ComponentInstance)
    public:
        DataControlComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource)
        {
            mOutputData = getComponent<DataControlComponent>()->mOutputData.get();
            mParameterData = getComponent<DataControlComponent>()->mParameterData.get();
        }
        
        // Initialize the component
        bool init(utility::ErrorState& errorState) override;
        
        void initData(std::vector<std::string> vec3Fields, std::vector<std::string> floatFields, int size)
        {
            mOutputData->initFields(vec3Fields, floatFields, size);
        }

        void initParameters(std::vector<std::tuple<std::string, float, float, glm::vec3>> vec3Parameters, std::vector<std::tuple<std::string, float, float, float>> floatParameters)
        {
            mParameterData->initParameters(vec3Parameters, floatParameters);
        }
        
        /**
         * Sets the value for index i in field fieldName.
         */
        void setFloat(int index, const std::string& fieldName, float value)
        {
            mOutputData->setFloat(index, fieldName, value);
        }
        
        /**
         * Sets the value for index i in field fieldName.
         */
        void setVec3(int index, const std::string& fieldName, glm::vec3 value)
        {
            mOutputData->setVec3(index, fieldName, value);
        }

        
        /**
         * Returns the parameter value.
         */
        float getFloat(const std::string& name)
        {
            return mParameterData->getFloat(name);
        }

        /**
         * Returns the parameter value.
         */
        glm::vec3 getVec3(const std::string& name)
        {
            return mParameterData->getVec3(name);
        }

    private:
        OutputData* mOutputData = nullptr;
        ParameterData* mParameterData = nullptr;
    };
        
}
