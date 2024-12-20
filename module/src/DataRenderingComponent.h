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

// External includes
#include <rendercomponent.h>
#include <renderablemesh.h>
#include <rtti/objectptr.h>
#include <componentptr.h>
#include <transformcomponent.h>
#include <color.h>
#include <uniforminstance.h>
#include <materialinstance.h>

#include "OutputData.h"


namespace nap
{
	
	class DataRenderingComponentInstance;
	
	/**
	 * Component that renders 3D objects based on certain fields of @OutputData.
	 */
	class NAPAPI DataRenderingComponent : public RenderableComponent
	{
		RTTI_ENABLE(RenderableComponent)
		DECLARE_COMPONENT(DataRenderingComponent, DataRenderingComponentInstance)
	public:
		virtual void getDependentComponents(std::vector<rtti::TypeInfo>& components) const override;
		
		// Component Properties
		MaterialInstanceResource mMaterialInstanceResource; ///< Property: 'MaterialInstance' the material used to shade the text
		rtti::ObjectPtr<IMesh> mMesh; ///< Property: 'Mesh' mesh to copy onto target
		
		bool mDynamicColoring = true; ///< Property: Whether the coloring of elements will be dependant on real-time data.
		
		ResourcePtr<OutputData> mData;  ///< Property: 'Data' pointer to the data resource.
	};
	
	
	class NAPAPI DataRenderingComponentInstance : public RenderableComponentInstance
	{
		RTTI_ENABLE(RenderableComponentInstance)
	public:
		DataRenderingComponentInstance(EntityInstance& entity, Component& resource);
		
		virtual bool init(utility::ErrorState& errorState) override;
		
		virtual void update(double deltaTime) override;
		
	protected:
		
		virtual void onDraw(IRenderTarget& renderTarget, VkCommandBuffer commandBuffer, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
		
	private:
		TransformComponentInstance* mTransform = nullptr; // Transform.
		MaterialInstance mMaterialInstance; // Material instance created from resource.
		RenderableMesh mMesh; // Mesh to draw.
		
		UniformMat4Instance* mModelMatUniform = nullptr; // Pointer to the model matrix uniform.
		UniformMat4Instance* mViewMatUniform = nullptr; // Pointer to the view matrix uniform.
		UniformMat4Instance* mProjectMatUniform = nullptr; // Pointer to the projection matrix uniform
		
		UniformVec3Instance* mColorUniform = nullptr; // Color uniform slot
		glm::vec3 mBaseColor;
		glm::vec3 mDefaultScale = glm::vec3(0.1f);
		bool mDynamicColoring = true;
		;
		
		RenderService* mRenderService = nullptr; // RenderService.
		
		OutputData* mData = nullptr; // Data pointer.
	};
}
