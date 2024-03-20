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
	}

	Window::Window(const WindowCreateInfo& createInfo, const WindowClass& windowClass) :
		mHandle(nullptr),
		mCursor(nullptr)
	{
		DWORD style = ComputeStyle(createInfo.Style, createInfo.Visible);
		HWND parentHandle = createInfo.Parent ? static_cast<HWND>(createInfo.Parent->GetSystemHandle()) : nullptr;

		// Compute position and size
		LONG left = createInfo.Position.Width;
		LONG top = createInfo.Position.Height;
		LONG width = static_cast<int>(createInfo.Size.Width);
		LONG height = static_cast<int>(createInfo.Size.Height);

		RECT rectangle = {0, 0, width, height};
		AdjustWindowRect(&rectangle, style, false);
		width = rectangle.right - rectangle.left;
		height = rectangle.bottom - rectangle.top;

		mHandle = CreateWindowEx(
			0,
			MAKEINTATOM(windowClass.GetHandle()),
			createInfo.Title.c_str(),
			style,
			left, top,
			width, height,
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

	Ref<Cursor> Window::GetCursor() const
	{
		return mCursor;
	}

	void Window::SetCursor(Ref<Cursor> cursor)
	{
		mCursor = cursor;
	}

	std::string Window::GetTitle()
	{
		std::size_t titleLength = GetWindowTextLength(mHandle);
		if (titleLength == 0)
		{
			DWORD lastError = GetLastError();
			if (lastError)
				throw std::system_error(static_cast<int>(lastError), SystemError::GetSystemErrorCategory());
		}

		std::string title(titleLength + 1, '\0');
		int result = GetWindowText(mHandle, &title[0], title.size());
		if (result == 0)
			throw SystemError::GetLastError();

		return title;
	}

	void Window::SetTitle(const std::string& title)
	{
		if (SetWindowText(mHandle, title.c_str()) == FALSE)
			throw SystemError::GetLastError();
	}

	Extent2D<unsigned int> Window::GetSize()
	{
		RECT rect;
		if (GetClientRect(mHandle, &rect) == FALSE)
			throw SystemError::GetLastError();

		return {static_cast<unsigned int>(rect.right - rect.left), static_cast<unsigned int>(rect.bottom - rect.top)};
	}

	void Window::SetSize(const Extent2D<unsigned int>& size)
	{
		LONG left = 0;
		LONG top = 0;
		LONG width = static_cast<int>(size.Width);
		LONG height = static_cast<int>(size.Height);

		RECT rectangle = {left, top, width, height};
		DWORD style = static_cast<DWORD>(GetWindowLongPtr(mHandle, GWL_STYLE));

		if (AdjustWindowRect(&rectangle, style, false) == FALSE)
			throw SystemError::GetLastError();

		width = rectangle.right - rectangle.left;
		height = rectangle.bottom - rectangle.top;

		if (SetWindowPos(mHandle, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER) == FALSE)
			throw SystemError::GetLastError();
	}

	Extent2D<int> Window::GetPosition()
	{
		return MakeExtent(0, 0);
	}

	void Window::SetPosition(const Extent2D<int>& position)
	{
		if (SetWindowPos(mHandle, nullptr, position.Width, position.Height, 0, 0, SWP_NOSIZE | SWP_NOZORDER) == FALSE)
			throw SystemError::GetLastError();
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
