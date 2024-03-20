#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOW_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOW_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Window/Window.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowClass.hpp>
#include <Cocktail/Core/System/Window/WindowCreateInfo.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class Window final : public Inherit<Window, Object, Ck::Window>
	{
	public:

		/**
		 * \brief 
		 * \param createInfo 
		 * \param windowClass 
		 */
		Window(const WindowCreateInfo& createInfo, const WindowClass& windowClass);
		
		/**
		 * \brief 
		 * \return 
		 */
		bool PollEvents() override;

		/**
		 * \brief 
		 */
		void Update() override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Cursor> GetCursor() const override;

		/**
		 * \brief 
		 * \param cursor 
		 */
		void SetCursor(Ref<Cursor> cursor) override;

		/**
		 * \brief 
		 * \return 
		 */
		std::string GetTitle() override;

		/**
		 * \brief 
		 * \param title 
		 */
		void SetTitle(const std::string& title) override;

		/**
		 * \brief 
		 * \return 
		 */
		Extent2D<unsigned> GetSize() override;
		
		/**
		 * \brief 
		 * \param size 
		 */
		void SetSize(const Extent2D<unsigned>& size) override;

		/**
		 * \brief 
		 * \return 
		 */
		Extent2D<int> GetPosition() override;

		/**
		 * \brief 
		 * \param position 
		 */
		void SetPosition(const Extent2D<int>& position) override;

		Signal<WindowCloseEvent>& OnCloseEvent() override;
		Signal<WindowRedrawEvent>& OnRedrawEvent() override;
		Signal<WindowFocusEvent>& OnFocusEvent() override;
		Signal<WindowMovedEvent>& OnMovedEvent() override;
		Signal<WindowResizedEvent>& OnResizedEvent() override;
		Signal<WindowTextEvent>& OnTextEvent() override;
		Signal<WindowKeyboardEvent>& OnKeyboardEvent() override;
		Signal<WindowMouseMoveEvent>& OnMouseMovedEvent() override;
		Signal<WindowMouseButtonEvent>& OnMouseButtonEvent() override;
		Signal<WindowMouseWheelEvent>& OnMouseWheelEvent() override;

		void* GetSystemHandle() const override;

	private:

		HWND mHandle;
		Ref<Cursor> mCursor;
		Signal<WindowCloseEvent> mOnClose;
		Signal<WindowRedrawEvent> mOnRedraw;
		Signal<WindowFocusEvent> mOnFocus;
		Signal<WindowMovedEvent> mOnMoved;
		Signal<WindowResizedEvent> mOnResized;
		Signal<WindowTextEvent> mOnTextEvent;
		Signal<WindowKeyboardEvent> mOnKeyboard;
		Signal<WindowMouseMoveEvent> mOnMouseMoved;
		Signal<WindowMouseButtonEvent> mOnMouseButton;
		Signal<WindowMouseWheelEvent> mOnMouseWheel;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOW_HPP
