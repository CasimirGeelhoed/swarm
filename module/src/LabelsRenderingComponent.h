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
        
        glm::vec2 mLabelOffset = glm::vec2(0.f); ///< Property: 2D labels offset in relation to center position.
        
        bool mIndices = false; ///< Property: if set to true, the Component will render indices instead of selected data.
        
    private:
    };

    
    class NAPAPI LabelsRenderingComponentInstance : public ComponentInstance
    {
        RTTI_ENABLE(ComponentInstance)
    public:
        LabelsRenderingComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource) { }
        
        bool init(utility::ErrorState& errorState) override;
        
        void update(double deltaTime) override;
        
        /**
		 * Updates the texts that will be displayed by retrieving the values of the data vield with the given name and type.
		 */
        void setDataToRender(std::string name, bool isVec3);
        
        /**
		 * Draws text using the Renderable2DTextComponent. Should be called from the app's draw() function.
		 */
        void draw(IRenderTarget& renderTarget, PerspCameraComponentInstance& perspCamera, glm::vec3 color);
        
    private:
        ComponentInstancePtr<Renderable2DTextComponent> mRenderableTextComponent = { this, &LabelsRenderingComponent::mRenderableTextComponent }; ///< Property: 'RenderableTextComponent' The component that takes care of text rendering.

        OutputData* mData = nullptr;
        glm::vec2 mLabelOffset;
        
        int mCount = 0;
        std::vector<glm::vec3> mPositions; // TODO: is it really necessary to copy this data?
        bool mDisplay = true;
        bool mIndices = false;
        
        std::string mFieldName = "";
        bool mFieldIsVec3 = false;
        
    };
        
}
