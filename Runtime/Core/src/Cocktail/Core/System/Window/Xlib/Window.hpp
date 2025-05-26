#ifndef COCKTAIl_CORE_SYSTEM_WINDOW_XLIB_WINDOW_HPP
#define COCKTAIl_CORE_SYSTEM_WINDOW_XLIB_WINDOW_HPP

#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>
#include <Cocktail/Core/System/Window/Window.hpp>
#include <Cocktail/Core/System/Window/WindowCreateInfo.hpp>

namespace Ck::Detail::Xlib
{
    class Window final : public Ck::Window
    {
    public:

		/**
		 * \brief 
		 * \param createInfo 
		 * \param windowClass 
		 */
		Window(::Display* display, const WindowCreateInfo& createInfo);
		
		/**
		 * \brief 
		 */
		~Window() override;

		/**
		 * \brief 
		 */
		bool PollEvents() override;

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
		Extent2D<unsigned int> GetSize() const override;
		
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

		struct WMHints
        {
            enum class Flag
            {
                Functions = 1 << 0,
                Decorations = 1 << 1,
            };

            enum class Decor
            {
                All = 1 << 0,
                Border = 1 << 1,
                ResizeH = 1 << 2,
                Title = 1 << 3,
                Menu = 1 << 4,
                Minimize = 1 << 5,
                Maximize = 1 << 6,
            };

            enum class Function
            {
                All = 1 << 0,
                Resize = 1 << 1,
                Move = 1 << 2,
                Minimize = 1 << 3,
                Maximize = 1 << 4,
                Close = 1 << 5,
            };

            Flags<Flag> flags = 0;
            Flags<Function> functions = 0;
            Flags<Decor> decorations = 0;
            long inputMode = 0;
            unsigned long state = 0;
        };

		struct Handle
		{
			::Display* Display;
			::Visual Visual;
			::Colormap Colormap;
			::Window Window;
		};

		Handle mHandle;
		std::shared_ptr<Cursor> mCursor;
		Extent2D<int> mCursorPosition;
		::Cursor mHiddenCursor;
		bool mMapped;
		Extent2D<unsigned int> mLastSize;
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

#endif // COCKTAIl_CORE_SYSTEM_WINDOW_XLIB_WINDOW_HPP
