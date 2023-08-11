#include "PythonLoggingComponent.h"

// Nap includes
#include <entity.h>

// RTTI
RTTI_DEFINE_CLASS(nap::PythonLoggingComponent)

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::PythonLoggingComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
    RTTI_FUNCTION("reconnect", &nap::PythonLoggingComponentInstance::reconnect)
    RTTI_FUNCTION("addMessage", &nap::PythonLoggingComponentInstance::addMessage)
RTTI_END_CLASS

namespace nap
{
    
    bool PythonLoggingComponentInstance::init(utility::ErrorState& errorState)
    {
        mMessages.reserve(25);
        return true;
    }


    void PythonLoggingComponentInstance::reconnect()
    {
        mPythonErrorReceived.disconnect();
        
        PythonScriptComponentInstance* pythonScriptComponent = getEntityInstance()->findComponent<PythonScriptComponentInstance>();
        
        if(pythonScriptComponent != nullptr)
            pythonScriptComponent->mPythonErrorCaught.connect(mPythonErrorReceived);
        else
            nap::Logger::warn("PythonLoggingComponent couldn't find PythonScriptComponent.");
    }


    void PythonLoggingComponentInstance::addMessage(const std::string& message)
    {
        onMessageReceived(message);
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
