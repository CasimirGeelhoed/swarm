#include "swarmapp.h"

// External Includes
#include <utility/fileutils.h>
#include <nap/logger.h>
#include <inputrouter.h>
#include <rendergnomoncomponent.h>
#include <renderablemeshcomponent.h>
#include <perspcameracomponent.h>

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


        
		// All done!
		return true;
	}
	
	
	// Update app
	void swarmApp::update(double deltaTime)
	{
		// Use a default input router to forward input events (recursively) to all input components in the default scene
		nap::DefaultInputRouter input_router(true);
		mInputService->processWindowEvents(*mRenderWindow, input_router, { &mScene->getRootEntity() });
        
        
        // performance gui
        ImGui::Begin("Performance");
        ImGui::Text(utility::stringFormat("FPS: %.02f", getCore().getFramerate()).c_str());
        ImGui::End();
        
        ImGui::Begin("Visualisation options");
        ImGui::Checkbox("Circular grid", &mCircleGrid);
        ImGui::Checkbox("Dark mode", &mDarkMode);
        ImGui::End();

    }
	
	
	// Render app
	void swarmApp::render()
	{
		// Signal the beginning of a new frame, allowing it to be recorded.
		// The system might wait until all commands that were previously associated with the new frame have been processed on the GPU.
		// Multiple frames are in flight at the same time, but if the graphics load is heavy the system might wait here to ensure resources are available.
		mRenderService->beginFrame();

        if(mDarkMode)
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

			// Add Gnomon
			std::vector<nap::RenderableComponentInstance*> components_to_render
			{
				&mGnomonEntity->getComponent<RenderGnomonComponentInstance>(),
                mCircleGrid ? &mCircleGridEntity->getComponent<RenderableMeshComponentInstance>() : &mGridEntity->getComponent<RenderableMeshComponentInstance>(),
                &mRenderingEntity->getComponent<DataRenderingComponentInstance>(),
                &mShadowsEntity->getComponent<DataRenderingComponentInstance>()
			};

			// Render Gnomon
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
