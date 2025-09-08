#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Window/Win32/Window.hpp>

namespace Ck::Detail::Win32
{
	namespace
	{
		DWORD ComputeStyle(const Flags<WindowStyle>& style, bool visible)
		{
			DWORD windowStyle = 0;

			if (style & WindowStyle::Titlebar)
				windowStyle |= WS_CAPTION | WS_MINIMIZEBOX;

			if (style & WindowStyle::Closable)
				windowStyle |= WS_SYSMENU;

			if (style & WindowStyle::Resizable)
				windowStyle |= WS_THICKFRAME | WS_MAXIMIZEBOX;

			if (visible)
				windowStyle |= WS_VISIBLE;

			return windowStyle;
		}

		Extent2D<unsigned int> AdjustWindowSize(Extent2D<unsigned int> size, DWORD style)
		{
			RECT rectangle = {
				0,
				0,
				static_cast<LONG>(size.Width),
				static_cast<LONG>(size.Height)
			};

			if (AdjustWindowRect(&rectangle, style, false) == FALSE)
				throw SystemError::GetLastError();

			Extent2D<unsigned int> adjusted;
			adjusted.Width = rectangle.right - rectangle.left;
			adjusted.Height = rectangle.bottom - rectangle.top;

			return adjusted;
		}
	}

	Window::Window(const WindowCreateInfo& createInfo, const WindowClass& windowClass) :
		mHandle(nullptr),
		mCursor(nullptr),
		mIcon(nullptr),
		mCursorVisible(true)
	{
		DWORD style = ComputeStyle(createInfo.Style, createInfo.Visible);
		HWND parentHandle = createInfo.Parent ? static_cast<HWND>(createInfo.Parent->GetSystemHandle()) : nullptr;

		Extent2D<unsigned int> adjustedSize = AdjustWindowSize(createInfo.Size, style);

		mHandle = CreateWindowEx(
			0,
			MAKEINTATOM(windowClass.GetHandle()),
			createInfo.Title.GetData(),
			style,
			createInfo.Position.Width, createInfo.Position.Height,
			static_cast<int>(adjustedSize.Width), static_cast<int>(adjustedSize.Height),
			parentHandle,
			nullptr,
			nullptr,
			this
		);

		if (mHandle == nullptr)
			throw SystemError::GetLastError();
	}

