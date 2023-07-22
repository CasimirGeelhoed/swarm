#pragma once

// External Includes
#include <nap/resource.h>
#include <renderablemesh.h>

namespace nap
{
    class NAPAPI GridMesh : public IMesh
    {
        RTTI_ENABLE(IMesh)
    public:
        // Constructor
        GridMesh(Core& core);

        /**
        * Creates the mesh.
        * @param errorState contains the error message when initialization fails.
        * @return if initialization succeeded.
        */
        virtual bool init(utility::ErrorState& errorState) override;

        
        int mLines = 20; ///< Property.
        
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
