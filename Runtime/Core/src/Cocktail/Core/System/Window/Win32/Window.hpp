#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOW_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WIN32_WINDOW_HPP

#include <Cocktail/Core/System/Window/Window.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowClass.hpp>
#include <Cocktail/Core/System/Window/WindowCreateInfo.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class Window final : public Ck::Window
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
		 * \param visible
		 */
		void SetVisible(bool visible) override;

		/**
		 * \brief 
		 * \return 
		 */
		bool HasFocus() override;

		/**
		 * \brief 
		 */
		void Minimize() override;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsMinimized() const override;

		/**
		 * \brief 
		 */
		void Maximize() override;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsMaximized() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<Cursor> GetCursor() const override;

		/**
		 * \brief 
		 * \param cursor 
		 */
		void SetCursor(std::shared_ptr<Cursor> cursor) override;

		bool IsCursorVisible() const override;

        /**
         * \brief Change cursor visibility in the area of the window
         * \param visible 
         */
		void SetCursorVisible(bool visible) override;

		/**
		 * \brief Enable or disable the capture of the cursor inside the window
		 * \param capture The state of the capture
		 */
		void EnableCaptureCursor(bool capture) override;

		/**
		 * \brief 
		 * \param icon 
		 */
		void SetIcon(const Image* icon) override;

		/**
		 * \brief 
		 * \return 
		 */
		std::string GetTitle() const override;

		/**
		 * \brief 
		 * \param title 
		 */
		void SetTitle(const std::string& title) override;

		/**
		 * \brief 
		 * \return 
		 */
		Extent2D<unsigned> GetSize() const override;
		
		/**
		 * \brief 
		 * \param size 
		 */
		void SetSize(const Extent2D<unsigned>& size) override;

		/**
		 * \brief 
		 * \return 
		 */
		Extent2D<int> GetPosition() const override;

		/**
		 * \brief 
		 * \param position 
		 */
		void SetPosition(const Extent2D<int>& position) override;

		/**
		 * \brief 
		 * \return 
		 */
		Extent2D<unsigned int> GetMinSize() const override;

		/**
		 * \brief 
		 * \param size 
		 */
		void SetMinSize(const Extent2D<unsigned int>& size) override;

		/**
		 * \brief 
		 * \return 
		 */
		Extent2D<unsigned int> GetMaxSize() const override;

		/**
		 * \brief 
		 * \param size 
		 */
		void SetMaxSize(const Extent2D<unsigned int>& size) override;

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
		std::shared_ptr<Cursor> mCursor;
		Optional<Extent2D<unsigned int>> mMinSize;
		Optional<Extent2D<unsigned int>> mMaxSize;
		HICON mIcon;
		bool mCursorVisible;
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
