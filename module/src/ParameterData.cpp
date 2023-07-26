#include "ParameterData.h"

RTTI_BEGIN_CLASS(nap::ParameterData)
RTTI_END_CLASS

namespace nap {

    void ParameterData::initParameters(std::vector<std::tuple<std::string, float, float, glm::vec3>> vec3Parameters, std::vector<std::tuple<std::string, float, float, float>> floatParameters)
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


    bool ParameterData::setFloat(const std::string& name, float value)
    {
        auto it = mFloatParameters.find(name);
        if(it == mFloatParameters.end())
            return false;
        
        it->second->setValue(value);
        return true;
    }


    bool ParameterData::setVec3(const std::string& name, glm::vec3 value)
    {
        auto it = mVec3Parameters.find(name);
        if(it == mVec3Parameters.end())
            return false;

        it->second->setValue(value);
        return true;
    }


    float ParameterData::getFloat(const std::string& name)
    {
        auto it = mFloatParameters.find(name);
        if(it == mFloatParameters.end())
            return 0.f;

        return it->second->mValue;
    }


    glm::vec3 ParameterData::getVec3(const std::string& name)
    {
        auto it = mVec3Parameters.find(name);
        if(it == mVec3Parameters.end())
            return glm::vec3(0.f);

        return it->second->mValue;
    }
    
}
