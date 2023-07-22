#pragma once

#include <nap/resource.h>

#include <parameternumeric.h>
#include <parametervec.h>

namespace nap {
    


    // TODO: more data types / generic Parameter data type.
    // TODO: optimise OSC parsing by using Signals instead of searching by name every cycle
    class NAPAPI ParameterData : public Resource {
        RTTI_ENABLE(Resource)
        
    public:
        ParameterData() : Resource() { }
        
        void initParameters(std::vector<std::tuple<std::string, float, float, glm::vec3>> vec3Parameters, std::vector<std::tuple<std::string, float, float, float>> floatParameters)
        {
            mVec3Parameters.clear();
            for(auto& s : vec3Parameters)
            {
                auto param = std::make_unique<ParameterVec3>();
                param->mID = std::get<0>(s);
                param->mName = std::get<0>(s);
                param->mValue = std::get<3>(s);
                param->mMinimum = std::get<1>(s);
                param->mMaximum = std::get<2>(s);
                mVec3Parameters.emplace(std::get<0>(s), std::move(param));
            }
            
            mFloatParameters.clear();
            for(auto& s : floatParameters)
            {
                auto param = std::make_unique<ParameterFloat>();
                param->mID = std::get<0>(s);
                param->mName = std::get<0>(s);
                param->mValue = std::get<3>(s);
                param->mMinimum = std::get<1>(s);
                param->mMaximum = std::get<2>(s);
                mFloatParameters.emplace(std::get<0>(s), std::move(param));
            }
        }
        
        void setFloat(const std::string& name, float value)
        {
            mFloatParameters[name]->setValue(value);
        }
        
        void setVec3(const std::string& name, glm::vec3 value)
        {
            mVec3Parameters[name]->setValue(value);
        }
        
        float getFloat(const std::string& name)
        {
            return mFloatParameters[name]->mValue;
        }
        
        glm::vec3 getVec3(const std::string& name)
        {
            return mVec3Parameters[name]->mValue;
        }
        
    private:
        std::unordered_map<std::string, std::unique_ptr<ParameterVec3>> mVec3Parameters;
        std::unordered_map<std::string, std::unique_ptr<ParameterFloat>> mFloatParameters;
    };
    
}
