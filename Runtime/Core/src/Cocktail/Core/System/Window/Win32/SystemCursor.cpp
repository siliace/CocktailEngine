#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Window/Win32/SystemCursor.hpp>

namespace Ck::Detail::Win32
{
	namespace
	{
		const char* ToWin32SystemCursorType(SystemCursorType type)
		{
			switch (type)
			{
			case SystemCursorType::Arrow: return IDC_ARROW;
			case SystemCursorType::ArrowWait: return IDC_APPSTARTING;
			case SystemCursorType::Wait: return IDC_WAIT;
			case SystemCursorType::Text: return IDC_IBEAM;
			case SystemCursorType::Hand: return IDC_HAND;
			case SystemCursorType::SizeHorizontal: return IDC_SIZEWE;
			case SystemCursorType::SizeVertical: return IDC_SIZENS;
			case SystemCursorType::SizeTopLeftBottomRight: return IDC_SIZENWSE;
			case SystemCursorType::SizeBottomLeftTopRight: return IDC_SIZENESW;
			case SystemCursorType::SizeLeft: return IDC_SIZEWE;
			case SystemCursorType::SizeRight: return IDC_SIZEWE;
			case SystemCursorType::SizeTop: return IDC_SIZENS;
			case SystemCursorType::SizeBottom: return IDC_SIZENS;
			case SystemCursorType::SizeTopLeft: return IDC_SIZENWSE;
			case SystemCursorType::SizeBottomRight: return IDC_SIZENWSE;
			case SystemCursorType::SizeBottomLeft: return IDC_SIZENESW;
			case SystemCursorType::SizeTopRight: return IDC_SIZENESW;
			case SystemCursorType::SizeAll: return IDC_SIZEALL;
			case SystemCursorType::Cross: return IDC_CROSS;
			case SystemCursorType::Help: return IDC_HELP;
			case SystemCursorType::NotAllowed: return IDC_NO;
			}

			COCKTAIL_UNREACHABLE();
		}
	}

	SystemCursor::SystemCursor(SystemCursorType type):
		mType(type)
	{
		mHandle = LoadCursor(nullptr, ToWin32SystemCursorType(mType));
		if (!mHandle)
			throw SystemError::GetLastError();
	}

	SystemCursorType SystemCursor::GetType() const
	{
		return mType;
	}

	void* SystemCursor::GetSystemHandle() const
	{
		return mHandle;
	}
}
