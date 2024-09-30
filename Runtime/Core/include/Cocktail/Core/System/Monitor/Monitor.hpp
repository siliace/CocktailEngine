#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_MONITOR_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_MONITOR_HPP

#include <Cocktail/Core/Meta/Implements.hpp>
#include <Cocktail/Core/System/Monitor/VideoMode.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class Monitor : public Implements<Monitor, Interface>
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		virtual bool IsPrimary() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual const VideoMode& GetVideoMode() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual void* GetSystemHandle() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_MONITOR_HPP
