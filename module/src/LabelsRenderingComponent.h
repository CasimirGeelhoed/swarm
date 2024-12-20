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

// Nap includes
#include <component.h>
#include <componentptr.h>

#include <renderable2dtextcomponent.h>
#include <perspcameracomponent.h>

#include "OutputData.h"

namespace nap
{
	
	class LabelsRenderingComponentInstance;
	
	/**
	 * Component that renders text labels next to sound sources.
	 */
	class NAPAPI LabelsRenderingComponent : public Component
	{
		RTTI_ENABLE(Component)
		DECLARE_COMPONENT(LabelsRenderingComponent, LabelsRenderingComponentInstance)
		
	public:
		LabelsRenderingComponent() : Component() { }
		
		ComponentPtr<Renderable2DTextComponent> mRenderableTextComponent = nullptr; ///< Property: 'RenderableTextComponent' The component that takes care of text rendering.
		ResourcePtr<OutputData> mData; ///< Property: 'Data' pointer to the data resource
		
		glm::vec2 mLabelOffset = glm::vec2(0.f); ///< Property: 'LabelOffset' 2D labels offset in relation to center position.
		
		bool mIndices = false; ///< Property: if set to true, the Component will render indices instead of selected data.
		
	};
	
	
	class NAPAPI LabelsRenderingComponentInstance : public ComponentInstance
	{
		RTTI_ENABLE(ComponentInstance)
	public:
		LabelsRenderingComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource) { }
		
		bool init(utility::ErrorState& errorState) override;
		
		void update(double deltaTime) override;
		
		/**
		 * Updates the texts that will be displayed by retrieving the values of the data field with the given name and type.
		 * @param name the name of the data field
		 * @param isVec3 whether the data field is a vec3
		 */
		void setDataToRender(std::string name, bool isVec3);
		
		/**
		 * Draws text using the Renderable2DTextComponent. Should be called from the app's draw() function.
		 * @param renderTarget the render target to draw to
		 * @param perspCamera the camera
		 * @param color the color of the text
		 */
		void draw(IRenderTarget& renderTarget, PerspCameraComponentInstance& perspCamera, glm::vec3 color);
		
		glm::vec2 mLabelOffset;
		
	private:
		ComponentInstancePtr<Renderable2DTextComponent> mRenderableTextComponent = { this, &LabelsRenderingComponent::mRenderableTextComponent }; ///< Property: 'RenderableTextComponent' The component that takes care of text rendering.
		
		OutputData* mData = nullptr;
		
		int mCount = 0;
		std::vector<glm::vec3> mPositions; // TODO: is it really necessary to copy this data?
		bool mDisplay = true;
		bool mIndices = false;
		
		std::string mFieldName = "";
		bool mFieldIsVec3 = false;
		
	};
	
}
