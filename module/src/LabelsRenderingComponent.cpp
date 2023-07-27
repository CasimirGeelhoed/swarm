#include "LabelsRenderingComponent.h"

// Nap includes
#include <entity.h>

RTTI_BEGIN_CLASS(nap::LabelsRenderingComponent)
    RTTI_PROPERTY("RenderableTextComponent", &nap::LabelsRenderingComponent::mRenderableTextComponent, nap::rtti::EPropertyMetaData::Required)
    RTTI_PROPERTY("Data", &nap::LabelsRenderingComponent::mData, nap::rtti::EPropertyMetaData::Required)
    RTTI_PROPERTY("LabelOffset", &nap::LabelsRenderingComponent::mLabelOffset, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::LabelsRenderingComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

namespace nap
{
    
    bool LabelsRenderingComponentInstance::init(utility::ErrorState& errorState)
    {
        // Get resource
        LabelsRenderingComponent* resource = getComponent<LabelsRenderingComponent>();
        
        // Get data pointer
        mData = resource->mData.get();
        
        // Save label offset
        mLabelOffset = resource->mLabelOffset;

        return true;
        
    }

    void LabelsRenderingComponentInstance::update(double deltaTime)
    {
        mPositions = mData->getVec3Field("position");
        mCount = mPositions.size();
        mRenderableTextComponent->resize(mCount);
        
        utility::ErrorState e;
        for(int i = 0; i < mPositions.size(); i++)
            mRenderableTextComponent->setText(i, std::to_string(i + 1), e);
    }

    void LabelsRenderingComponentInstance::draw(IRenderTarget& renderTarget, PerspCameraComponentInstance& perspCamera, glm::vec3 color)
    {
        for(int i = 0; i < mCount; i++)
        {
            // Get the color based on preference (tag or regular)
            mRenderableTextComponent->setColor(color);
            glm::vec3 pos = glm::vec4(mPositions[i], 1.0f);
            glm::vec3 loc = perspCamera.worldToScreen(pos, math::Rect(0, 0, renderTarget.getBufferSize().x, renderTarget.getBufferSize().y));
            if (loc.z < 1.0f)
            {
                mRenderableTextComponent->setLocation(glm::vec2(loc) + mLabelOffset);
                mRenderableTextComponent->setOrientation(utility::ETextOrientation::Center);
                mRenderableTextComponent->setLineIndex(i);
                mRenderableTextComponent->draw(renderTarget);
            }
        }
    }
        
}
