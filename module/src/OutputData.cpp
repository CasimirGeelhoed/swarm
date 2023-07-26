#include "OutputData.h"

RTTI_BEGIN_CLASS(nap::OutputData)
RTTI_END_CLASS

namespace nap {

    void OutputData::initFields(std::vector<std::string> vec3Fields, std::vector<std::string> floatFields, int size)
    {
        mSize = size;
        
        mVec3Fields.clear();
        for(auto& s : vec3Fields)
            mVec3Fields.emplace(s, std::vector<glm::vec3>(mSize));
        
        mFloatFields.clear();
        for(auto& s : floatFields)
            mFloatFields.emplace(s, std::vector<float>(mSize));
    }


    bool OutputData::setFloat(int index, const std::string& fieldName, float value)
    {
        if(index < 0 || index >= mSize)
            return false;
        
        auto it = mFloatFields.find(fieldName);
        if(it == mFloatFields.end())
            return false;
        
        it->second[index] = value;
        return true;
    }


    bool OutputData::setVec3(int index, const std::string& fieldName, glm::vec3 value)
    {
        if(index < 0 || index >= mSize)
            return false;
        
        auto it = mVec3Fields.find(fieldName);
        if(it == mVec3Fields.end())
            return false;

        it->second[index] = value;
        return true;
    }


    const std::vector<glm::vec3>& OutputData::getVec3Field(const std::string& name)
    {
        return mVec3Fields[name];
    }


    const std::vector<float>& OutputData::getFloatField(const std::string& name)
    {
        return mFloatFields[name];
    }
    
}

