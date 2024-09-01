#pragma once

// External Includes
#include <nap/service.h>

namespace nap
{
	
	class NAPAPI SwarmServiceConfiguration : public ServiceConfiguration
	{
		RTTI_ENABLE(ServiceConfiguration)
		
	public:
		rtti::TypeInfo getServiceType() const;
		
		std::string mScriptPath = "scripts/script.lua"; ///< Property: 'ScriptPath' The path of the script.
		
		int mOSCInputPort = 333; ///< Property: 'OSCOutputAddress'  The OSC output port.
		std::string mOSCOutputAddress = "127.0.0.1"; ///< Property: 'OSCOutputAddress'  The OSC output address.
		int mOSCOutputPort = 7000; ///< Property: 'OSCOutputAddress'  The OSC output port.
		float mOSCRate = 30.f; ///< Property: 'OSCRate'  The OSC output rate.
		
		bool mGnomon = true; ///< Property: 'Gnomon'  Whether the gnomon is beying displayed.
		bool mShadows = true; ///< Property: 'Shadows'  Whether the shadows are beying displayed.
		bool mCircleGrid = true; ///< Property: 'CircleGrid'  Whether the displayed grid is circular or rectangular.
		bool mDarkMode = true; ///< Property: 'DarkMode'  Whether darkmode is enabled.
		bool mCapFPS = true; ///< Property: 'CapFPS'  Whether the display frame rate is being capped to 60FPS.
		bool mEditParameters = true; ///< Property: 'EditParameters'  Whether input parameters are editable in the GUI.
		bool mLabels = true; ///< Property: 'Labels'  Whether source labels are being displayed.

		std::string mSelectedData = ""; ///< Property: 'SelectedData' The currently selected output data to display in the source labels.
	};
	
	
	class NAPAPI swarmService : public Service
	{
		RTTI_ENABLE(Service)
	public:
		// Default Constructor
		swarmService(ServiceConfiguration* configuration) : Service(configuration)	{ }
		
		/**
		 * Use this call to register service dependencies
		 * A service that depends on another service is initialized after all it's associated dependencies
		 * This will ensure correct order of initialization, update calls and shutdown of all services
		 * @param dependencies rtti information of the services this service depends on
		 */
		virtual void getDependentServices(std::vector<rtti::TypeInfo>& dependencies) override;
		
		/**
		 * Initializes the service
		 * @param errorState contains the error message on failure
		 * @return if the service was initialized correctly
		 */
		virtual bool init(nap::utility::ErrorState& errorState) override;
		
		/**
		 * Invoked by core in the app loop. Update order depends on service dependency
		 * This call is invoked after the resource manager has loaded any file changes but before
		 * the app update call. If service B depends on A, A:s:update() is called before B::update()
		 * @param deltaTime: the time in seconds between calls
		 */
		virtual void update(double deltaTime) override;
		
		/**
		 * Invoked when exiting the main loop, after app shutdown is called
		 * Use this function to close service specific handles, drivers or devices
		 * When service B depends on A, Service B is shutdown before A
		 */
		virtual void shutdown() override;
		
		
		SwarmServiceConfiguration& getSwarmServiceConfiguration() { return *getConfiguration<SwarmServiceConfiguration>(); }
		
	};
}
