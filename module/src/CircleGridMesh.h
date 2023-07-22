#pragma once

// External Includes
#include <nap/resource.h>
#include <renderablemesh.h>

namespace nap
{
    /**
     * Simple Circle mesh.
     */
    class NAPAPI CircleGridMesh : public IMesh
    {
        RTTI_ENABLE(IMesh)
    public:
        // Constructor
        CircleGridMesh(Core& core);

        /**
        * Creates the mesh.
        * @param errorState contains the error message when initialization fails.
        * @return if initialization succeeded.
        */
        virtual bool init(utility::ErrorState& errorState) override;

        
        int mSectors = 20; ///< Property.
        
        int mCircles = 1; ///< Property.
        
        int mMeridians = 0; ///< Property.
        
        
        /**
         * @return the mesh instance
         */
        virtual MeshInstance& getMeshInstance()    override                { return *mMeshInstance; }

        /**
         * @return the mesh instance
         */
        virtual const MeshInstance& getMeshInstance() const override    { return *mMeshInstance; }
        
    private:
        std::unique_ptr<nap::MeshInstance> mMeshInstance = nullptr;
        nap::RenderService* mRenderService = nullptr;
    };
}
