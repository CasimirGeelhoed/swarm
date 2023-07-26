#include "PythonErrorHandlerComponent.h"

// Nap includes
#include <entity.h>

// RTTI
RTTI_BEGIN_CLASS(nap::PythonErrorHandlerComponent)
    RTTI_PROPERTY("PythonScriptComponent", &nap::PythonErrorHandlerComponent::mPythonScriptComponent, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::PythonErrorHandlerComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

namespace nap
{
    
    bool PythonErrorHandlerComponentInstance::init(utility::ErrorState& errorState)
    {
        // Set up python error log.
        mReceivedPythonErrors.reserve(25);
        mPythonScriptComponent->mPythonErrorCaught.connect(mPythonErrorReceived);

        return true;
        
    }


    const std::vector<std::string>& PythonErrorHandlerComponentInstance::getMessages()
    {
        return mReceivedPythonErrors;
    }


    void PythonErrorHandlerComponentInstance::onPythonErrorReceived(const std::string& errorMessage)
    {
        // Add
        mReceivedPythonErrors.emplace_back(errorMessage);

        // Remove first element when out of range
        if (mReceivedPythonErrors.size() > 25)
            mReceivedPythonErrors.erase(mReceivedPythonErrors.begin());
    }

}
