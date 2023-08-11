#include "PythonLoggingComponent.h"

// Nap includes
#include <entity.h>

// RTTI
RTTI_DEFINE_CLASS(nap::PythonLoggingComponent)

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::PythonLoggingComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
    RTTI_FUNCTION("addMessage", &nap::PythonLoggingComponentInstance::addMessage)
RTTI_END_CLASS

namespace nap
{
    
    bool PythonLoggingComponentInstance::init(utility::ErrorState& errorState)
    {
        mMessages.reserve(25);
        return true;
    }


    void PythonLoggingComponentInstance::update(double deltaTime)
    {
        // Try to find the python script component (which has intialised before this component).
        if(!mPythonScriptComponentFound)
        {
            PythonScriptComponentInstance* pythonScriptComponent = getEntityInstance()->findComponent<PythonScriptComponentInstance>();
            if(pythonScriptComponent != nullptr)
            {
                // Connect signal to display python errors
                pythonScriptComponent->mPythonErrorCaught.connect(mPythonErrorReceived);
                mPythonScriptComponentFound = true;
            }
        }
    }


    const std::vector<std::string>& PythonLoggingComponentInstance::getMessages()
    {
        return mMessages;
    }


    void PythonLoggingComponentInstance::onMessageReceived(const std::string& message)
    {
        // Add message
        mMessages.emplace_back(message);

        // Remove first element when out of range
        if (mMessages.size() > 25)
            mMessages.erase(mMessages.begin());
    }

}