	bool Window::PollEvents()
	{
		MSG message{};
		while (PeekMessage(&message, mHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		return IsWindow(mHandle) == TRUE;
	}

	void Window::Update()
	{
		RECT rect;
		if (GetClientRect(mHandle, &rect) == FALSE)
			throw SystemError::GetLastError();

		if (InvalidateRect(mHandle, &rect, FALSE) == FALSE)
			throw SystemError::GetLastError();

		if (UpdateWindow(mHandle) == FALSE)
			throw SystemError::GetLastError();
	}

	void Window::SetVisible(bool visible)
	{
		if (ShowWindow(mHandle, visible ? SW_RESTORE : SW_HIDE))
			throw SystemError::GetLastError();
	}

	bool Window::HasFocus()
	{
		return GetFocus() == mHandle;
	}

	void Window::Minimize()
	{
		if (ShowWindow(mHandle, SW_SHOWMINIMIZED) == FALSE)
			throw SystemError::GetLastError();
	}

	bool Window::IsMinimized() const
	{
		return IsIconic(mHandle) == TRUE;
	}

	void Window::Maximize()
	{
		if (ShowWindow(mHandle, SW_SHOWMAXIMIZED) == FALSE)
			throw SystemError::GetLastError();
	}

	bool Window::IsMaximized() const
	{
		WINDOWPLACEMENT placement;
		placement.length = sizeof(WINDOWPLACEMENT);

		if (GetWindowPlacement(mHandle, &placement) == FALSE)
			throw SystemError::GetLastError();

		return placement.showCmd == SW_MAXIMIZE;
	}

	std::shared_ptr<Cursor> Window::GetCursor() const
	{
		return mCursor;
	}

	void Window::SetCursor(std::shared_ptr<Cursor> cursor)
	{
		mCursor = cursor;
	}

	bool Window::IsCursorVisible() const
	{
		return mCursorVisible;
	}

	void Window::SetCursorVisible(bool visible)
	{
		mCursorVisible = visible;
	}

	void Window::EnableCaptureCursor(bool capture)
	{
		if (capture)
		{
			RECT winRect;
			if (GetWindowRect(mHandle, &winRect) == FALSE)
				throw SystemError::GetLastError();

			if (ClipCursor(&winRect) == FALSE)
				throw SystemError::GetLastError();
		}
		else
		{
			if (ClipCursor(nullptr) == FALSE)
				throw SystemError::GetLastError();
		}
	}

	void Window::SetIcon(const Image* icon)
	{
		if (mIcon)
			DestroyIcon(mIcon);

		if (icon)
		{
			Extent2D<unsigned int> size = icon->GetSize();
			PixelFormat iconFormat = icon->GetFormat();
			assert(iconFormat.GetLayout() == PixelFormat::Layout::RGBA);

			std::size_t allocationSize = iconFormat.ComputeAllocationSize(icon->GetSize());
			std::unique_ptr<unsigned char[]> bgraPixels = std::make_unique<unsigned char[]>(allocationSize);

			const unsigned char* pixels = icon->GetPixels().GetData();
			for (std::size_t i = 0; i < allocationSize / 4; i++)
			{
				bgraPixels[i * 4 + 0] = pixels[i * 4 + 2];
				bgraPixels[i * 4 + 1] = pixels[i * 4 + 1];
				bgraPixels[i * 4 + 2] = pixels[i * 4 + 0];
				bgraPixels[i * 4 + 3] = pixels[i * 4 + 3];
			}

			mIcon = CreateIcon(
				GetModuleHandle(nullptr), 
				static_cast<int>(size.Width), static_cast<int>(size.Height), 
				1, 
				32, 
				nullptr, 
				bgraPixels.get()
			);

			if (!mIcon)
				throw SystemError::GetLastError();

			SendMessageW(mHandle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(mIcon));
			SendMessageW(mHandle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(mIcon));
		}
		else
		{
			mIcon = nullptr;
		}
	}

	String Window::GetTitle() const
	{
		std::size_t titleLength = GetWindowTextLength(mHandle);
		if (titleLength == 0)
		{
			DWORD lastError = GetLastError();
			if (lastError)
				throw std::system_error(static_cast<int>(lastError), SystemError::GetSystemErrorCategory());

			return String::Empty;
		}

		TextChar* title = COCKTAIL_STACK_ALLOC(TextChar, titleLength);
		int result = GetWindowText(mHandle, title, titleLength);
		if (result == 0)
			throw SystemError::GetLastError();

		return String(title, titleLength);
	}

	void Window::SetTitle(const String& title)
	{
		if (SetWindowText(mHandle, title.GetData()) == FALSE)
			throw SystemError::GetLastError();
	}

	Extent2D<unsigned int> Window::GetSize() const
	{
		RECT rect;
		if (GetClientRect(mHandle, &rect) == FALSE)
			throw SystemError::GetLastError();

		return {static_cast<unsigned int>(rect.right - rect.left), static_cast<unsigned int>(rect.bottom - rect.top)};
	}

	void Window::SetSize(const Extent2D<unsigned int>& size)
	{
		DWORD style = static_cast<DWORD>(GetWindowLongPtr(mHandle, GWL_STYLE));
		Extent2D<unsigned int> adjustedSize = AdjustWindowSize(size, style);
		if (SetWindowPos(mHandle, nullptr, 0, 0, adjustedSize.Width, adjustedSize.Height, SWP_NOMOVE | SWP_NOZORDER) == FALSE)
			throw SystemError::GetLastError();
	}

	Extent2D<int> Window::GetPosition() const
	{
		RECT rect;
		if (GetClientRect(mHandle, &rect) == FALSE)
			throw SystemError::GetLastError();

		return { rect.left, rect.top };
	}

	void Window::SetPosition(const Extent2D<int>& position)
	{
		if (SetWindowPos(mHandle, nullptr, position.Width, position.Height, 0, 0, SWP_NOSIZE | SWP_NOZORDER) == FALSE)
			throw SystemError::GetLastError();
	}

	Extent2D<unsigned int> Window::GetMinSize() const
	{
		return mMinSize.GetOr(MakeExtent(0u, 0u));
	}

	void Window::SetMinSize(const Extent2D<unsigned int>& size)
	{
		if (size != MakeExtent(0u, 0u))
		{
			Extent2D<unsigned int> currentSize = GetSize();

			Extent2D<unsigned int> clampedSize;
			clampedSize.Width = std::max(currentSize.Width, size.Width);
			clampedSize.Height = std::max(currentSize.Height, size.Height);

			Uint32 style = static_cast<Uint32>(GetWindowLongPtr(mHandle, GWL_STYLE));
			mMinSize = Optional<Extent2D<unsigned int>>::Of(
				AdjustWindowSize(clampedSize, style)
			);

			SetSize(mMinSize.Get());
		}
		else
		{
			mMinSize = Optional<Extent2D<unsigned int>>::Empty();
		}
	}

	Extent2D<unsigned int> Window::GetMaxSize() const
	{
		return mMaxSize.GetOr(MakeExtent(0u, 0u));
	}

	void Window::SetMaxSize(const Extent2D<unsigned int>& size)
	{
		if (size != MakeExtent(0u, 0u))
		{
			Extent2D<unsigned int> currentSize = GetSize();

			Extent2D<unsigned int> clampedSize;
			clampedSize.Width = std::min(currentSize.Width, size.Width);
			clampedSize.Height = std::min(currentSize.Height, size.Height);

			Uint32 style = static_cast<Uint32>(GetWindowLongPtr(mHandle, GWL_STYLE));
			mMaxSize = Optional<Extent2D<unsigned int>>::Of(
				AdjustWindowSize(clampedSize, style)
			);

			SetSize(mMaxSize.Get());
		}
		else
		{
			mMaxSize = Optional<Extent2D<unsigned int>>::Empty();
		}
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
		return mHandle;
	}
}
