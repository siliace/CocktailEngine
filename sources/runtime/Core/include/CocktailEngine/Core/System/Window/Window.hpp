#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOW_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOW_HPP

#include <CocktailEngine/Core/Image/Image.hpp>
#include <CocktailEngine/Core/Signal/Observable.hpp>
#include <CocktailEngine/Core/Signal/Signal.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/System/Window/WindowEvent.hpp>

namespace Ck
{
    class Cursor;

    /**
     * \brief Interface of a system window, the surface an application draws into and takes its input from
     *
     * A Window wraps a native window of the underlying windowing system and republishes
     * its events as Signal emissions. Instances are created through
     * WindowFactory::CreateWindow, which transfers ownership to the caller.
     *
     * Events are only published from inside PollEvents, which the owner is expected to
     * call once per frame. A window and its event pump belong to the thread that created
     * the window, and every connected callback runs on that thread, nested inside the
     * PollEvents call that produced the event. The references returned by the signal
     * accessors stay valid for the whole lifetime of the window, so connecting to them
     * once after creation is enough.
     *
     * Platform behavior may vary depending on the underlying windowing system
     * (e.g. Win32, X11, Wayland). A request a platform cannot honor is ignored rather
     * than reported, so the per-function notes below are the only description of what is
     * actually supported.
     *
     * Window derives from Observable to own the connections its own backend establishes.
     * It is not the subscription point for consumers: those connect through their own
     * Observable, which is the Application in most cases.
     *
     * \see WindowFactory::CreateWindow
     */
    class Window : public Observable
    {
    public:

        /**
         * \brief Virtual destructor
         *
         * Destroying a window releases the native window behind it, so the handle returned
         * by GetSystemHandle and every cursor association go with it.
         *
         * An implementation whose own callbacks read its members has to disconnect them
         * here rather than rely on the base class, which releases the connections it owns
         * only once the derived members are already gone.
         */
        virtual ~Window() = default;

        /**
         * \brief Processes the events the system queued for this window
         *
         * Drains the pending native events and translates each of them into the matching
         * signal emission. Connected callbacks run synchronously, before this function
         * returns, which is why a window the user just closed is already reported as gone
         * by the return value of that same call.
         *
         * This is the only function that publishes events, and it must be called from the
         * thread that created the window.
         *
         * \return True if the window is still open after the processed events, false once it is gone
         */
        virtual bool PollEvents() = 0;

        /**
         * \brief Shows or hides the window
         *
         * Hiding a window keeps it alive and keeps its events flowing; only its mapping on
         * the screen is dropped.
         *
         * \param visible True to show the window, false to hide it
         */
        virtual void SetVisible(bool visible) = 0;

        /**
         * \brief Indicates whether this window currently holds the keyboard focus
         *
         * \return True if the window has the focus, false otherwise
         */
        virtual bool HasFocus() = 0;

        /**
         * \brief Minimizes the window to the taskbar or dock
         *
         * Requires WindowStyle::Minimizable to have been requested at creation for the
         * user to be able to perform the same action.
         */
        virtual void Minimize() = 0;

        /**
         * \brief Indicates whether the window is currently minimized
         *
         * \return True if the window is minimized, false otherwise
         */
        virtual bool IsMinimized() const = 0;

        /**
         * \brief Maximizes the window to the work area of its monitor
         *
         * Requires WindowStyle::Maximizable to have been requested at creation for the
         * user to be able to perform the same action.
         */
        virtual void Maximize() = 0;

        /**
         * \brief Indicates whether the window is currently maximized
         *
         * \return True if the window is maximized, false otherwise
         */
        virtual bool IsMaximized() const = 0;

        /**
         * \brief Retrieves the cursor displayed over the client area of this window
         *
         * \return Non-owning pointer to the current cursor, or nullptr if none was set
         */
        virtual Cursor* GetCursor() const = 0;

        /**
         * \brief Sets the cursor displayed over the client area of this window
         *
         * The window keeps a non-owning pointer to the cursor, so the caller is
         * responsible for keeping it alive at least as long as it stays in use. System
         * cursors obtained from WindowFactory::LoadSystemCursor live as long as the
         * factory, whereas an ImageCursor is owned by whoever created it.
         *
         * \param cursor Non-owning cursor to display, which must outlive this window or be replaced before it dies
         */
        virtual void SetCursor(Cursor* cursor) = 0;

