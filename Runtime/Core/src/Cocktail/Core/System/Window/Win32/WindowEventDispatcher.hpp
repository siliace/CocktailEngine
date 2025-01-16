#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOWEVENTDISPATCHER_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOWEVENTDISPATCHER_HPP

#include <type_traits>

#include <Cocktail/Core/System/Window/Win32/Window.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class WindowEventDispatcher
	{
	public:

		/**
		 * \brief 
		 * \param window 
		 * \param msg 
		 * \param wParam 
		 * \param lParam 
		 */
		void Dispatch(Window* window, UINT msg, WPARAM wParam, LPARAM lParam) const;

	private:

		/**
		 * \brief
		 * \param msg
		 * \return
		 */
		static MouseButtonEvent::ClickType GetMessageClickType(UINT msg);

		/**
		 * \brief 
		 * \param wParam 
		 * \return 
		 */
		static KeyboardKey GetMessageKey(WPARAM wParam);

		/**
		 * \brief
		 * \param msg
		 * \return
		 */
		static MouseButton GetMessageMouseButton(UINT msg);

		/**
		 * \brief 
		 * \tparam T 
		 * \param window 
		 * \return 
		 */
		template <typename T, typename = std::enable_if_t<std::is_base_of_v<WindowEvent, T>>>
		T CreateWindowEvent(Window* window) const
		{
			T event;
			event.Window = window;

			return event;
		}

		/**
		 * \brief 
		 * \tparam T 
		 * \param window
		 * \param wParam
		 * \param lParam 
		 * \return 
		 */
		template <typename T, typename = std::enable_if_t<std::is_base_of_v<MouseEvent, T> && std::is_base_of_v<WindowEvent, T>>>
		T CreateWindowMouseEvent(Window* window, WPARAM wParam, LPARAM lParam) const
		{
			T event = CreateWindowEvent<T>(window);

			if (wParam & MK_CONTROL)
				event.Flags |= MouseEventFlagBits::ControlKey;

			if (wParam & MK_LBUTTON)
				event.Flags |= MouseEventFlagBits::LeftButton;

			if (wParam & MK_MBUTTON)
				event.Flags |= MouseEventFlagBits::MiddleButton;

			if (wParam & MK_RBUTTON)
				event.Flags |= MouseEventFlagBits::RightButton;

			if (wParam & MK_SHIFT)
				event.Flags |= MouseEventFlagBits::ShiftKey;

			if (wParam & MK_XBUTTON1)
				event.Flags |= MouseEventFlagBits::ExtraButton1;

			if (wParam & MK_XBUTTON2)
				event.Flags |= MouseEventFlagBits::ExtraButton2;

			event.Position.Width = GET_X_LPARAM(lParam);
			event.Position.Height = GET_Y_LPARAM(lParam);

			return event;
		}

		mutable Extent2D<int> mMousePosition; /// TODO: this should be per window !
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOWEVENTDISPATCHER_HPP
