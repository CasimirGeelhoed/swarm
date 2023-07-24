#include "swarmapp.h"

// External Includes
#include <utility/fileutils.h>
#include <nap/logger.h>
#include <inputrouter.h>
#include <rendergnomoncomponent.h>
#include <renderablemeshcomponent.h>
#include <perspcameracomponent.h>
#include <oschandler.h>

#include <swarmservice.h>

#include <DataRenderingComponent.h>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::swarmApp)
	RTTI_CONSTRUCTOR(nap::Core&)
RTTI_END_CLASS

namespace nap 
{
	/**
	 * Initialize all the resources and instances used for drawing
	 * slowly migrating all functionality to NAP
	 */
	bool swarmApp::init(utility::ErrorState& error)
	{
		// Retrieve services
		mRenderService	= getCore().getService<nap::RenderService>();
		mSceneService	= getCore().getService<nap::SceneService>();
		mInputService	= getCore().getService<nap::InputService>();
		mGuiService		= getCore().getService<nap::IMGuiService>();

		// Fetch the resource manager
		mResourceManager = getCore().getResourceManager();

		// Get the render window
		mRenderWindow = mResourceManager->findObject<nap::RenderWindow>("Window");
		if (!error.check(mRenderWindow != nullptr, "unable to find render window with name: %s", "Window"))
			return false;

		// Get the scene that contains our entities and components
		mScene = mResourceManager->findObject<Scene>("Scene");
		if (!error.check(mScene != nullptr, "unable to find scene with name: %s", "Scene"))
			return false;

		// Get the camera entity
		mCameraEntity = mScene->findEntity("CameraEntity");
		if (!error.check(mCameraEntity != nullptr, "unable to find entity with name: %s", "CameraEntity"))
			return false;

		// Get the Gnomon entity
		mGnomonEntity = mScene->findEntity("GnomonEntity");
		if (!error.check(mGnomonEntity != nullptr, "unable to find entity with name: %s", "GnomonEntity"))
			return false;
                
        // Get the Rendering entity
        mRenderingEntity = mScene->findEntity("RenderingEntity");
        if (!error.check(mRenderingEntity != nullptr, "unable to find entity with name: %s", "RenderingEntity"))
            return false;
        
        // Get the Receiving entity
        mReceivingEntity = mScene->findEntity("ReceivingEntity");
        if (!error.check(mReceivingEntity != nullptr, "unable to find entity with name: %s", "ReceivingEntity"))
            return false;

        
        // Get the Grid entity
        mGridEntity = mScene->findEntity("GridEntity");
        if (!error.check(mGridEntity != nullptr, "unable to find entity with name: %s", "GridEntity"))
            return false;
        
        mCircleGridEntity = mScene->findEntity("CircleGridEntity");
        if (!error.check(mCircleGridEntity != nullptr, "unable to find entity with name: %s", "CircleGridEntity"))
            return false;

        
        // Get the Shadows entity
        mShadowsEntity = mScene->findEntity("ShadowsEntity");
        if (!error.check(mShadowsEntity != nullptr, "unable to find entity with name: %s", "ShadowsEntity"))
            return false;
        
        
        mOutputData = mResourceManager->findObject("OutputData");
        if (!error.check(mOutputData != nullptr, "unable to find resource with name: %s", "OutputData"))
            return false;
        
        mParameterData = mResourceManager->findObject("ParameterData");
        if (!error.check(mParameterData != nullptr, "unable to find resource with name: %s", "ParameterData"))
            return false;
        
        mOSCSender = mResourceManager->findObject("OSCSender");
        if (!error.check(mOSCSender != nullptr, "unable to find resource with name: %s", "OSCSender"))
            return false;
        
        mConfig = &getCore().getService<nap::swarmService>()->getSwarmServiceConfiguration();
        
        restartOSCSender();
        
		// All done!
		return true;
	}
	
	
	void swarmApp::update(double deltaTime)
	{
		// Use a default input router to forward input events (recursively) to all input components in the default scene
		nap::DefaultInputRouter input_router(true);
		mInputService->processWindowEvents(*mRenderWindow, input_router, { &mScene->getRootEntity() });
        
        updateGUI();
    }