        /**
         * \brief Indicates whether the cursor is visible over the client area of this window
         *
         * \return True if the cursor is visible, false if it is hidden
         */
        virtual bool IsCursorVisible() const = 0;

        /**
         * \brief Change cursor visibility in the area of the window
         *
         * Only applies over the client area: the cursor stays visible over the decorations
         * and outside the window. Hiding it does not confine it, so the cursor keeps
         * moving and keeps reporting positions.
         *
         * \param visible True to show the cursor over the client area, false to hide it
         */
        virtual void SetCursorVisible(bool visible) = 0;

        /**
         * \brief Enable or disable the capture of the cursor inside the window
         *
         * While captured, the cursor cannot leave the window, which is what a mouse-look
         * camera needs. The capture is a system-wide state the window takes for itself, so
         * it must be released before the window is destroyed.
         *
         * \param capture The state of the capture
         */
        virtual void EnableCaptureCursor(bool capture) = 0;

        /**
         * \brief Sets the icon of the window, displayed in its title bar and in the task switcher
         *
         * The image is converted to a native icon and the window keeps no reference to it,
         * so it can be released as soon as this function returns. The image is expected to
         * carry the ImageRawFormat::Type::RedGreenBlueAlpha8 raw format.
         *
         * \param image Image to use as the icon, or nullptr to drop the current one
         */
        virtual void SetIcon(const Image* image) = 0;

        /**
         * \brief Retrieves the title displayed in the title bar of the window
         *
         * \return The current window title
         */
        virtual String GetTitle() const = 0;

        /**
         * \brief Sets the title displayed in the title bar of the window
         *
         * Only observable when the window was created with WindowStyle::Titlebar, though
         * the title also names the window for the task switcher and the window manager.
         *
         * \param title The title to display
         */
        virtual void SetTitle(const String& title) = 0;

        /**
         * \brief Retrieves the size in pixels of the client area of the window
         *
         * The result excludes the decorations, so it is the size that matters to a render
         * surface rather than the footprint of the window on the screen.
         *
         * \return The client area size as a 2D extent (width, height)
         */
        virtual Extent2D<unsigned int> GetSize() const = 0;

        /**
         * \brief Resizes the client area of the window
         *
         * The requested size is the one of the client area, the decorations being
         * accounted for on top of it. A window manager is free to refuse or to adjust the
         * request, which is why the resulting size is reported by a WindowResizedEvent
         * instead of being returned here.
         *
         * \param size The client area size to apply, in pixels
         */
        virtual void SetSize(const Extent2D<unsigned int>& size) = 0;

        /**
         * \brief Retrieves the position of the window on the virtual screen
         *
         * Coordinates are expressed in the virtual screen space shared by every monitor,
         * where a secondary monitor placed left of the primary one yields negative
         * abscissas. Width carries the abscissa and Height the ordinate.
         *
         * \return The window position as a 2D extent (x, y)
         */
        virtual Extent2D<int> GetPosition() const = 0;

        /**
         * \brief Moves the window on the virtual screen
         *
         * Coordinates follow the same convention as GetPosition. Moving a window across a
         * monitor boundary surfaces as a WindowMonitorChangedEvent.
         *
         * \param position The position to apply, as a 2D extent (x, y)
         */
        virtual void SetPosition(const Extent2D<int>& position) = 0;

        /**
         * \brief Retrieves the lower bound a user resize cannot go below
         *
         * \return The minimum client area size, or a null extent when no bound is set
         */
        virtual Extent2D<unsigned int> GetMinSize() const = 0;

        /**
         * \brief Constrains how small the user can make the window
         *
         * Only constrains the interactive resize, not SetSize. The window is grown
         * immediately when it is already smaller than the requested bound. Passing a null
         * extent drops the constraint.
         *
         * The bound must not exceed the maximum size on either axis when one is set.
         *
         * \param size The minimum client area size to enforce, or a null extent to remove the bound
         */
        virtual void SetMinSize(const Extent2D<unsigned int>& size) = 0;

        /**
         * \brief Retrieves the upper bound a user resize cannot go above
         *
         * \return The maximum client area size, or a null extent when no bound is set
         */
        virtual Extent2D<unsigned int> GetMaxSize() const = 0;

