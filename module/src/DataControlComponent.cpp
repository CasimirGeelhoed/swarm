#include "DataControlComponent.h"

// Nap includes
#include <entity.h>

// RTTI
RTTI_BEGIN_CLASS(nap::DataControlComponent)
    RTTI_PROPERTY("ParameterData", &nap::DataControlComponent::mParameterData, nap::rtti::EPropertyMetaData::Required)
    RTTI_PROPERTY("OutputData", &nap::DataControlComponent::mOutputData, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::DataControlComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
    RTTI_FUNCTION("initData", &nap::DataControlComponentInstance::initData)
    RTTI_FUNCTION("initParameters", &nap::DataControlComponentInstance::initParameters)
    RTTI_FUNCTION("setFloat", &nap::DataControlComponentInstance::setFloat)
    RTTI_FUNCTION("setVec3", &nap::DataControlComponentInstance::setVec3)
    RTTI_FUNCTION("getFloat", &nap::DataControlComponentInstance::getFloat)
    RTTI_FUNCTION("getVec3", &nap::DataControlComponentInstance::getVec3)
RTTI_END_CLASS

namespace nap
{
    
    bool DataControlComponentInstance::init(utility::ErrorState& errorState)
    {
        return true;
    }
        
}
