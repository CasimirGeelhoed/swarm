/*
 This file is part of Swarm, a free scriptable OSC controller with 3D visualisation.
 Copyright (C) 2024 Casimir Geelhoed
 
 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or any later version.
 
 This program is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of  MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along with
 this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include <LabelsRenderingComponent.h>
#include <LuaScriptComponent.h>
#include <DataSendingComponent.h>

namespace nap 
{

	bool CoreApp::init(utility::ErrorState& error)
	{
		mConfig = &getCore().getService<nap::swarmService>()->getSwarmServiceConfiguration();
		
		setFramerate(60.0);
		capFramerate(mConfig->mCapFPS);
		
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
		
		// Get the Controlling entity
		mControllingEntity = mScene->findEntity("ControllingEntity");
		if (!error.check(mControllingEntity != nullptr, "unable to find entity with name: %s", "ControllingEntity"))
			return false;
		
		// Get the Sending entity
		mSendingEntity = mScene->findEntity("SendingEntity");
		if (!error.check(mSendingEntity != nullptr, "unable to find entity with name: %s", "SendingEntity"))
			return false;
		
		// Get the Grid entity
		mGridEntity = mScene->findEntity("GridEntity");
		if (!error.check(mGridEntity != nullptr, "unable to find entity with name: %s", "GridEntity"))
			return false;
		
		// Get the Circle Grid entity
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
		
		mOSCReceiver = mResourceManager->findObject("OSCReceiver");
		if (!error.check(mOSCReceiver != nullptr, "unable to find resource with name: %s", "OSCReceiver"))
			return false;
		
		// Calculate pixel multiplier (2.75 is the scale of the computer I am working at).
		mPixelMultiplier = mGuiService->getScale() / 2.75;
		
		// Apply pixel multiplier to labels rendering components.
		auto* indicesRenderingComponent = mRenderingEntity->findComponentByID<LabelsRenderingComponentInstance>("IndicesRenderingComponent");
		if (!error.check(indicesRenderingComponent != nullptr, "unable to find component with name: %s", "IndicesRenderingComponent"))
			return false;
		indicesRenderingComponent->mLabelOffset *= mPixelMultiplier;
		
		auto* dataLabelsRenderingComponent = mRenderingEntity->findComponentByID<LabelsRenderingComponentInstance>("DataLabelsRenderingComponent");
		if (!error.check(dataLabelsRenderingComponent != nullptr, "unable to find component with name: %s", "DataLabelsRenderingComponent"))
			return false;
		dataLabelsRenderingComponent->mLabelOffset *= mPixelMultiplier;
		
		// Restart OSC sender, update OSC rate, select data, etc..
		postResourcesLoaded();
		
		// and make this happen again after each hotload.
		mResourceManager->mPostResourcesLoadedSignal.connect(mPostResourcesLoadedSlot);
		
		
		// All done!
		return true;
	}
	
	
	void CoreApp::postResourcesLoaded()
	{
		restartOSCReceiver();
		restartOSCSender();
		updateOSCRate();
		updateSelectedData();
		
		initScriptSelector();
		loadScript();
	}
	
	
	void CoreApp::update(double deltaTime)
	{
		// Use a default input router to forward input events (recursively) to all input components in the default scene
		nap::DefaultInputRouter input_router(true);
		mInputService->processWindowEvents(*mRenderWindow, input_router, { &mScene->getRootEntity() });
		
		updateGUI();
		
		// update status message
		if(mStatusMessageTimer > 0.f)
			mStatusMessageTimer -= deltaTime;
		else
			mShowStatusMessage = false;
	}
	
	
	void CoreApp::writeConfig()
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
	
	
	void CoreApp::render()
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
			
			// Render labels.
			if(mConfig->mLabels)
			{
				auto* indicesRenderingComponent = mRenderingEntity->findComponentByID<LabelsRenderingComponentInstance>("IndicesRenderingComponent");
				indicesRenderingComponent->draw(*mRenderWindow, perp_cam, mConfig->mDarkMode ? mColor : mDarkColor);
				
				auto* dataLabelsRenderingComponent = mRenderingEntity->findComponentByID<LabelsRenderingComponentInstance>("DataLabelsRenderingComponent");
				dataLabelsRenderingComponent->draw(*mRenderWindow, perp_cam, mConfig->mDarkMode ? mColor : mDarkColor);
			}
			
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
	
	void CoreApp::restartOSCSender()
	{
		mOSCSender->mIPAddress = mConfig->mOSCOutputAddress;
		mOSCSender->mPort = mConfig->mOSCOutputPort;
		mOSCSender->stop();
		utility::ErrorState e;
		mOSCSender->start(e);
		
		// show message
		setStatusMessage("Sending OSC to " + mConfig->mOSCOutputAddress + " / " + std::to_string(mConfig->mOSCOutputPort), 5.0f);
	}
	
	
	void CoreApp::restartOSCReceiver()
	{
		mOSCReceiver->mPort = mConfig->mOSCInputPort;
		mOSCReceiver->stop();
		utility::ErrorState e;
		mOSCReceiver->start(e);
		
		// show message
		setStatusMessage("Receiving OSC on port " + std::to_string(mConfig->mOSCInputPort), 5.0f);
	}

	
	void CoreApp::updateSelectedData()
	{
		for(auto& x : mOutputData->getVec3Fields())
		{
			std::string name = x.first.c_str();
			if(mConfig->mSelectedData == name)
				selectData(name, true);
		}
		for(auto& x : mOutputData->getFloatFields())
		{
			std::string name = x.first.c_str();
			if(mConfig->mSelectedData == name)
				selectData(name, false);
		}
	}
	
	
	void CoreApp::selectData(std::string fieldName, bool isVec3)
	{
		auto* dataLabelsRenderingComponent = mRenderingEntity->findComponentByID<LabelsRenderingComponentInstance>("DataLabelsRenderingComponent");
		dataLabelsRenderingComponent->setDataToRender(fieldName, isVec3);
	}
	
	
	void CoreApp::updateGUI()
	{
		if(mShowStatusMessage)
			showStatusMessage();
		
		showLuaLogMessage();
		
		showVersionNumber();

		int windowWidth = mRenderWindow->getWidthPixels();
		int windowHeight = mRenderWindow->getHeightPixels();
		int settingsWidth = 500 * mPixelMultiplier;
		int monitorWidth = 350 * mPixelMultiplier;
		int paramWidth = 0.66 * (windowWidth - settingsWidth - monitorWidth);
		int dataWidth = windowWidth - settingsWidth - monitorWidth - paramWidth;
				
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(settingsWidth, 0));
		ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
		ImGui::Begin("Settings");
		showSettings();
		ImGui::End();
		
		ImGui::SetNextWindowPos(ImVec2(settingsWidth, 0));
		ImGui::SetNextWindowSize(ImVec2(paramWidth, 0));
		ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
		ImGui::Begin("Input Parameters");
		if(mConfig->mEditParameters)
			showEditableParameters();
		else
			showParameters();
		ImGui::End();
		
		ImGui::SetNextWindowPos(ImVec2(settingsWidth + paramWidth, 0));
		ImGui::SetNextWindowSize(ImVec2(dataWidth, 0));
		ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
		ImGui::Begin("Output Data");
		showOutputData();
		ImGui::End();
		
		ImGui::SetNextWindowPos(ImVec2(settingsWidth + paramWidth + dataWidth, 0));
		ImGui::SetNextWindowSize(ImVec2(monitorWidth, 0));
		ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
		ImGui::Begin("Monitor");
		showMonitorOptions();
		ImGui::End();
		
		int i = 1;
		
		if(mLuaLogVisible)
		{
			ImGui::Begin("Lua Log", &mLuaLogVisible);
			showLuaLog();
			ImGui::End();
		}
		else
		{
			ImGui::SetNextWindowPos(ImVec2(0, windowHeight - 32 * mPixelMultiplier - 100 * mPixelMultiplier * (i++)));
			bool b = true;
			ImGui::Begin("lualogbutton", &b, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );
			if(ImGui::Button("Lua Log"))
				mLuaLogVisible = true;
			ImGui::End();
		}
		
		if(mOSCLogVisible)
		{
			ImGui::Begin("OSC Input Log", &mOSCLogVisible);
			showOSCLog();
			ImGui::End();
		}
		else
		{
			ImGui::SetNextWindowPos(ImVec2(0, windowHeight - 32 * mPixelMultiplier - 100 * mPixelMultiplier * (i++)));
			bool b = true;
			ImGui::Begin("osclogbutton", &b, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );
			if(ImGui::Button("OSC Log"))
				mOSCLogVisible = true;
			ImGui::End();
		}
		
		if(mAboutVisible)
		{
			ImGui::SetNextWindowPos(ImVec2(mRenderWindow->getWidthPixels() * 0.5f, mRenderWindow->getHeightPixels() * 0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
			ImGui::SetNextWindowSize(ImVec2(mPixelMultiplier * 1000, mPixelMultiplier * 300));
			ImGui::Begin("About", &mAboutVisible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::Text(("Swarm " + mVersion).c_str());
			ImGui::Text("Copyright © Casimir Geelhoed 2024");
			ImGui::Text("Published under the GNU General Public License.");
			ImGui::End();
		}
				
	}
	
	
	void CoreApp::setStatusMessage(std::string message, float duration)
	{
		mStatusMessageTimer = duration;
		mStatusMessage = message;
		mShowStatusMessage = true;
	}
	
	
	void CoreApp::updateOSCRate()
	{
		mSendingEntity->getComponent<DataSendingComponentInstance>().setOutputRate(mConfig->mOSCRate);
	}
	
	
	void CoreApp::loadScript()
	{
		auto luaScriptComponent = mControllingEntity->findComponent<LuaScriptComponentInstance>();
		assert(luaScriptComponent);
		
		utility::ErrorState e;
		if(luaScriptComponent->loadScript(mConfig->mScriptPath, e))
			setStatusMessage("Loaded script: " + mConfig->mScriptPath, 5.0f);
		else
			setStatusMessage("Failed to load script: " + mConfig->mScriptPath, 5.0f);
		
	}

	
	void CoreApp::showSettings()
	{
		showScriptSelector();
		
		// The "Refresh" button repopulates the script selector and reloads the current script.
		if(ImGui::Button("Refresh"))
		{
			initScriptSelector();
			loadScript();
		}

		ImGui::Separator();
		
		ImGui::PushItemWidth(200);
		
		ImGui::InputInt("Input Port", &mConfig->mOSCInputPort, 0);
		if(ImGui::Button("Apply##input"))
		{
			restartOSCReceiver();
			writeConfig();
		}
		
		ImGui::Separator();

		static char buf[16] = "";
		std::copy(mConfig->mOSCOutputAddress.begin(), mConfig->mOSCOutputAddress.end(), buf);
		if(ImGui::InputText("Output Address", buf, IM_ARRAYSIZE(buf)))
			mConfig->mOSCOutputAddress = buf;
		ImGui::InputInt("Output Port", &mConfig->mOSCOutputPort, 0);
		ImGui::PopItemWidth();
		if(ImGui::Button("Apply##output"))
		{
			restartOSCSender();
			writeConfig();
		}
		
		ImGui::Separator();
		
		ImGui::PushItemWidth(200);
		if (ImGui::SliderFloat("OSC Rate", &mConfig->mOSCRate, 0, 60.0f))
		{
			updateOSCRate();
			writeConfig();
		}
		ImGui::PopItemWidth();
		
		
		ImGui::Separator();
		
		if(ImGui::Checkbox("Edit Parameters", &mConfig->mEditParameters))
		{
			writeConfig();
		}
		
		ImGui::Separator();
		
		if(ImGui::Checkbox("Limit FPS", &mConfig->mCapFPS))
		{
			capFramerate(mConfig->mCapFPS);
			writeConfig();
		}
		
		ImGui::Separator();
		
		ImGui::Text(utility::stringFormat("FPS: %.02f", getCore().getFramerate()).c_str());
		
		ImGui::Separator();
		
		if(ImGui::Button("About"))
			mAboutVisible = true;

	}
	
	
	static bool vector_getter(void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	
	void CoreApp::initScriptSelector()
	{
		// read all directory files
		mScriptPaths.clear();
		utility::listDir("scripts", mScriptPaths);
				
		// filter out non-lua files
		mScriptPaths.erase(std::remove_if(mScriptPaths.begin(), mScriptPaths.end(), [&](const std::string& s) -> bool { return utility::getFileExtension(s) != "lua"; }), mScriptPaths.end());
		
		// sort alphabetically
		std::sort(mScriptPaths.begin(), mScriptPaths.end());
		
		// set selected script index
		for(int i = 0; i < mScriptPaths.size(); i++)
			if(mScriptPaths[i] == mConfig->mScriptPath)
				mSelectedScriptIndex = i;
		
		// if the config script path was not found, set it to the first script path
		if(mSelectedScriptIndex == 0)
			mConfig->mScriptPath = mScriptPaths[0];
		
		// get file names without extension to display
		mScriptNames.clear();
		for(auto& s : mScriptPaths)
			mScriptNames.emplace_back(utility::getFileName(s));
	}

	
	void CoreApp::showScriptSelector()
	{
		// show combo
		if (ImGui::Combo("Script", &mSelectedScriptIndex, vector_getter,
						 static_cast<void*>(&mScriptNames), mScriptNames.size()))
		{
			mConfig->mScriptPath = mScriptPaths[mSelectedScriptIndex]; // change selected script in config
			writeConfig(); // save changes to config file
			loadScript(); // try to load the newly selected script
		}
	}

	
	void CoreApp::showEditableParameters()
	{
		for(auto& x : mParameterData->getVec3Parameters())
		{
			auto* param = x.second.get();
			glm::vec3 value = param->mValue;
			if (ImGui::SliderFloat3(param->getDisplayName().c_str(), &(value[0]), param->mMinimum, param->mMaximum))
				param->setValue(value);
			ImGui::Separator();
		}
		
		for(auto& x : mParameterData->getFloatParameters())
		{
			auto* param = x.second.get();
			float value = param->mValue;
			if (ImGui::SliderFloat(param->getDisplayName().c_str(), &value, param->mMinimum, param->mMaximum))
				param->setValue(value);
			ImGui::Separator();
		}
	}
	
	
	void CoreApp::showParameters()
	{
		ImGui::Columns(4, "params", false);
		
		for(auto& x : mParameterData->getVec3Parameters())
		{
			ImGui::Text(x.first.c_str());
			ImGui::NextColumn();
			ImGui::Text("%.03f", x.second->mValue.x);
			ImGui::NextColumn();
			ImGui::Text("%.03f", x.second->mValue.y);
			ImGui::NextColumn();
			ImGui::Text("%.03f", x.second->mValue.z);
			ImGui::NextColumn();
			ImGui::Separator();
		}
		
		ImGui::Columns(2, "params", false);
		
		for(auto& x : mParameterData->getFloatParameters())
		{
			ImGui::Text(x.first.c_str());
			ImGui::NextColumn();
			ImGui::Text("%.03f", x.second->mValue);
			ImGui::NextColumn();
			ImGui::Separator();
		}
		
		ImGui::Columns(1);
	}
	
	
	void CoreApp::showOutputData()
	{
		for(auto& x : mOutputData->getVec3Fields())
		{
			std::string name = x.first.c_str();
			if(ImGui::RadioButton(name.c_str(), mConfig->mSelectedData == name))
			{
				selectData(name, true);
				mConfig->mSelectedData = name;
				writeConfig();
			}
			ImGui::Separator();
		}
		
		for(auto& x : mOutputData->getFloatFields())
		{
			std::string name = x.first.c_str();
			if(ImGui::RadioButton(name.c_str(), mConfig->mSelectedData == name))
			{
				selectData(name, false);
				mConfig->mSelectedData = name;
				writeConfig();
			}
			ImGui::Separator();
		}
		
		if(ImGui::RadioButton("(none)", mConfig->mSelectedData == ""))
		{
			selectData("", false);
			mConfig->mSelectedData = "";
			writeConfig();
		}
		
	}
	
	
	void CoreApp::showMonitorOptions()
	{
		if(ImGui::Checkbox("Gnomon", &mConfig->mGnomon))
			writeConfig();
		if(ImGui::Checkbox("Shadows", &mConfig->mShadows))
			writeConfig();
		if(ImGui::Checkbox("Circular grid", &mConfig->mCircleGrid))
			writeConfig();
		if(ImGui::Checkbox("Labels", &mConfig->mLabels))
			writeConfig();
		if(ImGui::Checkbox("Dark mode", &mConfig->mDarkMode))
			writeConfig();
	}
	
	
	void CoreApp::showOSCLog()
	{
		// Get the osc handle component
		auto oscHandler = mReceivingEntity->findComponent<OscHandlerComponentInstance>();
		assert(oscHandler);
		
		// Get all received osc messages and convert into a single string
		std::string msg;
		for (const auto& message : oscHandler->getMessages())
			msg += (message + "\n");
		
		// Backup text
		char txt[256] = "No OSC Messages Received";
		
		// If there are no messages display that instead of the received messages
		char* displayMsg = msg.empty() ? txt : &msg[0];
		size_t displaySize = msg.empty() ? 256 : msg.size();
		
		// Display block of text
		ImGui::InputTextMultiline("OSC Messages", displayMsg, displaySize, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 15), ImGuiInputTextFlags_ReadOnly);
	}
	
	
	void CoreApp::showLuaLog()
	{
		// Get the python error handler component
		auto luaScriptComponent = mControllingEntity->findComponent<LuaScriptComponentInstance>();
		assert(luaScriptComponent);
		
		// Get all received messages and convert into a single string
		std::string msg;
		for (const auto& message : luaScriptComponent->getLogMessages())
			msg += (message + "\n");
		
		// Backup text
		char txt[256] = "No Lua Messages";
		
		// If there are no messages display that instead of the received messages
		char* display_msg = msg.empty() ? txt : &msg[0];
		size_t display_size = msg.empty() ? 256 : msg.size();
		
		// Display block of text
		ImGui::InputTextMultiline("Lua Errors", display_msg, display_size, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 15), ImGuiInputTextFlags_ReadOnly);
	}
	
	
	void CoreApp::showStatusMessage()
	{
		int windowWidth = mRenderWindow->getWidthPixels();
		int windowHeight = mRenderWindow->getHeightPixels();
		
		auto textWidth = ImGui::CalcTextSize(mStatusMessage.c_str()).x;
		ImGui::SetNextWindowPos(ImVec2(0.5f * mRenderWindow->getWidthPixels(), windowHeight - 75 * mPixelMultiplier), ImGuiCond_Always, ImVec2(0.5f,0.0f));
		ImGui::SetNextWindowSize(ImVec2(textWidth + 50.f * mPixelMultiplier, 75)); // Note: + 50.f is needed for padding which is not taken into account.
		bool b = true;
		ImGui::Begin("status", &b, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::Text(mStatusMessage.c_str());
		ImGui::End();
	}
	
	
	void CoreApp::showLuaLogMessage()
	{
		auto luaScriptComponent = mControllingEntity->findComponent<LuaScriptComponentInstance>();
		assert(luaScriptComponent);
		
		const std::vector<std::string>& messages = luaScriptComponent->getLogMessages();
		if(messages.empty())
			return;
		
		const std::string& message = messages.back();
		
		auto textHeight = ImGui::CalcTextSize(message.c_str()).y;
		ImGui::SetNextWindowPos(ImVec2(0, 60 * mPixelMultiplier));
		ImGui::SetNextWindowSize(ImVec2(mRenderWindow->getWidthPixels(), 250 * mPixelMultiplier));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
		bool b = true;
		ImGui::Begin("luaMessage", &b, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::TextWrapped(message.c_str());
		ImGui::End();
		ImGui::PopStyleColor();

	}
	
	
	void CoreApp::showVersionNumber()
	{
		int windowWidth = mRenderWindow->getWidthPixels();
		int windowHeight = mRenderWindow->getHeightPixels();
		
		std::string text = "Swarm " + mVersion;
		ImGui::SetNextWindowPos(ImVec2(windowWidth - (ImGui::CalcTextSize(text.c_str()).x + 37 * mPixelMultiplier), windowHeight - 75 * mPixelMultiplier));
		// Note: '+ 37' is needed for padding which is not taken into account (or because ImGui::CalcTextSize doesn't return the right value).
		bool b = true;
		ImGui::Begin("versionnr", &b, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::Text(text.c_str());
		ImGui::End();
	}
	
	
	void CoreApp::windowMessageReceived(WindowEventPtr windowEvent)
	{
		mRenderService->addEvent(std::move(windowEvent));
	}
	
	
	void CoreApp::inputMessageReceived(InputEventPtr inputEvent)
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
	
	
	int CoreApp::shutdown()
	{
		return 0;
	}
	
}
