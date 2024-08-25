#pragma once

// External Includes
#include <nap/resource.h>
#include <renderablemesh.h>

namespace nap
{
	
	/**
	 * Simple square grid mesh.
	 */
	class NAPAPI GridMesh : public IMesh
	{
		RTTI_ENABLE(IMesh)
	public:
		// Constructor
		GridMesh(Core& core);
		
		virtual bool init(utility::ErrorState& errorState) override;
		
		int mLines = 20; ///< Property: The number of lines in the grid.
		
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
