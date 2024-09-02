/*
 This file is part of Swarm, a free scriptable OSC controller with 3D visualisation.
 Copyright (C) 2024 Casimir Geelhoed
 
 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or any later version.
 
 This program is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of  MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along with
 this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// External Includes
#include <nap/resource.h>
#include <renderablemesh.h>

namespace nap
{
	/**
	 * Simple circular grid mesh.
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
