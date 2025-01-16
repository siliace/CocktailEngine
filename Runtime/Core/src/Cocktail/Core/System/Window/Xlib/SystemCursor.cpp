#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Window/Xlib/SystemCursor.hpp>

namespace Ck::Detail::Xlib
{
	namespace
	{
		int ToXlibSystemCursorType(SystemCursorType type)
		{
			switch (type)
			{
			case SystemCursorType::Arrow: return XC_cross;
			case SystemCursorType::ArrowWait: return XC_watch;
			case SystemCursorType::Wait: return XC_watch;
			case SystemCursorType::Text: return XC_xterm;
            case SystemCursorType::Hand: return XC_hand2;
			case SystemCursorType::SizeHorizontal: return XC_sb_h_double_arrow;
			case SystemCursorType::SizeVertical: return XC_sb_v_double_arrow;
        	case SystemCursorType::SizeLeft: return XC_left_side;  
        	case SystemCursorType::SizeRight: return XC_right_side; 
        	case SystemCursorType::SizeTop: return XC_top_side;   
        	case SystemCursorType::SizeBottom: return XC_bottom_side;
        	case SystemCursorType::SizeTopLeft: return XC_top_left_corner;
        	case SystemCursorType::SizeBottomRight: return XC_bottom_right_corner;
        	case SystemCursorType::SizeBottomLeft: return XC_bottom_left_corner;
        	case SystemCursorType::SizeTopRight: return XC_top_right_corner;
        	case SystemCursorType::SizeAll: return XC_fleur;
			case SystemCursorType::Cross: return XC_crosshair;
            case SystemCursorType::Help: return XC_question_arrow;
			case SystemCursorType::NotAllowed: return XC_X_cursor;
			}

			COCKTAIL_UNREACHABLE();
		}
	}

	SystemCursor::SystemCursor(::Display* display, SystemCursorType type) :
		mDisplay(display),
		mType(type)
	{
		mHandle = XCreateFontCursor(mDisplay, ToXlibSystemCursorType(mType));
	}

    SystemCursor::~SystemCursor()
    {
		XFreeCursor(mDisplay, mHandle);
    }

	SystemCursorType SystemCursor::GetType() const
	{
		return mType;
	}

	void* SystemCursor::GetSystemHandle() const
	{
		return reinterpret_cast<void*>(mHandle);
	}
}