    void swarmApp::writeConfig()
    {
        static std::string configError;
        utility::ErrorState errorState;
        auto configPath = getCore().getProjectInfo()->mServiceConfigFilename;
        if (configPath.empty())
            configPath = "config.json";
        configError.clear();
        if (!getCore().writeConfigFile(configPath, errorState))
        {
            configError = errorState.toString();
            nap::Logger::error(configError);
        }
    }
	

	void swarmApp::render()
	{
		// Signal the beginning of a new frame, allowing it to be recorded.
		// The system might wait until all commands that were previously associated with the new frame have been processed on the GPU.
		// Multiple frames are in flight at the same time, but if the graphics load is heavy the system might wait here to ensure resources are available.
		mRenderService->beginFrame();

        if(mConfig->mDarkMode)
            mRenderWindow->setClearColor({ mDarkColor[0], mDarkColor[1], mDarkColor[2], 1.f });
        else
            mRenderWindow->setClearColor({ mColor[0], mColor[1], mColor[2], 1.f });
        
		// Begin recording the render commands for the main render window
		if (mRenderService->beginRecording(*mRenderWindow))
		{
			// Begin render pass
			mRenderWindow->beginRendering();

			// Get Perspective camera to render with
			auto& perp_cam = mCameraEntity->getComponent<PerspCameraComponentInstance>();

			std::vector<nap::RenderableComponentInstance*> components_to_render
			{
                &mRenderingEntity->getComponent<DataRenderingComponentInstance>()
			};
            
            if(mConfig->mGnomon)
                components_to_render.emplace_back(&mGnomonEntity->getComponent<RenderGnomonComponentInstance>());

            if(mConfig->mCircleGrid)
                components_to_render.emplace_back(&mCircleGridEntity->getComponent<RenderableMeshComponentInstance>());
            else
                components_to_render.emplace_back(&mGridEntity->getComponent<RenderableMeshComponentInstance>());

            if(mConfig->mShadows)
                components_to_render.emplace_back(&mShadowsEntity->getComponent<DataRenderingComponentInstance>());

			mRenderService->renderObjects(*mRenderWindow, perp_cam, components_to_render);

			// Render GUI elements
			mGuiService->draw();

			// Stop render pass
			mRenderWindow->endRendering();

			// End recording
			mRenderService->endRecording();
		}

		// Proceed to next frame
		mRenderService->endFrame();
	}

    void swarmApp::restartOSCSender()
    {
        mOSCSender->mIPAddress = mConfig->mOSCOutputAddress;
        mOSCSender->mPort = mConfig->mOSCOutputPort;
        mOSCSender->stop();
        utility::ErrorState e;
        mOSCSender->start(e);
    }
	

    void swarmApp::updateGUI()
    {
        int windowWidth = mRenderWindow->getWidthPixels();
        int settingsWidth = 500;
        int monitorWidth = 350;
        int paramWidth = 0.66 * (windowWidth - settingsWidth - monitorWidth);
        int dataWidth = windowWidth - settingsWidth - monitorWidth - paramWidth;
        
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(settingsWidth, 0));
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
        ImGui::Begin("Settings");
        {
            ImGui::PushItemWidth(200);
            static char buf[16] = "";
            std::copy(mConfig->mOSCOutputAddress.begin(), mConfig->mOSCOutputAddress.end(), buf);
            if(ImGui::InputText("OSC Address", buf, IM_ARRAYSIZE(buf)))
                mConfig->mOSCOutputAddress = buf;
            ImGui::InputInt("OSC Port", &mConfig->mOSCOutputPort, 0);
            ImGui::PopItemWidth();
            if(ImGui::Button("Apply"))
            {
                writeConfig();
                restartOSCSender();
            }
                
            
            ImGui::Separator();
            ImGui::Text(utility::stringFormat("FPS: %.02f", getCore().getFramerate()).c_str());
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(settingsWidth, 0));
        ImGui::SetNextWindowSize(ImVec2(paramWidth, 0));
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
        ImGui::Begin("Parameters");
        {
            ImGui::Columns(4, "params", false);
            for(auto& x : mParameterData->getVec3Parameters())
            {
                ImGui::Text(x.first.c_str());
                ImGui::NextColumn();
                ImGui::Text("%.02f", x.second->mValue.x);
                ImGui::NextColumn();
                ImGui::Text("%.02f", x.second->mValue.y);
                ImGui::NextColumn();
                ImGui::Text("%.02f", x.second->mValue.z);
                ImGui::NextColumn();
                ImGui::Separator();
            }

            for(auto& x : mParameterData->getFloatParameters())
            {
                ImGui::Text(x.first.c_str());
                ImGui::NextColumn();
                ImGui::Text("%.02f", x.second->mValue);
                ImGui::NextColumn();
                ImGui::NextColumn();
                ImGui::NextColumn();
                ImGui::Separator();
            }
            ImGui::Columns(1);
        }
        ImGui::End();
        
