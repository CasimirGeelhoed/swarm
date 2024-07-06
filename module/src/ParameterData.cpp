#include "ParameterData.h"

RTTI_BEGIN_CLASS(nap::ParameterData)
RTTI_END_CLASS

namespace nap {

	void ParameterData::addVec3Parameter(const std::string& name, float min, float max, glm::vec3 value)
	{
		auto param = std::make_unique<ParameterVec3>();
		param->mID = name;
		param->mName = name;
		param->mValue = value;
		param->mMinimum = min;
		param->mMaximum = max;
		mVec3Parameters.emplace(name, std::move(param));
	}


	void ParameterData::addFloatParameter(const std::string& name, float min, float max, float value)
	{
		auto param = std::make_unique<ParameterFloat>();
		param->mID = name;
		param->mName = name;
		param->mValue = value;
		param->mMinimum = min;
		param->mMaximum = max;
		mFloatParameters.emplace(name, std::move(param));
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
