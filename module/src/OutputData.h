#pragma once

#include <nap/resource.h>

#include <glm/glm.hpp>

namespace nap {
    
    /**
     * Resource that holds all data to be sent out. Data is set by the python script and read out by the monitor and OSC sender.
     */
    class NAPAPI OutputData : public Resource {
        RTTI_ENABLE(Resource)

    public:
        OutputData() : Resource() { }
        
        // TODO: also bool type, string type?
        void initFields(std::vector<std::string> vec3Fields, std::vector<std::string> floatFields, int size)
        {
            mVec3Fields.clear();
            for(auto& s : vec3Fields)
                mVec3Fields.insert({s, std::vector<glm::vec3>(size)});
            
            mFloatFields.clear();
            for(auto& s : floatFields)
                mFloatFields.insert({s, std::vector<float>(size)});
        }
        
        // TODO: optimize
        void setFloat(int index, std::string fieldName, float value)
        {
            mFloatFields[fieldName][index] = value;
        }
        
        // TODO: optimize?
        void setVec3(int index, std::string fieldName, glm::vec3 value)
        {
            mVec3Fields[fieldName][index] = value;
        }
        

        const std::vector<glm::vec3>& getVec3Field(std::string name)
        {
            return mVec3Fields[name];
        }
        
        const std::vector<float>& getFloatField(std::string name)
        {
            return mFloatFields[name];
        }
        
        const std::unordered_map<std::string, std::vector<glm::vec3>>& getVec3Fields()
        {
            return mVec3Fields;
        }
        
        const std::unordered_map<std::string, std::vector<float>>& getFloatFields()
        {
            return mFloatFields;
        }
        
    private:
        std::unordered_map<std::string, std::vector<glm::vec3>> mVec3Fields;
        std::unordered_map<std::string, std::vector<float>> mFloatFields;
    };
    
}
