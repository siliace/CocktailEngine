#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleService.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	namespace
	{
		WORD ConsoleColorToTextAttribute(ConsoleColor color)
		{
			switch (color)
			{
			case ConsoleColor::Black: return 0;
			case ConsoleColor::Red:	return FOREGROUND_RED;
			case ConsoleColor::Green: return FOREGROUND_GREEN;
			case ConsoleColor::Yellow: return FOREGROUND_RED | FOREGROUND_GREEN;
			case ConsoleColor::Blue: return FOREGROUND_BLUE;
			case ConsoleColor::Magenta:	return FOREGROUND_BLUE | FOREGROUND_RED;
			case ConsoleColor::Cyan: return FOREGROUND_BLUE | FOREGROUND_GREEN;
			case ConsoleColor::White: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			case ConsoleColor::BrightBlack: return FOREGROUND_INTENSITY;
			case ConsoleColor::BrightRed:	return FOREGROUND_RED | FOREGROUND_INTENSITY;
			case ConsoleColor::BrightGreen: return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			case ConsoleColor::BrightYellow: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			case ConsoleColor::BrightBlue: return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			case ConsoleColor::BrightMagenta:	return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
			case ConsoleColor::BrightCyan: return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			case ConsoleColor::BrightWhite: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			case ConsoleColor::Transparent:	return 0;
			}

			COCKTAIL_UNREACHABLE();
		}

		WORD ConsoleColorToBackgroundAttribute(ConsoleColor color)
		{
			switch (color)
			{
			case ConsoleColor::Black: return 0 << 4;
			case ConsoleColor::Red:	return BACKGROUND_RED;
			case ConsoleColor::Green: return BACKGROUND_GREEN;
			case ConsoleColor::Yellow: return BACKGROUND_RED | BACKGROUND_GREEN;
			case ConsoleColor::Blue: return BACKGROUND_BLUE;
			case ConsoleColor::Magenta:	return BACKGROUND_BLUE | BACKGROUND_RED;
			case ConsoleColor::Cyan: return BACKGROUND_BLUE | BACKGROUND_GREEN;
			case ConsoleColor::White: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
			case ConsoleColor::BrightBlack: return BACKGROUND_INTENSITY;
			case ConsoleColor::BrightRed:	return BACKGROUND_RED | BACKGROUND_INTENSITY;
			case ConsoleColor::BrightGreen: return BACKGROUND_GREEN | BACKGROUND_INTENSITY;
			case ConsoleColor::BrightYellow: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
			case ConsoleColor::BrightBlue: return BACKGROUND_BLUE | BACKGROUND_INTENSITY;
			case ConsoleColor::BrightMagenta:	return BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
			case ConsoleColor::BrightCyan: return BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
			case ConsoleColor::BrightWhite: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
			case ConsoleColor::Transparent:	return 0;
			}

			COCKTAIL_UNREACHABLE();
		}
	}

	ConsoleService::ConsoleService() :
		mOutput(ConsoleWriter::FromOutputHandle()),
		mError(ConsoleWriter::FromErrorHandle())
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (GetConsoleScreenBufferInfo(handle, &info) == TRUE)
		{
			mInitialAttributes = info.wAttributes;
		}
		else
		{
			// When the console does not support this function (like the console in CLion IDE)
			WORD textAttribute = ConsoleColorToTextAttribute(ConsoleColor::White);
			WORD backgroundAttribute = ConsoleColorToBackgroundAttribute(ConsoleColor::Transparent);
			mInitialAttributes = textAttribute | backgroundAttribute;
		}
	}

	ConsoleService::~ConsoleService()
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, mInitialAttributes);
	}

	Writer<>& ConsoleService::GetOutput()
	{
		return mOutput;
	}

	Writer<>& ConsoleService::GetError()
	{
		return mError;
	}

	Reader<>& ConsoleService::GetInput()
	{
		return mInput;
	}

	void ConsoleService::Clear()
	{
		COORD screen = { 0, 0 };
		CONSOLE_SCREEN_BUFFER_INFO info;
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		if (GetConsoleScreenBufferInfo(consoleHandle, &info) == FALSE)
			throw SystemError::GetLastError();

		DWORD consoleSize = info.dwSize.X * info.dwSize.Y;

		if (FillConsoleOutputCharacter(consoleHandle, ' ', consoleSize, screen, nullptr) == FALSE)
			throw SystemError::GetLastError();

		GetConsoleScreenBufferInfo(consoleHandle, &info);

		if (FillConsoleOutputAttribute(consoleHandle, info.wAttributes, consoleSize, screen, nullptr) == FALSE)
			throw SystemError::GetLastError();

		SetConsoleCursorPosition(consoleHandle, screen);
	}

	void ConsoleService::SetColors(ConsoleColor text, ConsoleColor background, ConsoleStyle)
	{
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &consoleScreenBufferInfo);

		WORD textAttributes = ConsoleColorToTextAttribute(text);
		WORD backgroundAttributes = ConsoleColorToBackgroundAttribute(background);

		WORD currentAttributes = consoleScreenBufferInfo.wAttributes;

		if (textAttributes == 0 && text == ConsoleColor::Transparent)
			textAttributes = currentAttributes & 0x0F;

		if (backgroundAttributes == 0 && background == ConsoleColor::Transparent)
			backgroundAttributes = currentAttributes & 0xF0;

		// Appliquer
		SetConsoleTextAttribute(consoleHandle, textAttributes | backgroundAttributes);
	}

	void ConsoleService::Beep() const
	{
		Beep(440, Duration::Seconds(1));
	}

	void ConsoleService::Beep(unsigned int frequency, const Duration& duration) const
	{
		::Beep(
			frequency, 
			duration.As(TimeUnit::Milliseconds()).GetCount()
		);
	}
}
