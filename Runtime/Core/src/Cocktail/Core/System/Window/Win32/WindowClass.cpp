#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowClass.hpp>

namespace Ck::Detail::Win32
{
	WindowClass::WindowClass(HINSTANCE instanceHandle, WNDPROC windowProc, const TextChar* className) :
		mInstanceHandle(instanceHandle)
	{
		WNDCLASSW windowClass;
		windowClass.style = 0;
		windowClass.lpfnWndProc = windowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = mInstanceHandle;
		windowClass.hIcon = nullptr;
		windowClass.hCursor = nullptr;
		windowClass.hbrBackground = nullptr;
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = className;
		
		mHandle = RegisterClass(&windowClass);
		if (mHandle == INVALID_ATOM)
			throw SystemError::GetLastError();
	}

	WindowClass::~WindowClass()
	{
		UnregisterClass(MAKEINTATOM(mHandle), mInstanceHandle);
	}

	ATOM WindowClass::GetHandle() const
	{
		return mHandle;
	}
}
