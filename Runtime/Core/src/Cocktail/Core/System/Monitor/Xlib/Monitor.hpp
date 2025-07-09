#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_XLIB_MONITOR_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_XLIB_MONITOR_HPP

#include <Cocktail/Core/System/Monitor/Monitor.hpp>
#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>

namespace Ck::Detail::Xlib
{
	/**
	 * \brief 
	 */
	class Monitor : public Ck::Monitor
	{
	public:

		/**
		 * \brief 
		 * \param display
		 * \param crtc
		 * \param output
		 */
		Monitor(::Display* display, RRCrtc crtc, RROutput output);

		/**
		 * \brief
		 */
		~Monitor() override;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsPrimary() const override;

		/**
		 * \brief Get the physical size (in pixel) of the monitor
		 * \return The size of the monitor
		 */
		Extent2D<unsigned int> GetSize() const override;

		Orientation GetOrientation() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::string GetName() const override;

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

		::Display* mDisplay;
		XRRScreenResources* mScreenResources;
		RRCrtc mCrtc;
		XRRCrtcInfo* mCrtcInfo;
		RROutput mOutput;
		XRROutputInfo* mOutputInfo;
		Extent2D<unsigned int> mSize;
		Orientation mOrientation;
		std::string mName;
		VideoMode mCurrentVideoMode;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_XLIB_MONITOR_HPP
