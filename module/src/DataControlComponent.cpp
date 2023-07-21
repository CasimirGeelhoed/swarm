#include "DataControlComponent.h"

// Nap includes
#include <entity.h>

// RTTI
RTTI_BEGIN_CLASS(nap::DataControlComponent)
    RTTI_PROPERTY("Data", &nap::DataControlComponent::mData, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::DataControlComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
    RTTI_FUNCTION("resizeData", &nap::DataControlComponentInstance::resizeData)
    RTTI_FUNCTION("setData", &nap::DataControlComponentInstance::setData)
RTTI_END_CLASS

namespace nap
{
    
    bool DataControlComponentInstance::init(utility::ErrorState& errorState)
    {
        return true;
    }
        
}
