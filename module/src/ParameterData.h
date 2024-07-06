#pragma once

#include <nap/resource.h>

#include <parameternumeric.h>
#include <parametervec.h>

namespace nap {
    

    // TODO: more data types?

    /**
     * Data structure for input parameters.
     */
    class NAPAPI ParameterData : public Resource {
        RTTI_ENABLE(Resource)
        
    public:
        ParameterData() : Resource() { }
        
		/**
		 * Adds vec3 parameter.
		 */
		void addVec3Parameter(const std::string& name, float min, float max, glm::vec3 value);
		
		/**
		 * Adds float parameter.
		 */
		void addFloatParameter(const std::string& name, float min, float max, float value);
		
        /**
         * Safely set float value of parameter.
         * Returns false if unsuccesful.
         * TODO: could be optimised.
         */
        bool setFloat(const std::string& name, float value);
        
        /**
         * Safely set vec3 value of parameter.
         * Returns false if unsuccesful.
         * TODO: could be optimised.
         */
        bool setVec3(const std::string& name, glm::vec3 value);
        
        /**
         * Safe function to get float parameter value
         * TODO: log error if not found.
         */
        float getFloat(const std::string& name);
        
        /**
         * Safe function to get vec3 parameter value
         * TODO: log error if not found.
         */
        glm::vec3 getVec3(const std::string& name);
        
        
        const std::map<std::string, std::unique_ptr<ParameterVec3>>& getVec3Parameters() { return mVec3Parameters; }
        
        const std::map<std::string, std::unique_ptr<ParameterFloat>>& getFloatParameters() { return mFloatParameters; }
        
    private:
        std::map<std::string, std::unique_ptr<ParameterVec3>> mVec3Parameters;
        std::map<std::string, std::unique_ptr<ParameterFloat>> mFloatParameters;
    };
    
}
