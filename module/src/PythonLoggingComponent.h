#pragma once

// Nap includes
#include <component.h>
#include <pythonscriptcomponent.h>

namespace nap
{
    
    class PythonLoggingComponentInstance;
    
    /**
     * Catches Python errors / receives Python log messages and exposes them to the GUI.
     * NOTE: for this to work, PythonScriptComponent should depend on PythonLoggingComponent.
     */
    class NAPAPI PythonLoggingComponent : public Component
    {
        RTTI_ENABLE(Component)
        DECLARE_COMPONENT(PythonLoggingComponent, PythonLoggingComponentInstance)
        
    public:
        PythonLoggingComponent() : Component() { }
                
    private:
    };

    
    class NAPAPI PythonLoggingComponentInstance : public ComponentInstance
    {
        RTTI_ENABLE(ComponentInstance)
    public:
        PythonLoggingComponentInstance(EntityInstance& entity, Component& resource) : ComponentInstance(entity, resource) { }
        
        bool init(utility::ErrorState& errorState) override;
        
        /**
         * Tells the component to reconnect to the current PythonScriptComponent to listen for errors. Called in the Python script's initialisation (so it always reconnects after a hot reload of the python script).
         */
        void reconnect();

        /**
         * Manually adds a message. Called by the Python script.
         */
        void addMessage(const std::string& message);
        
        /**
         * Returns the last 25 messsages.
         */
        const std::vector<std::string>& getMessages();
        
    private:
        Slot<const std::string&> mPythonErrorReceived = { this, &PythonLoggingComponentInstance::onMessageReceived };
        
        void onMessageReceived(const std::string& message);
                        
        std::vector<std::string> mMessages; ///< Holds all the received messages.

    };
        
}