        /**
         * \brief Constrains how large the user can make the window
         *
         * Only constrains the interactive resize, not SetSize. The window is shrunk
         * immediately when it is already larger than the requested bound. Passing a null
         * extent drops the constraint.
         *
         * The bound must not be smaller than the minimum size on either axis when one is
         * set.
         *
         * \param size The maximum client area size to enforce, or a null extent to remove the bound
         */
        virtual void SetMaxSize(const Extent2D<unsigned int>& size) = 0;

        /**
         * \brief Retrieves the signal emitted when the window is about to be closed
         *
         * The window is still usable while the callbacks run, but it is gone by the time
         * the enclosing PollEvents returns false. This signal is the place to ask the
         * application to exit; it carries no veto.
         *
         * \return The close signal
         */
        virtual Signal<WindowCloseEvent>& OnCloseEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when the window gains or loses the keyboard focus
         *
         * \return The focus signal
         */
        virtual Signal<WindowFocusEvent>& OnFocusEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when the window has been moved on the screen
         *
         * \return The move signal
         */
        virtual Signal<WindowMovedEvent>& OnMovedEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when the user starts an interactive resize
         *
         * Opens the sequence closed by OnResizingEndEvent, during which OnResizingEvent is
         * emitted repeatedly. Only an interactive resize produces this sequence: SetSize,
         * Maximize and Minimize go straight to OnResizedEvent.
         *
         * \return The resize-start signal
         */
        virtual Signal<WindowResizingStartEvent>& OnResizingStartEvent() = 0;

        /**
         * \brief Retrieves the signal emitted repeatedly while the user drags a window border
         *
         * Reports the size the window is about to take, which makes it the signal to watch
         * to follow a resize live rather than wait for it to settle. Emitted only between
         * OnResizingStartEvent and OnResizingEndEvent.
         *
         * \return The resizing signal
         */
        virtual Signal<WindowResizingEvent>& OnResizingEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when the user ends an interactive resize
         *
         * Closes the sequence opened by OnResizingStartEvent. The final size is not
         * carried here but by the WindowResizedEvent of the settled window.
         *
         * \return The resize-end signal
         */
        virtual Signal<WindowResizingEndEvent>& OnResizingEndEvent() = 0;

        /**
         * \brief Retrieves the signal emitted every time the client area took a new size
         *
         * Fired for every size change whatever its cause, a drag, a maximize, a minimize
         * or a SetSize call, which makes it the one signal a render surface has to follow.
         * A minimize reports a null size along WindowDisplayMode::Minimized.
         *
         * \return The resized signal
         */
        virtual Signal<WindowResizedEvent>& OnResizedEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when the user typed a character
         *
         * Carries the text the keyboard layout and the input method produced, which is
         * what a text field needs, as opposed to the physical keys reported by
         * OnKeyboardEvent. The two are emitted independently for the same keystroke.
         *
         * \return The text signal
         */
        virtual Signal<WindowTextEvent>& OnTextEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when the user presses or releases a key
         *
         * Reports physical keys, independently of the keyboard layout. Text entry is
         * reported by OnTextEvent instead.
         *
         * \return The keyboard signal
         */
        virtual Signal<WindowKeyboardEvent>& OnKeyboardEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when the cursor moves over the window
         *
         * \return The mouse-move signal
         */
        virtual Signal<WindowMouseMoveEvent>& OnMouseMovedEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when the user presses or releases a mouse button
         *
         * \return The mouse-button signal
         */
        virtual Signal<WindowMouseButtonEvent>& OnMouseButtonEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when the user rolls a mouse wheel
         *
         * \return The mouse-wheel signal
         */
        virtual Signal<WindowMouseWheelEvent>& OnMouseWheelEvent() = 0;

        /**
         * \brief Retrieves the signal emitted when this window moves to another monitor
         *
         * The event reports the monitor association before and after the transition.
         *
         * \return The monitor-change signal
         */
        virtual Signal<WindowMonitorChangedEvent>& OnMonitorChangedEvent() = 0;

        /**
         * \brief Retrieves the underlying system-specific handle
         *
         * What the returned pointer designates is backend-specific, so only code already
         * written against a given windowing system can make sense of it. It is the native
         * window handle itself on Win32, and a pointer to the display, window and input
         * context of the window on X11.
         *
         * \return Non-owning pointer to the native window handle, valid as long as the window is
         */
        virtual void* GetSystemHandle() const = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOW_HPP
