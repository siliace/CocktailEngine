#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>
#include <Cocktail/Core/System/Window/Cursor.hpp>
#include <Cocktail/Core/System/Window/Xlib/AtomManager.hpp>
#include <Cocktail/Core/System/Window/Xlib/Window.hpp>

#ifndef Button6
    #define Button6 6
#endif // Button6

#ifndef Button7
    #define Button7 7
#endif // Button6

#ifndef Button8
    #define Button8 8
#endif // Button6

#ifndef Button9
    #define Button9 9
#endif // Button6

namespace Ck::Detail::Xlib
{
    namespace
    {
        static constexpr long EventsMasks = KeyPressMask | KeyReleaseMask |        /// Keyboard events
                                            PointerMotionMask | ButtonMotionMask | /// Mouse move events
                                            ButtonPressMask | ButtonReleaseMask |  /// Mouse buttons events
                                            FocusChangeMask |                      /// Focus events
                                            StructureNotifyMask |                  /// Resize events
                                            VisibilityChangeMask |                 /// Visibility change (internal uses only)
                                            EnterWindowMask | LeaveWindowMask |    /// Mouse Enter/Leave
                                            PropertyChangeMask;                    

        Optional<KeyboardKey> ConvertKeySym(KeySym keySym)
        {
            switch (keySym)
            {
            case XK_a: return Optional<KeyboardKey>::Of(KeyboardKey::A);
            case XK_b: return Optional<KeyboardKey>::Of(KeyboardKey::B);
            case XK_c: return Optional<KeyboardKey>::Of(KeyboardKey::C);
            case XK_d: return Optional<KeyboardKey>::Of(KeyboardKey::D);
            case XK_e: return Optional<KeyboardKey>::Of(KeyboardKey::E);
            case XK_f: return Optional<KeyboardKey>::Of(KeyboardKey::F);
            case XK_g: return Optional<KeyboardKey>::Of(KeyboardKey::G);
            case XK_h: return Optional<KeyboardKey>::Of(KeyboardKey::H);
            case XK_i: return Optional<KeyboardKey>::Of(KeyboardKey::I);
            case XK_j: return Optional<KeyboardKey>::Of(KeyboardKey::J);
            case XK_k: return Optional<KeyboardKey>::Of(KeyboardKey::K);
            case XK_l: return Optional<KeyboardKey>::Of(KeyboardKey::L);
            case XK_m: return Optional<KeyboardKey>::Of(KeyboardKey::M);
            case XK_n: return Optional<KeyboardKey>::Of(KeyboardKey::N);
            case XK_o: return Optional<KeyboardKey>::Of(KeyboardKey::O);
            case XK_p: return Optional<KeyboardKey>::Of(KeyboardKey::P);
            case XK_q: return Optional<KeyboardKey>::Of(KeyboardKey::Q);
            case XK_r: return Optional<KeyboardKey>::Of(KeyboardKey::R);
            case XK_s: return Optional<KeyboardKey>::Of(KeyboardKey::S);
            case XK_t: return Optional<KeyboardKey>::Of(KeyboardKey::T);
            case XK_u: return Optional<KeyboardKey>::Of(KeyboardKey::U);
            case XK_v: return Optional<KeyboardKey>::Of(KeyboardKey::V);
            case XK_w: return Optional<KeyboardKey>::Of(KeyboardKey::W);
            case XK_x: return Optional<KeyboardKey>::Of(KeyboardKey::X);
            case XK_y: return Optional<KeyboardKey>::Of(KeyboardKey::Y);
            case XK_z: return Optional<KeyboardKey>::Of(KeyboardKey::Z);
            case XK_0: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad0);
            case XK_1: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad1);
            case XK_2: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad2);
            case XK_3: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad3);
            case XK_4: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad4);
            case XK_5: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad5);
            case XK_6: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad6);
            case XK_7: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad7);
            case XK_8: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad8);
            case XK_9: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad9);
            case XK_Num_Lock: return Optional<KeyboardKey>::Of(KeyboardKey::NumLock);
            case XK_KP_0: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad0);
            case XK_KP_1: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad1);
            case XK_KP_2: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad2);
            case XK_KP_3: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad3);
            case XK_KP_4: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad4);
            case XK_KP_5: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad5);
            case XK_KP_6: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad6);
            case XK_KP_7: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad7);
            case XK_KP_8: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad8);
            case XK_KP_9: return Optional<KeyboardKey>::Of(KeyboardKey::NumPad9);
            case XK_KP_Add: return Optional<KeyboardKey>::Of(KeyboardKey::Add);
            case XK_KP_Subtract: return Optional<KeyboardKey>::Of(KeyboardKey::Subtract);
            case XK_KP_Multiply: return Optional<KeyboardKey>::Of(KeyboardKey::Multiply);
            case XK_KP_Divide: return Optional<KeyboardKey>::Of(KeyboardKey::Divide);
            case XK_F1: return Optional<KeyboardKey>::Of(KeyboardKey::F1);
            case XK_F2: return Optional<KeyboardKey>::Of(KeyboardKey::F2);
            case XK_F3: return Optional<KeyboardKey>::Of(KeyboardKey::F3);
            case XK_F4: return Optional<KeyboardKey>::Of(KeyboardKey::F4);
            case XK_F5: return Optional<KeyboardKey>::Of(KeyboardKey::F5);
            case XK_F6: return Optional<KeyboardKey>::Of(KeyboardKey::F6);
            case XK_F7: return Optional<KeyboardKey>::Of(KeyboardKey::F7);
            case XK_F8: return Optional<KeyboardKey>::Of(KeyboardKey::F8);
            case XK_F9: return Optional<KeyboardKey>::Of(KeyboardKey::F9);
            case XK_F10: return Optional<KeyboardKey>::Of(KeyboardKey::F10);
            case XK_F11: return Optional<KeyboardKey>::Of(KeyboardKey::F11);
            case XK_F12: return Optional<KeyboardKey>::Of(KeyboardKey::F12);
            case XK_F13: return Optional<KeyboardKey>::Of(KeyboardKey::F13);
            case XK_F14: return Optional<KeyboardKey>::Of(KeyboardKey::F14);
            case XK_F15: return Optional<KeyboardKey>::Of(KeyboardKey::F15);
            case XK_F16: return Optional<KeyboardKey>::Of(KeyboardKey::F16);
            case XK_F17: return Optional<KeyboardKey>::Of(KeyboardKey::F17);
            case XK_F18: return Optional<KeyboardKey>::Of(KeyboardKey::F18);
            case XK_F19: return Optional<KeyboardKey>::Of(KeyboardKey::F19);
            case XK_F20: return Optional<KeyboardKey>::Of(KeyboardKey::F20);
            case XK_F21: return Optional<KeyboardKey>::Of(KeyboardKey::F21);
            case XK_F22: return Optional<KeyboardKey>::Of(KeyboardKey::F22);
            case XK_F23: return Optional<KeyboardKey>::Of(KeyboardKey::F23);
            case XK_F24: return Optional<KeyboardKey>::Of(KeyboardKey::F24);
            case XK_Left: return Optional<KeyboardKey>::Of(KeyboardKey::Left);
            case XK_Right: return Optional<KeyboardKey>::Of(KeyboardKey::Right);
            case XK_Up: return Optional<KeyboardKey>::Of(KeyboardKey::Up);
            case XK_Down: return Optional<KeyboardKey>::Of(KeyboardKey::Down);
            case XK_space: return Optional<KeyboardKey>::Of(KeyboardKey::Space);
            case XK_Escape: return Optional<KeyboardKey>::Of(KeyboardKey::Escape);
            case XK_Control_L: return Optional<KeyboardKey>::Of(KeyboardKey::LeftControl);
            case XK_Control_R: return Optional<KeyboardKey>::Of(KeyboardKey::RightControl);
            case XK_Alt_L: return Optional<KeyboardKey>::Of(KeyboardKey::LeftAlt);
            case XK_Alt_R: return Optional<KeyboardKey>::Of(KeyboardKey::RightAlt);
            case XK_Super_L: return Optional<KeyboardKey>::Of(KeyboardKey::LeftSystem);
            case XK_Super_R: return Optional<KeyboardKey>::Of(KeyboardKey::RightSystem);
            case XK_Shift_L: return Optional<KeyboardKey>::Of(KeyboardKey::LeftShift);
            case XK_Shift_R: return Optional<KeyboardKey>::Of(KeyboardKey::RightShift);
            }
            return Optional<KeyboardKey>::Empty();
        }

        Optional<KeyboardKey> GetMessageKey(const XKeyEvent& keyEvent)
        {
            for (int i = 0; i < 4; i++)
            {
                KeySym keySym = XLookupKeysym(const_cast<XKeyEvent*>(&keyEvent), i); // Why 1st parameter is not const ?
                Optional<KeyboardKey> key = ConvertKeySym(keySym);
                if (key.IsEmpty())
                    continue;

                return key;
            }

            return Optional<KeyboardKey>::Empty();
        }

        MouseEventFlags GetMouseEventFlags(unsigned int xFlags)
        {
            MouseEventFlags flags;

            if (xFlags & Button1Mask)
                flags |= MouseEventFlagBits::LeftButton;
            
            if (xFlags & Button2Mask)
                flags |= MouseEventFlagBits::MiddleButton;
            
            if (xFlags & Button3Mask)
                flags |= MouseEventFlagBits::RightButton;
            
            if (xFlags & Button4Mask)
                flags |= MouseEventFlagBits::ExtraButton1;
            
            if (xFlags & Button5Mask)
                flags |= MouseEventFlagBits::ExtraButton2;
            
            if (xFlags & ShiftMask)
                flags |= MouseEventFlagBits::ShiftKey;
            
            if (xFlags & ControlMask)
                flags |= MouseEventFlagBits::ControlKey;

            return flags;
        }

        MouseButton GetMouseButton(int button)
        {
            switch (button)
            {
            case Button1: return MouseButton::Left;
            case Button2: return MouseButton::Middle;
            case Button3: return MouseButton::Right;
            case Button8: return MouseButton::Extra1;
            case Button9: return MouseButton::Extra2;
            }

            COCKTAIL_UNREACHABLE();
        }

        ::Cursor CreateHiddenCursor(::Display* display, ::Window window)
        {
            ::Pixmap cursorPixmap = XCreatePixmap(display, window, 1, 1, 1);
            ::GC graphicsContext = XCreateGC(display, cursorPixmap, 0, nullptr);
            XDrawPoint(display, cursorPixmap, graphicsContext, 0, 0);
            XFreeGC(display, graphicsContext);

            XColor color;
            color.flags = DoRed | DoGreen | DoBlue;
            color.red = color.blue = color.green = 0;
            ::Cursor hidden = XCreatePixmapCursor(display, cursorPixmap, cursorPixmap, &color, &color, 0, 0);

            XFreePixmap(display, cursorPixmap);

            return hidden;
        }
    }

    Window::Window(::Display* display, const WindowCreateInfo& createInfo) :
        mHiddenCursor(0),
        mMapped(false)
    {
        mHandle.Display = display;

        int screen = DefaultScreen(mHandle.Display);
        ::Visual* visual = XDefaultVisual(mHandle.Display, screen);

        ::Window parentHandle;
        if (createInfo.Parent)
        {
            Ck::Window* parent = createInfo.Parent;
            parentHandle = ((Handle*)parent->GetSystemHandle())->Window;
        }
        else
        {
            parentHandle = RootWindow(mHandle.Display, screen);
        }

        mHandle.Colormap = XCreateColormap(mHandle.Display, parentHandle, visual, AllocNone);

        XSetWindowAttributes attributes;
        attributes.border_pixel = 0;
        attributes.background_pixmap = 0;
        attributes.colormap = mHandle.Colormap;
        attributes.event_mask = EventsMasks;

        // Create the window
        mHandle.Window = XCreateWindow(
            mHandle.Display, 
            parentHandle, 
            createInfo.Position.Width, createInfo.Position.Height,
            createInfo.Size.Width, createInfo.Size.Height,
            0,
            DefaultDepth(mHandle.Display, DefaultScreen(mHandle.Display)),
            InputOutput,
            visual,
            CWColormap | CWEventMask | CWBorderPixel,
            &attributes
        );

        SetTitle(createInfo.Title);

        // Set the atoms used internaly
        App::Invoke([&](AtomManager* atomManager) {
            Atom deleteAtom = atomManager->GetAtom(CK_TEXT("WM_DELETE_WINDOW"));
            XSetWMProtocols(mHandle.Display, mHandle.Window, &deleteAtom, 1);

            Atom hintsAtom = atomManager->GetAtom(CK_TEXT("_MOTIF_WM_HINTS"), false);
            if (!hintsAtom)
                return;

            WMHints hints;
            Flags<WindowStyle> style = createInfo.Style;
            if (style != 0)
            {
                hints.decorations |= WMHints::Decor::Menu | WMHints::Decor::Title;
                hints.functions |= WMHints::Function::Move;
            }

            if (style & WindowStyle::Closable)
                hints.functions |= WMHints::Function::Close;

            if (style & WindowStyle::Resizable)
            {
                hints.decorations |= WMHints::Decor::ResizeH;
                hints.functions |= WMHints::Function::Resize;
            }

            if (style & WindowStyle::Minimizable)
            {
                hints.decorations |= WMHints::Decor::Minimize;
                hints.functions |= WMHints::Function::Minimize;
            }

            if (style & WindowStyle::Maximizable)
            {
                hints.decorations |= WMHints::Decor::Maximize;
                hints.functions |= WMHints::Function::Maximize;
            }

            XChangeProperty(
                mHandle.Display,
                mHandle.Window,
                hintsAtom,
                hintsAtom,
                32,
                PropModeReplace,
                reinterpret_cast<const unsigned char*>(&hints),
                5
            );
        });

        mLastSize = GetSize();
        mHiddenCursor = CreateHiddenCursor(mHandle.Display, mHandle.Window);

        SetVisible(true);
    }

    Window::~Window()
    {
        if (mHandle.Colormap)
            XFreeColormap(mHandle.Display, mHandle.Colormap);

        if (mHandle.Window)
            XDestroyWindow(mHandle.Display, mHandle.Window);

        if (mHiddenCursor)
            XFreeCursor(mHandle.Display, mHiddenCursor);

        XFlush(mHandle.Display);
    }

    bool Window::PollEvents()
    {
        ::XEvent xEvent;
        bool closeRequested = false;
        while (XPending(mHandle.Display))
        {
            XNextEvent(mHandle.Display, &xEvent);
            switch (xEvent.type)
            {
            case Expose:
                {
                    if (xEvent.xexpose.count == 0)
                    {
                        WindowRedrawEvent event;
                        event.Window = this;

                        mOnRedraw.Emit(event);
                    }
                }
                break;

            case ClientMessage:
                {
                    AtomManager* atomManager = App::Resolve<AtomManager>();
                    if (xEvent.xclient.message_type == atomManager->GetAtom(CK_TEXT("WM_PROTOCOLS")))
                    {
                        static const Atom wmDeleteWindow = atomManager->GetAtom(CK_TEXT("WM_DELETE_WINDOW"));
                        static const Atom netWmPing = atomManager->GetAtom(CK_TEXT("_NET_WM_PING"), false);
                        if (xEvent.xclient.format == 32 && xEvent.xclient.data.l[0] == static_cast<long>(wmDeleteWindow))
                        {
                            WindowCloseEvent closeEvent;
                            closeEvent.Window = this;
                            mOnClose.Emit(closeEvent);

                            XFreeColormap(mHandle.Display, mHandle.Colormap);
                            mHandle.Colormap = 0;

                            XDestroyWindow(mHandle.Display, mHandle.Window);
                            mHandle.Window = 0;

                            XFlush(mHandle.Display);

                            closeRequested = true;
                        }
                        else if (netWmPing != 0 && xEvent.xclient.format == 32 && xEvent.xclient.data.l[0] == static_cast<long>(netWmPing))
                        {
                            ::Window rootWindow = DefaultRootWindow(mHandle.Display);
                            xEvent.xclient.window = rootWindow;
                            XSendEvent(mHandle.Display, rootWindow, False, SubstructureNotifyMask | SubstructureRedirectMask, &xEvent);
                        }
                    }
                }
                break;

            case DestroyNotify:
                {
                    mHandle.Colormap = 0;
                    mHandle.Window = 0;
                }
                break;

            case MotionNotify:
                {  
                    WindowMouseMoveEvent event;
                    event.Window = this;
                    event.Flags = GetMouseEventFlags(xEvent.xmotion.state);
                    event.Position.Width = xEvent.xmotion.x;
                    event.Position.Height = xEvent.xmotion.y;
                    event.RelativeDisplacement = event.Position - mCursorPosition;

                    mOnMouseMoved.Emit(event);

                    mCursorPosition = event.Position;
                }
                break;

            case ConfigureNotify:
                {
                    Extent2D<int> position(xEvent.xconfigure.x, xEvent.xconfigure.y);
                    Extent2D<unsigned int> size(xEvent.xconfigure.width, xEvent.xconfigure.height);

                    if (size != mLastSize)
                    {
                        WindowResizedEvent event;
                        event.Window = this;
                        event.Size = size;
                        event.DisplayMode = WindowDisplayMode::Normal;

                        mOnResized.Emit(event);

                        mLastSize = size;
                    }
                    else
                    {
                        WindowMovedEvent event;
                        event.Window = this;
                        event.Position = position;

                        mOnMoved.Emit(event);
                    }
                }
                break;

            case ButtonPress:
            case ButtonRelease:
                {
                    const bool pressed = xEvent.type == ButtonPress;
                    const Extent2D<int> position(xEvent.xbutton.x, xEvent.xbutton.y);

                    if (xEvent.xbutton.button <= Button3 || xEvent.xbutton.button >= Button8)
                    {
                        WindowMouseButtonEvent event;
                        event.Window = this;
                        event.Button = GetMouseButton(xEvent.xbutton.button);
                        event.Flags = GetMouseEventFlags(xEvent.xbutton.state);
                        event.Type = pressed ? MouseButtonEvent::ClickType::Pressed : MouseButtonEvent::ClickType::Released;
                        event.Position = position;

                        mOnMouseButton.Emit(event);
                    }
                    else
                    {
                        WindowMouseWheelEvent event;
                        event.Window = this;
                        event.Flags = GetMouseEventFlags(xEvent.xbutton.state);
                        event.Position = position;

                        switch (xEvent.xbutton.button)
                        {
                        case Button4:
                            event.Wheel = MouseWheel::Vertical;
                            event.Delta = 1.f;
                            break;

                        case Button5:
                            event.Wheel = MouseWheel::Vertical;
                            event.Delta = -1.f;
                            break;

                        case Button6:
                            event.Wheel = MouseWheel::Horizontal;
                            event.Delta = 1.f;
                            break;

                        case Button7:
                            event.Wheel = MouseWheel::Horizontal;
                            event.Delta = -1.f;
                            break;
                        }

                        mOnMouseWheel.Emit(event);
                    }
                }
                break;

            case KeyPress:
            case KeyRelease:
                {
                    GetMessageKey(xEvent.xkey).Then([&](KeyboardKey key) {
                        WindowKeyboardEvent event;
                        event.Window = this;
                        event.Key = key;
                        event.Pressed = xEvent.type == KeyPress;
                        event.Repeated = false;

                        mOnKeyboard.Emit(event);
                    });
                }
                break;
            
            case UnmapNotify:
                {
                    if (xEvent.xunmap.window == mHandle.Window)
                        mMapped = false;
                }
                break;

            case VisibilityNotify:
                {
                    if (xEvent.xvisibility.state != VisibilityPartiallyObscured)
                        mMapped = true;
                }
                break;
            }
        }

        return !closeRequested;
    }

    void Window::Update()
    {   
        XEvent exposeEvent;
        exposeEvent.type = Expose;
        exposeEvent.xexpose.display = mHandle.Display;
        XSendEvent(mHandle.Display, mHandle.Window, False, ExposureMask, &exposeEvent);
    }

	bool Window::HasFocus()
	{
        ::Window focused;
        int revertTo;
        XGetInputFocus(mHandle.Display, &focused, &revertTo);

        return focused == mHandle.Window;
	}

	void Window::Minimize()
	{
	}

	bool Window::IsMinimized() const
	{
        return false;
	}

	void Window::Maximize()
	{
	}

	bool Window::IsMaximized() const
	{
        return false;
	}

    void Window::SetVisible(bool visible)
    {
        if (visible)
        {
            XMapWindow(mHandle.Display, mHandle.Window);
            XFlush(mHandle.Display);

            while (!mMapped)
                PollEvents();
        }
        else 
        {
            XUnmapWindow(mHandle.Display, mHandle.Window);
            XFlush(mHandle.Display);

            while (mMapped)
                PollEvents();
        }
    }

    std::shared_ptr<Cursor> Window::GetCursor() const
    {
        return mCursor;
    }

    void Window::SetCursor(std::shared_ptr<Cursor> cursor)
    {
        mCursor = std::move(cursor);
        XDefineCursor(mHandle.Display, mHandle.Window, (::Cursor)mCursor->GetSystemHandle());
    }

    bool Window::IsCursorVisible() const
    {
        return false;
    }

    void Window::SetCursorVisible(bool visible)
    {
        ::Cursor cursorHandle = visible ? (::Cursor)mCursor->GetSystemHandle() : mHiddenCursor;
        XDefineCursor(mHandle.Display, mHandle.Window, cursorHandle);
        XFlush(mHandle.Display);
    }

    void Window::EnableCaptureCursor(bool capture)
    {
        ::Display* display = mHandle.Display;
        ::Window window = mHandle.Window;

        if (capture)
        {
            XGrabPointer(display, window, True, 0, GrabModeAsync, GrabModeAsync, window, 0, CurrentTime);
        }
        else
        {
            XUngrabPointer(display, CurrentTime);
        }
	}

	void Window::SetIcon(const Image* icon)
	{
    }

    String Window::GetTitle() const
    {
        Utf8Char* buffer = COCKTAIL_STACK_ALLOC(Utf8Char, 256);
        XFetchName(mHandle.Display, mHandle.Window, reinterpret_cast<char**>(&buffer));

        return buffer;
    }

    void Window::SetTitle(const String& title)
    {
        XChangeProperty(mHandle.Display, mHandle.Window,
            XInternAtom(mHandle.Display, "_NET_WM_NAME", False),
            XInternAtom(mHandle.Display, "UTF8_STRING", False),
            8, PropModeReplace,
            reinterpret_cast<const unsigned char*>(title.GetData()), title.GetLength()
        );
    }

    Extent2D<unsigned int> Window::GetSize() const
    {
        XWindowAttributes windowAttributes;
        XGetWindowAttributes(mHandle.Display, mHandle.Window, &windowAttributes);
        return Extent2D<unsigned int>(windowAttributes.width, windowAttributes.height);
    }

    void Window::SetSize(const Extent2D<unsigned int>& size)
    {
        XResizeWindow(mHandle.Display, mHandle.Window, size.Width, size.Height);
        XFlush(mHandle.Display);
    }

	Extent2D<int> Window::GetPosition() const
    {
        XWindowAttributes windowAttributes;
        XGetWindowAttributes(mHandle.Display, mHandle.Window, &windowAttributes);
        return Extent2D<int>(windowAttributes.x, windowAttributes.y);

    }

	void Window::SetPosition(const Extent2D<int>& position)
    {
        XMoveWindow(mHandle.Display, mHandle.Window, position.Width, position.Height);
        XFlush(mHandle.Display);
    }

	Extent2D<unsigned int> Window::GetMinSize() const
	{
        XSizeHints sizeHints;
        XGetNormalHints(mHandle.Display, mHandle.Window, &sizeHints);
        return Extent2D<unsigned int>(sizeHints.min_width, sizeHints.min_height);
	}

	void Window::SetMinSize(const Extent2D<unsigned int>& size)
	{   
        Extent2D<unsigned int> currentSize = GetSize();

        XSizeHints sizeHints;
        sizeHints.flags = PMinSize;
        sizeHints.min_width = size.Width;
        sizeHints.min_height = size.Height;
        XSetNormalHints(mHandle.Display, mHandle.Window, &sizeHints);

        Extent2D<unsigned int> clampedSize;
        clampedSize.Width = std::max(size.Width, currentSize.Width);
        clampedSize.Height = std::max(size.Height, currentSize.Height);

        SetSize(clampedSize);
	}

	Extent2D<unsigned int> Window::GetMaxSize() const
	{
        XSizeHints sizeHints;
        XGetNormalHints(mHandle.Display, mHandle.Window, &sizeHints);
        return Extent2D<unsigned int>(sizeHints.max_width, sizeHints.max_height);
	}

	void Window::SetMaxSize(const Extent2D<unsigned int>& size)
	{
        Extent2D<unsigned int> currentSize = GetSize();

        XSizeHints sizeHints;
        sizeHints.flags = PMaxSize;
        sizeHints.max_width = size.Width;
        sizeHints.max_height = size.Height;
        XSetNormalHints(mHandle.Display, mHandle.Window, &sizeHints);

        Extent2D<unsigned int> clampedSize;
        clampedSize.Width = std::min(size.Width, currentSize.Width);
        clampedSize.Height = std::min(size.Height, currentSize.Height);

        SetSize(clampedSize);
	}

	Signal<WindowCloseEvent>& Window::OnCloseEvent()
    {
        return mOnClose;
    }
	
    Signal<WindowRedrawEvent>& Window::OnRedrawEvent()
    {
        return mOnRedraw;
    }
	
    Signal<WindowFocusEvent>& Window::OnFocusEvent()
    {
        return mOnFocus;
    }
	
    Signal<WindowMovedEvent>& Window::OnMovedEvent()
    {
        return mOnMoved;
    }
	
    Signal<WindowResizedEvent>& Window::OnResizedEvent()
    {
        return mOnResized;
    }
	
    Signal<WindowTextEvent>& Window::OnTextEvent()
    {
        return mOnTextEvent;
    }
	
    Signal<WindowKeyboardEvent>& Window::OnKeyboardEvent()
    {
        return mOnKeyboard;
    }
	
    Signal<WindowMouseMoveEvent>& Window::OnMouseMovedEvent()
    {
        return mOnMouseMoved;
    }
	
    Signal<WindowMouseButtonEvent>& Window::OnMouseButtonEvent()
    {
        return mOnMouseButton;
    }

	Signal<WindowMouseWheelEvent>& Window::OnMouseWheelEvent()
    {
        return mOnMouseWheel;
    }

    void* Window::GetSystemHandle() const
    {
        return (void*)&mHandle;
    }
}
