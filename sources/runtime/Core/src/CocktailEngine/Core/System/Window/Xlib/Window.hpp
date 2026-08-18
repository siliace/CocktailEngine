#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_WINDOW_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_WINDOW_HPP

#include <CocktailEngine/Core/System/Unix/Xlib/Xlib.hpp>
#include <CocktailEngine/Core/System/Window/Window.hpp>
#include <CocktailEngine/Core/System/Window/WindowCreateInfo.hpp>

namespace Ck::Detail::Xlib
{
    /**
     * \brief Xlib implementation of a Window, backed by an X11 window on a shared display
     *
     * Translates the X11 event stream inside PollEvents rather than through a separate
     * dispatcher, X11 having no per-window callback to hook into. The display connection
     * is shared by every window of the process and owned by the service provider, so this
     * class only releases what it created itself: the window, its colormap, its input
     * context and its hidden cursor.
     *
     * X11 keeps the event queue per connection and not per window, which is why PollEvents
     * drains the whole display queue. What the window manager actually grants is also
     * negotiated rather than set, so several requests here are hints the window manager is
     * free to ignore, and the state-related ones are not implemented at all.
     */
    class Window final : public Ck::Window
    {
    public:

        /**
         * \brief Everything a consumer needs to draw into this window through X11
         *
         * Returned by GetSystemHandle so that a graphic backend can create a surface
         * without going through Xlib itself. Every field belongs to the window or to the
         * shared connection, so none of them is to be released by the consumer.
         */
        struct Handle
        {
            /// Connection shared by every window of the process, owned by the service provider.
            ::Display* Display;

            /// Visual the window was created with, describing how pixels are laid out.
            ::Visual* Visual;

            /// Colormap created for this window.
            ::Colormap Colormap;

            /// The X11 window itself, reset to zero once the window has been destroyed.
            ::Window Window;

            /// Input method opened for this window, which turns keystrokes into text.
            ::XIM InputMethod;

            /// Input context of this window, used to compose the text events.
            ::XIC InputContext;
        };

        /**
         * \brief Creates the X11 window and negotiates its decorations with the window manager
         *
         * Requests the decorations matching the style through the Motif hints, which is
         * what most window managers still read, and subscribes to the WM_DELETE_WINDOW
         * protocol so that a close request reaches PollEvents. The window is mapped before
         * returning, and its initial monitor association is resolved through
         * MonitorService.
         *
         * \param display The shared display connection, which has to outlive this window
         * \param createInfo The structure describing the Window to create
         */
        Window(::Display* display, const WindowCreateInfo& createInfo);

        /**
         * \brief Destroys the X11 window and everything created along with it
         *
         * Leaves the shared display connection alone, only flushing it so that the
         * destruction actually reaches the server.
         */
        ~Window() override;

        /**
         * \brief Processes the events the system queued for this window
         *
         * Drains the event queue of the shared display connection, so events belonging to
         * the other windows of the process go through here too.
         *
         * \return True if the window is still open after the processed events, false once it is gone
         */
        bool PollEvents() override;

        bool HasFocus() override;

        /**
         * \brief Minimizes the window to the taskbar or dock
         *
         * Not implemented on this backend.
         */
        void Minimize() override;

        /**
         * \brief Indicates whether the window is currently minimized
         *
         * \return False, whatever the actual state of the window
         */
        bool IsMinimized() const override;

        /**
         * \brief Maximizes the window to the work area of its monitor
         *
         * Not implemented on this backend.
         */
        void Maximize() override;

        /**
         * \brief Indicates whether the window is currently maximized
         *
         * \return False, whatever the actual state of the window
         */
        bool IsMaximized() const override;

        /**
         * \brief Shows or hides the window
         *
         * Maps or unmaps the window and pumps the pending events once, without waiting for
         * the server to confirm the change: the window is not necessarily in the requested
         * state yet when this call returns. Pumping also means the callbacks connected to
         * this window can run from inside this call.
         *
         * \param visible True to show the window, false to hide it
         */
        void SetVisible(bool visible) override;

        Cursor* GetCursor() const override;

        void SetCursor(Cursor* cursor) override;

        bool IsCursorVisible() const override;

        /**
         * \brief Change cursor visibility in the area of the window
         *
         * Hides the cursor by attaching a fully transparent one to the window, and shows it
         * back by reattaching the current cursor. A window with no cursor set keeps the
         * transparent one, so the pointer stays invisible over it.
         *
         * \param visible True to show the cursor over the client area, false to hide it
         */
        void SetCursorVisible(bool visible) override;

        /**
         * \brief Enable or disable the capture of the cursor inside the window
         *
         * Grabs the pointer for this window, which also redirects to it every pointer
         * event the other windows would have received.
         *
         * \param capture The state of the capture
         */
        void EnableCaptureCursor(bool capture) override;

        /**
         * \brief Sets the icon of the window, displayed in its title bar and in the task switcher
         *
         * Not implemented on this backend.
         *
         * \param icon Ignored
         */
        void SetIcon(const Image* icon) override;

        String GetTitle() const override;

        void SetTitle(const String& title) override;

        Extent2D<unsigned int> GetSize() const override;

        void SetSize(const Extent2D<unsigned>& size) override;

        Extent2D<int> GetPosition() const override;

