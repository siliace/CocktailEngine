#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOW_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOW_HPP

#include <CocktailEngine/Core/System/Window/Win32/WindowClass.hpp>
#include <CocktailEngine/Core/System/Window/Window.hpp>
#include <CocktailEngine/Core/System/Window/WindowCreateInfo.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief Win32 implementation of a Window, backed by an HWND
     *
     * Holds the native window and the signals it publishes, but does not translate the
     * native messages itself: that is the job of the WindowEventDispatcher the window
     * procedure creates alongside each window. The dispatcher calls back into the signal
     * accessors below, which is why they are the only state the two share.
     *
     * The handle is created on, and bound to, the thread running the constructor, since a
     * Win32 message queue belongs to a thread. PollEvents has to run on that same thread.
     *
     * Most of the overrides translate a failed Win32 call into a SystemError, so the
     * accessors of this class throw where the interface returns.
     */
    class Window final : public Ck::Window
    {
    public:

        /**
         * \brief Creates the native window and binds it to the calling thread
         *
         * The requested client-area size is inflated to account for the decorations the
         * requested style implies, so that the client area ends up the size that was asked
         * for. The window is registered with the window procedure through the creation
         * parameter, which is how the dispatcher finds it back.
         *
         * \param createInfo The structure describing the Window to create
         * \param windowClass The registered window class to instantiate, owned by the factory and expected to outlive this window
         */
        Window(const WindowCreateInfo& createInfo, const WindowClass& windowClass);

        ~Window() override;

        bool PollEvents() override;

        void SetVisible(bool visible) override;

        bool HasFocus() override;

        void Minimize() override;

        bool IsMinimized() const override;

        void Maximize() override;

        bool IsMaximized() const override;

        Cursor* GetCursor() const override;

        void SetCursor(Cursor* cursor) override;

        bool IsCursorVisible() const override;

        /**
         * \brief Change cursor visibility in the area of the window
         *
         * Only records the wish. Win32 asks for the shape of the cursor through
         * WM_SETCURSOR as it enters the client area, so the window procedure is what
         * eventually applies this flag, and the change is only observable on the next
         * cursor move.
         *
         * \param visible True to show the cursor over the client area, false to hide it
         */
        void SetCursorVisible(bool visible) override;

        /**
         * \brief Enable or disable the capture of the cursor inside the window
         *
         * Clips the cursor to the screen rectangle of the window. The clipping region is a
         * system-wide setting rather than a property of the window, so the last caller
         * wins and the region survives the window that set it.
         *
         * \param capture The state of the capture
         */
        void EnableCaptureCursor(bool capture) override;

        void SetIcon(const Image* icon) override;

        String GetTitle() const override;

        void SetTitle(const String& title) override;

        Extent2D<unsigned> GetSize() const override;

        void SetSize(const Extent2D<unsigned>& size) override;

        Extent2D<int> GetPosition() const override;

        void SetPosition(const Extent2D<int>& position) override;

        Extent2D<unsigned int> GetMinSize() const override;

        void SetMinSize(const Extent2D<unsigned int>& size) override;

        Extent2D<unsigned int> GetMaxSize() const override;

        void SetMaxSize(const Extent2D<unsigned int>& size) override;

        /**
         * \brief Retrieves the minimum size of the whole window, decorations included
         *
         * Win32 constrains an interactive resize in terms of the outer frame, whereas the
         * bound the caller sets is a client area size, so the conversion happens here. The
         * result is empty when no bound is set, and also when the conversion failed, in
         * which case the window is simply left unconstrained rather than reporting an
         * error from inside the window procedure.
         *
         * \return The minimum tracking size, or an empty optional if there is none to report
         */
        Optional<Extent2D<unsigned int>> GetMinTrackSize() const;

        /**
         * \brief Retrieves the maximum size of the whole window, decorations included
         *
         * Counterpart of GetMinTrackSize, with the same conversion and the same meaning for
         * an empty result.
         *
         * \return The maximum tracking size, or an empty optional if there is none to report
         */
        Optional<Extent2D<unsigned int>> GetMaxTrackSize() const;

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
         * \return The HWND of this window, as an untyped pointer
         */
        void* GetSystemHandle() const override;

    private:

        /**
         * \brief Brings the window back within its size bounds
         *
         * The bounds themselves are advertised to the window manager through
         * WM_GETMINMAXINFO, which only constrains what the user can do. This is what
         * applies them to a window that already sits outside the range when a bound is set.
         */
        void UpdateClampedSize();

        HWND mHandle;

        /// Non-owning cursor displayed over the client area, applied on WM_SETCURSOR.
        Cursor* mCursor;

        /// Minimum client area size, empty while no bound is set.
        Optional<Extent2D<unsigned int>> mMinSize;

        /// Maximum client area size, empty while no bound is set.
        Optional<Extent2D<unsigned int>> mMaxSize;

        /// Native icon built from the last SetIcon call, owned by this window.
        HICON mIcon;

        /// Wish recorded by SetCursorVisible, read back by the window procedure.
        bool mCursorVisible;

        Signal<WindowCloseEvent> mOnClose;
        Signal<WindowFocusEvent> mOnFocus;
        Signal<WindowMovedEvent> mOnMoved;
        Signal<WindowResizingStartEvent> mOnResizingStartEvent;
        Signal<WindowResizingEvent> mOnResizingEvent;
        Signal<WindowResizingEndEvent> mOnResizingEndEvent;
        Signal<WindowResizedEvent> mOnResized;
        Signal<WindowTextEvent> mOnTextEvent;
        Signal<WindowKeyboardEvent> mOnKeyboard;
        Signal<WindowMouseMoveEvent> mOnMouseMoved;
        Signal<WindowMouseButtonEvent> mOnMouseButton;
        Signal<WindowMouseWheelEvent> mOnMouseWheel;
        Signal<WindowMonitorChangedEvent> mOnMonitorChangedEvent;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOW_HPP
