#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_WIN32_MONITOR_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_WIN32_MONITOR_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Monitor/Monitor.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class Monitor : public Inherit<Monitor, Object, Ck::Monitor>
	{
	public:

		/**
		 * \brief 
		 * \param handle
		 * \param area
		 * \param deviceName
		 * \param primary 
		 */
		Monitor(HMONITOR handle, LPRECT area, LPCSTR deviceName, bool primary);

		/**
		 * \brief 
		 * \return 
		 */
		bool IsPrimary() const override;

		/**
		 * \brief 
		 * \return 
		 */
		const VideoMode& GetVideoMode() const override;

		/**
		 * \brief 
		 * \return 
		 */
		void* GetSystemHandle() const override;

	private:

		HMONITOR mHandle;
		bool mPrimary;
		Extent2D<unsigned int> mSize;
		VideoMode mVideoMode;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_WIN32_MONITOR_HPP
