#include "LabelsRenderingComponent.h"

// Nap includes
#include <entity.h>

RTTI_BEGIN_CLASS(nap::LabelsRenderingComponent)
    RTTI_PROPERTY("RenderableTextComponent", &nap::LabelsRenderingComponent::mRenderableTextComponent, nap::rtti::EPropertyMetaData::Required)
    RTTI_PROPERTY("Data", &nap::LabelsRenderingComponent::mData, nap::rtti::EPropertyMetaData::Required)
    RTTI_PROPERTY("LabelOffset", &nap::LabelsRenderingComponent::mLabelOffset, nap::rtti::EPropertyMetaData::Default)
    RTTI_PROPERTY("Indices", &nap::LabelsRenderingComponent::mIndices, nap::rtti::EPropertyMetaData::Default)
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
        
        // Save label offset & indices setting
        mLabelOffset = resource->mLabelOffset;
        mIndices = resource->mIndices;
        
        return true;
        
    }

    
    std::string roundToTwoDecimals(float number)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << (number + 0.05f);
        return ss.str();
    }


    void LabelsRenderingComponentInstance::setDataToRender(std::string name, bool isVec3)
    {
        if(name == "")
        {
            mDisplay = false;
            mTexts.clear();
            return;
        }
        
        if(isVec3)
        {
            auto field = mData->getVec3Field(name);
            mTexts.clear();
            mTexts.reserve(field.size());
            for(int i = 0; i < field.size(); i++)
                mTexts.emplace_back(roundToTwoDecimals(field[i].x) + " " + roundToTwoDecimals(field[i].y) + " " + roundToTwoDecimals(field[i].z));
        }
        else
        {
            auto field = mData->getFloatField(name);
            mTexts.clear();
            mTexts.reserve(field.size());
            for(int i = 0; i < field.size(); i++)
                mTexts.emplace_back(roundToTwoDecimals(field[i]));
        }

        mDisplay = true;
    }


    void LabelsRenderingComponentInstance::update(double deltaTime)
    {
        mPositions = mData->getVec3Field("position");
        mCount = mPositions.size();
        mRenderableTextComponent->resize(mCount);
        
        utility::ErrorState e;
        if(mIndices)
        {
            for(int i = 0; i < mCount; i++)
                mRenderableTextComponent->setText(i,std::to_string(i + 1), e);
        }
        else
        {
            for(int i = 0; i < std::min<int>(mCount, mTexts.size()); i++)
                mRenderableTextComponent->setText(i, mTexts[i], e);
        }
    }

    void LabelsRenderingComponentInstance::draw(IRenderTarget& renderTarget, PerspCameraComponentInstance& perspCamera, glm::vec3 color)
    {
        if(!mDisplay)
            return;
        
        for(int i = 0; i < mCount; i++)
        {
            // Get the color based on preference (tag or regular)
            mRenderableTextComponent->setColor(color);
            glm::vec3 pos = glm::vec4(mPositions[i], 1.0f);
            glm::vec3 loc = perspCamera.worldToScreen(pos, math::Rect(0, 0, renderTarget.getBufferSize().x, renderTarget.getBufferSize().y));
            if (loc.z < 1.0f)
            {
                mRenderableTextComponent->setLocation(glm::vec2(loc) + mLabelOffset);
                mRenderableTextComponent->setLineIndex(i);
                mRenderableTextComponent->draw(renderTarget);
            }
        }
    }
        
}
