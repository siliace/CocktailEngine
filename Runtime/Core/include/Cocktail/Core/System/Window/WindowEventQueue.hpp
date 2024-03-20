#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWEVENTQUEUE_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWEVENTQUEUE_HPP

#include <Cocktail/Core/Meta/Interface.hpp>

namespace Ck
{
	class Window;

	/**
	 * \brief 
	 */
	class WindowEventQueue : public Implements<WindowEventQueue, Interface>
	{
	public:
		
		/**
		 * \brief Poll pending events of every Window attached to this queue
		 */
		virtual void PollEvents() const = 0;

		/**
		 * \brief 
		 * \param attachTo 
		 */
		virtual void AttachTo(const Ref<WindowEventQueue>& attachTo) = 0;

		/**
		 * \brief 
		 */
		virtual void Detach() = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWEVENTQUEUE_HPP
