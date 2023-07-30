#pragma once

// Core includes
#include <nap/resourcemanager.h>
#include <nap/resourceptr.h>

// Module includes
#include <renderservice.h>
#include <imguiservice.h>
#include <sceneservice.h>
#include <inputservice.h>
#include <scene.h>
#include <renderwindow.h>
#include <entity.h>
#include <app.h>
#include <oscsender.h>

// data classes include
#include <OutputData.h>
#include <ParameterData.h>

namespace nap
{
	using namespace rtti;

    class SwarmServiceConfiguration;

	/**
	 * Main application that is called from within the main loop
	 */
	class swarmApp : public App
	{
		RTTI_ENABLE(App)
	public:
		/**
		 * Constructor
		 * @param core instance of the NAP core system
		 */
		swarmApp(nap::Core& core) : App(core) { }
		
		/**
		 * Initialize all the services and app specific data structures
		 * @param error contains the error code when initialization fails
		 * @return if initialization succeeded
		*/
		bool init(utility::ErrorState& error) override;
		
		/**
		 * Update is called every frame, before render.
		 * @param deltaTime the time in seconds between calls
		 */
		void update(double deltaTime) override;

		/**
		 * Render is called after update. Use this call to render objects to a specific target
		 */
		void render() override;

		/**
		 * Called when the app receives a window message.
		 * @param windowEvent the window message that occurred
		 */
		void windowMessageReceived(WindowEventPtr windowEvent) override;
		
		/**
		 * Called when the app receives an input message (from a mouse, keyboard etc.)
		 * @param inputEvent the input event that occurred
		 */
		void inputMessageReceived(InputEventPtr inputEvent) override;
		
		/**
		 * Called when the app is shutting down after quit() has been invoked
		 * @return the application exit code, this is returned when the main loop is exited
		 */
		virtual int shutdown() override;
        
	private:
        
        void updateGUI();
        
        // Updates to config file.
        void writeConfig();
        
        // Restarts the OSCSender with current configuration.
        void restartOSCSender();
        
        // Selects data for visualisation
        void selectData(std::string fieldName, bool isVec3);
        
        // GUI's
        void showPythonLog();
        void showOSCLog();
        void showSettings();
        void showParameters();
        void showEditableParameters();
        void showOutputData();
        void showMonitorOptions();
        void showStatusMessage();
        void showVersionNumber();

        void setStatusMessage(std::string message, float duration);
        
        bool mPythonLogVisible = false;
        bool mOSCLogVisible = false;
        
        std::string mStatusMessage = "";
        bool mShowStatusMessage = false;
        float mStatusMessageTimer = 0.0f;
        
        SwarmServiceConfiguration* mConfig;
        
		ResourceManager*			mResourceManager = nullptr;		///< Manages all the loaded data
		RenderService*				mRenderService = nullptr;		///< Render Service that handles render calls
		SceneService*				mSceneService = nullptr;		///< Manages all the objects in the scene
		InputService*				mInputService = nullptr;		///< Input service for processing input
		IMGuiService*				mGuiService = nullptr;			///< Manages GUI related update / draw calls
		ObjectPtr<RenderWindow>		mRenderWindow;					///< Pointer to the render window	
		ObjectPtr<Scene>			mScene = nullptr;				///< Pointer to the main scene
        
        ObjectPtr<EntityInstance>	mCameraEntity = nullptr;
		ObjectPtr<EntityInstance>	mGnomonEntity = nullptr;
        ObjectPtr<EntityInstance>   mRenderingEntity = nullptr;
        ObjectPtr<EntityInstance>   mControllingEntity = nullptr;
        ObjectPtr<EntityInstance>   mReceivingEntity = nullptr;
        ObjectPtr<EntityInstance>   mShadowsEntity = nullptr;
        ObjectPtr<EntityInstance>   mGridEntity = nullptr;
        ObjectPtr<EntityInstance>   mCircleGridEntity = nullptr;
        
        ResourcePtr<OSCSender> mOSCSender = nullptr;
        
        ResourcePtr<OutputData> mOutputData = nullptr;
        ResourcePtr<ParameterData> mParameterData = nullptr;
        
        glm::vec3 mColor = { 0.8f, 0.8f, 0.8f };
        glm::vec3 mDarkColor = { 0.1f, 0.1f, 0.1f };
        
    };
}
