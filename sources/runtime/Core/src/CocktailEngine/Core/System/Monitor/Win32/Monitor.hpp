#ifndef COCKTAILENGINE_CORE_SYSTEM_MONITOR_WIN32_MONITOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_MONITOR_WIN32_MONITOR_HPP

#include <CocktailEngine/Core/System/Monitor/Monitor.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class Monitor : public Ck::Monitor
	{
	public:

		/**
		 * \brief 
		 * \param handle
		 */
		explicit Monitor(HMONITOR handle);

		/**
		 * \brief 
		 * \return 
		 */
	    bool IsPrimary() const override;

	    /**
         * \brief
         * \return
         */
	    Extent2D<int> GetPosition() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Extent2D<unsigned int> GetSize() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Orientation GetOrientation() const override;

		/**
		 * \brief 
		 * \return 
		 */
		String GetName() const override;

		/**
		 * \brief 
		 * \return 
		 */
		VideoMode GetCurrentVideoMode() const override;

		/**
		 * \brief 
		 * \param videoMode 
		 */
		void SetCurrentVideoMode(const VideoMode& videoMode) override;

		/**
		 * \brief 
		 * \return 
		 */
		Array<VideoMode> GetSupportedVideoModes() const override;

		/**
		 * \brief 
		 * \return 
		 */
		void* GetSystemHandle() const override;

	private:
		
		HMONITOR mHandle;
		MONITORINFOEX mMonitorInfo;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_MONITOR_WIN32_MONITOR_HPP
