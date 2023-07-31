#pragma once

// Nap includes
#include <component.h>
#include <pythonscriptcomponent.h>

namespace nap
{
    
    class PythonErrorHandlerComponentInstance;
    
    // TODO: rename to PythonLoggingComponent?
    class NAPAPI PythonErrorHandlerComponent : public Component
    {
        RTTI_ENABLE(Component)
        DECLARE_COMPONENT(PythonErrorHandlerComponent, PythonErrorHandlerComponentInstance)
        
    public:
        PythonErrorHandlerComponent() : Component() { }
        
        ComponentPtr<PythonScriptComponent> mPythonScriptComponent; ///< The PythonScriptComponent.
        
    private:
    };

    
    class NAPAPI PythonErrorHandlerComponentInstance : public ComponentInstance
    {
        RTTI_ENABLE(ComponentInstance)
    public:
        PythonErrorHandlerComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource) { }
        
        // Initialize the component
        bool init(utility::ErrorState& errorState) override;
        
        // Manually add a message (called from Python)
        void addMessage(const std::string& message) { onPythonErrorReceived(message); }
        
        const std::vector<std::string>& getMessages();
        
    private:
        Slot<const std::string&> mPythonErrorReceived = { this, &PythonErrorHandlerComponentInstance::onPythonErrorReceived };
        
        void onPythonErrorReceived(const std::string& errorMessage);
        
        ComponentInstancePtr<PythonScriptComponent> mPythonScriptComponent = { this, &PythonErrorHandlerComponent::mPythonScriptComponent };
        
        std::vector<std::string> mMessages;        ///< Holds all the received events

    };
        
}
