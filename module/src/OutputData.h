#pragma once

#include <nap/resource.h>

#include <glm/glm.hpp>

namespace nap {
    
    /**
     * Resource that holds all data to be sent out. Data is set by the python script and read out by the monitor and OSC sender.
     */
    // TODO: also bool type, string type?
    class NAPAPI OutputData : public Resource {
        RTTI_ENABLE(Resource)

    public:
        OutputData() : Resource() { }
        
        void initFields(std::vector<std::string> vec3Fields, std::vector<std::string> floatFields, int size);
        
        /**
         * Safely set float value of field at index.
         * Returns false if unsuccesfully set.
         * TODO: could be optimised.
         */
        bool setFloat(int index, const std::string& fieldName, float value);
        
        /**
         * Safely set vec3 value of field at index.
         * Returns false if unsuccesfully set.
         * TODO: could be optimised.
         */
        bool setVec3(int index, const std::string& fieldName, glm::vec3 value);
        
        /**
         * Unsafe getter for vec3 field.
         */
        const std::vector<glm::vec3>& getVec3Field(const std::string& name);
        
        /**
         * Unsafe getter for float field
         */
        const std::vector<float>& getFloatField(const std::string& name);
        
        const std::unordered_map<std::string, std::vector<glm::vec3>>& getVec3Fields(){ return mVec3Fields; }
        
        const std::unordered_map<std::string, std::vector<float>>& getFloatFields(){ return mFloatFields; }
        
    private:
        int mSize;
        std::unordered_map<std::string, std::vector<glm::vec3>> mVec3Fields;
        std::unordered_map<std::string, std::vector<float>> mFloatFields;
    };
    
}