        ImGui::SetNextWindowPos(ImVec2(settingsWidth + paramWidth, 0));
        ImGui::SetNextWindowSize(ImVec2(dataWidth, 0));
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
        ImGui::Begin("Output data");
        {
            ImGui::Columns(3, "outputdata", false);
            int i = 0;
            for(auto& x : mOutputData->getVec3Fields())
            {
                ImGui::Text(x.first.c_str());
                ImGui::NextColumn();
                
                const char* items[] = { "Off", "Position", "Arrow" };
                static int item_current = 0;
                ImGui::Combo(("##combovec3" + std::to_string(i++)).c_str(), &item_current, items, IM_ARRAYSIZE(items));

                ImGui::NextColumn();
                bool dummy = false;
                ImGui::Checkbox(" ", &dummy);
                ImGui::NextColumn();
                ImGui::Separator();
            }
            
            
            for(auto& x : mOutputData->getFloatFields())
            {
                ImGui::Text(x.first.c_str());
                ImGui::NextColumn();
                
                const char* items[] = { "Off", "Red", "Green", "Blue", "Scale"};
                static int item_current = 0;
                ImGui::Combo(("##combofloat" + std::to_string(i++)).c_str(), &item_current, items, IM_ARRAYSIZE(items));
                
                ImGui::NextColumn();
                bool dummy = false;
                ImGui::Checkbox(" ", &dummy);
                ImGui::NextColumn();
                ImGui::Separator();
            }
        }
        ImGui::End();
        
        ImGui::SetNextWindowPos(ImVec2(settingsWidth + paramWidth + dataWidth, 0));
        ImGui::SetNextWindowSize(ImVec2(monitorWidth, 0));
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
        ImGui::Begin("Monitor");
        {
            if(ImGui::Checkbox("Gnomon", &mConfig->mGnomon)) writeConfig();
            if(ImGui::Checkbox("Shadows", &mConfig->mShadows)) writeConfig();
            if(ImGui::Checkbox("Circular grid", &mConfig->mCircleGrid)) writeConfig();
            if(ImGui::Checkbox("Dark mode", &mConfig->mDarkMode)) writeConfig();
        }
        ImGui::End();
        
        ImGui::Begin("OSC Input");
        showOSCLog();
        ImGui::End();

    }


    void swarmApp::showOSCLog()
    {
        // Get the osc handle component
        auto osc_handler = mReceivingEntity->findComponent<OscHandlerComponentInstance>();

        // Get all received osc messages and convert into a single string
        std::string msg;
        for (const auto& message : osc_handler->getMessages())
            msg += (message + "\n");

        // Backup text
        char txt[256] = "No OSC Messages Received";

        // If there are no messages display that instead of the received messages
        char* display_msg = msg.empty() ? txt : &msg[0];
        size_t display_size = msg.empty() ? 256 : msg.size();

        // Display block of text
        ImGui::InputTextMultiline("OSC Messages", display_msg, display_size, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 15), ImGuiInputTextFlags_ReadOnly);
    }


	void swarmApp::windowMessageReceived(WindowEventPtr windowEvent)
	{
		mRenderService->addEvent(std::move(windowEvent));
	}
	
	
	void swarmApp::inputMessageReceived(InputEventPtr inputEvent)
	{
		if (inputEvent->get_type().is_derived_from(RTTI_OF(nap::KeyPressEvent)))
		{
			// If we pressed escape, quit the loop
			nap::KeyPressEvent* press_event = static_cast<nap::KeyPressEvent*>(inputEvent.get());
			if (press_event->mKey == nap::EKeyCode::KEY_ESCAPE)
				quit();

			// f is pressed, toggle full-screen
			if (press_event->mKey == nap::EKeyCode::KEY_f)
				mRenderWindow->toggleFullscreen();
		}
		// Add event, so it can be forwarded on update
		mInputService->addEvent(std::move(inputEvent));
	}

	
	int swarmApp::shutdown()
	{
		return 0;
	}

}
