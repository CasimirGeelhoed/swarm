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
        

        /**
         * Set the amount of positions. Should be called before calling setPosition() (at initialisation).
         */
        void resize(int size)
        {
            mPositions.resize(size);
        }
        
        /**
         * Unsafe method to set position at index.
         */
        void setData(int index, glm::vec3 value)
        {
            mPositions[index] = value;
        }
        
        /**
         * Function to read positions.
         */
        const std::vector<glm::vec3>& getData()
        {
            return mPositions;
        }
        
    private:
        std::vector<glm::vec3> mPositions;
    };
    
}