        void SetPosition(const Extent2D<int>& position) override;

        Extent2D<unsigned int> GetMinSize() const override;

        /**
         * \brief Constrains how small the user can make the window
         *
         * Published as a size hint, which the window manager is free to ignore.
         *
         * \param size The minimum client area size to enforce
         */
        void SetMinSize(const Extent2D<unsigned int>& size) override;

        Extent2D<unsigned int> GetMaxSize() const override;

        /**
         * \brief Constrains how large the user can make the window
         *
         * Published as a size hint, which the window manager is free to ignore.
         *
         * \param size The maximum client area size to enforce
         */
        void SetMaxSize(const Extent2D<unsigned int>& size) override;

        Signal<WindowCloseEvent>& OnCloseEvent() override;
        Signal<WindowFocusEvent>& OnFocusEvent() override;
        Signal<WindowMovedEvent>& OnMovedEvent() override;
        Signal<WindowResizingStartEvent>& OnResizingStartEvent() override;
        Signal<WindowResizingEvent>& OnResizingEvent() override;
        Signal<WindowResizingEndEvent>& OnResizingEndEvent() override;
        Signal<WindowResizedEvent>& OnResizedEvent() override;
        Signal<WindowTextEvent>& OnTextEvent() override;
        Signal<WindowKeyboardEvent>& OnKeyboardEvent() override;
        Signal<WindowMouseMoveEvent>& OnMouseMovedEvent() override;
        Signal<WindowMouseButtonEvent>& OnMouseButtonEvent() override;
        Signal<WindowMouseWheelEvent>& OnMouseWheelEvent() override;
        Signal<WindowMonitorChangedEvent>& OnMonitorChangedEvent() override;

        /**
         * \brief Retrieves the underlying system-specific handle
         *
         * \return Non-owning pointer to the Handle of this window, valid as long as the window is
         */
        void* GetSystemHandle() const override;

    private:

        /**
         * \brief Motif window manager hints, the property describing what decorations a window wants
         *
         * Predates the current standards but is still the most widely understood way to
         * ask a window manager for a specific set of decorations. The layout of the five
         * fields is part of the protocol and must not be reordered.
         */
        struct WMHints
        {
            /**
             * \brief Tells which of the fields below carry a meaningful value
             */
            enum class Flag
            {
                /// The functions field is to be read.
                Functions = 1 << 0,

                /// The decorations field is to be read.
                Decorations = 1 << 1,
            };

            /**
             * \brief The decorations a window asks the window manager to draw around it
             */
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

            /**
             * \brief The actions a window lets the user perform on it
             */
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

        WindowDisplayMode GetCurrentDisplayMode() const;

        /**
         * \brief Republishes the size bounds and brings the window back within them
         *
         * Merges the bounds currently set into the size hints of the window rather than
         * replacing them, so that setting one bound leaves the other alone, then resizes
         * the window if its current size falls outside the range. The window manager is
         * free to ignore the hints, which is why the resize is done here rather than left
         * to it.
         */
        void UpdateClampedSize();

        Handle mHandle;

        /// Non-owning cursor attached to the window, kept to be restored after it was hidden.
        Cursor* mCursor;

        /// Non-owning monitor cached to detect a window-monitor transition.
        Monitor* mCurrentMonitor;

        /// Last known cursor position, used to derive the relative displacement of a move.
        Extent2D<int> mCursorPosition;

        /// Fully transparent cursor attached to the window to hide the pointer, owned by this window.
        ::Cursor mHiddenCursor;

        /// Whether a resize sequence is in progress, X11 reporting no explicit start or end.
        bool mResizing;

        /// Last size received, used to tell a resize from a move since both arrive as a configure event.
        Extent2D<unsigned int> mLastSize;

        /// The minimum size of the window if any, empty otherwise.
        Optional<Extent2D<unsigned int>> mMinSize;

        /// The maximal size of the window if any, empty otherwise.
        Optional<Extent2D<unsigned int>> mMaxSize;

        /// When the last size change arrived, used to call a resize sequence over once it goes quiet.
        Optional<Instant> mLastSizeEvent;

        /// Size requested by SetSize, used to report the resulting configure event as settled.
        Optional<Extent2D<unsigned int>> mPendingResize;

        /// Flag indicating whether the cursor of the window is visible
        bool mCursorVisible;

        /// Flag indicating whether the window have been mapped by the window manager
        bool mWindowMapped;

        Signal<WindowCloseEvent> mOnClose;
        Signal<WindowFocusEvent> mOnFocus;
        Signal<WindowMovedEvent> mOnMoved;
        Signal<WindowResizingStartEvent> mOnResizingStart;
        Signal<WindowResizingEvent> mOnResizing;
        Signal<WindowResizingEndEvent> mOnResizingEnd;
        Signal<WindowResizedEvent> mOnResized;
        Signal<WindowTextEvent> mOnTextEvent;
        Signal<WindowKeyboardEvent> mOnKeyboard;
        Signal<WindowMouseMoveEvent> mOnMouseMoved;
        Signal<WindowMouseButtonEvent> mOnMouseButton;
        Signal<WindowMouseWheelEvent> mOnMouseWheel;
        Signal<WindowMonitorChangedEvent> mOnMonitorChangedEvent;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_WINDOW_HPP
