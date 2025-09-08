#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_MONITOR_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_MONITOR_HPP

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/System/Monitor/VideoMode.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class Monitor
	{
	public:

		/**
		 * \brief Enumerator of possible orientations of a Monitor
		 */
		enum class Orientation
		{
			/**
			 * \brief The monitor is horizontal
			 */
			Landscape,

			/**
			 * \brief The monitor is horizontal but reversed
			 */
			UpsideDownLandscape,

			/**
			 * \brief The monitor is vertical on the right
			 */
			RightPortait,

			/**
			 * \brief The monitor is vertical on the left
			 */
			LeftPortait
		};

		/**
		 * \brief 
		 */
		virtual ~Monitor() = default;

		/**
		 * \brief 
		 * \return 
		 */
		virtual bool IsPrimary() const = 0;

		/**
		 * \brief Get the physical size (in pixel) of the monitor
		 * \return The size of the monitor
		 */
		virtual Extent2D<unsigned int> GetSize() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Orientation GetOrientation() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual String GetName() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual VideoMode GetCurrentVideoMode() const = 0;

		/**
		 * \brief
		 * \param videoMode
		 */
		virtual void SetCurrentVideoMode(const VideoMode& videoMode) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Array<VideoMode> GetSupportedVideoModes() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual void* GetSystemHandle() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_MONITOR_HPP
