#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/StringView.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleService.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	namespace
	{
		WORD ConsoleColorToBackgroundAttribute(ConsoleColor color)
		{
			switch (color)
			{
			case ConsoleColor::Black:
				return 0;
			case ConsoleColor::DarkRed:
				return BACKGROUND_RED;
			case ConsoleColor::DarkBlue:
				return BACKGROUND_BLUE;
			case ConsoleColor::DarkGreen:
				return BACKGROUND_GREEN;
			case ConsoleColor::Red:
				return BACKGROUND_INTENSITY | BACKGROUND_RED;
			case ConsoleColor::Blue:
				return BACKGROUND_INTENSITY | BACKGROUND_BLUE;
			case ConsoleColor::DarkCyan:
				return BACKGROUND_GREEN | BACKGROUND_BLUE;
			case ConsoleColor::DarkMagenta:
				return BACKGROUND_RED | BACKGROUND_BLUE;
			case ConsoleColor::Green:
				return BACKGROUND_INTENSITY | BACKGROUND_GREEN;
			case ConsoleColor::DarkYellow:
				return BACKGROUND_RED | BACKGROUND_GREEN;
			case ConsoleColor::Magenta:
				return BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE;
			case ConsoleColor::Cyan:
				return BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE;
			case ConsoleColor::DarkGray:
				return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
			case ConsoleColor::Yellow:
				return BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN;
			case ConsoleColor::White:
				return BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
			}

			COCKTAIL_UNREACHABLE();
		}

		WORD ConsoleColorToForegroundAttribute(ConsoleColor color)
		{
			switch (color)
			{
			case ConsoleColor::Black:
				return 0;
			case ConsoleColor::DarkRed:
				return FOREGROUND_RED;
			case ConsoleColor::DarkBlue:
				return FOREGROUND_BLUE;
			case ConsoleColor::DarkGreen:
				return FOREGROUND_GREEN;
			case ConsoleColor::Gray:
				return FOREGROUND_INTENSITY;
			case ConsoleColor::Red:
				return FOREGROUND_INTENSITY | FOREGROUND_RED;
			case ConsoleColor::Blue:
				return FOREGROUND_INTENSITY | FOREGROUND_BLUE;
			case ConsoleColor::DarkCyan:
				return FOREGROUND_GREEN | FOREGROUND_BLUE;
			case ConsoleColor::DarkMagenta:
				return FOREGROUND_RED | FOREGROUND_BLUE;
			case ConsoleColor::Green:
				return FOREGROUND_INTENSITY | FOREGROUND_GREEN;
			case ConsoleColor::DarkYellow:
				return FOREGROUND_RED | FOREGROUND_GREEN;
			case ConsoleColor::Magenta:
				return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
			case ConsoleColor::Cyan:
				return FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
			case ConsoleColor::DarkGray:
				return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			case ConsoleColor::Yellow:
				return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
			case ConsoleColor::White:
				return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			}

			COCKTAIL_UNREACHABLE();
		}
	}

	ConsoleService::ConsoleService() :
		mOutput(ConsoleWriter::FromOutputHandle()),
		mError(ConsoleWriter::FromErrorHandle()),
		mInput()
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (GetConsoleScreenBufferInfo(handle, &info) == TRUE)
		{
			mIntialAttributes = info.wAttributes;
		}
		else
		{
			// When the console does not support this function (like the console in CLion IDE) assume theses parameters
			WORD backgroundAttriubte = ConsoleColorToBackgroundAttribute(ConsoleColor::Black);
			WORD foregroundAttribute = ConsoleColorToForegroundAttribute(ConsoleColor::White);
			mIntialAttributes = backgroundAttriubte | foregroundAttribute;
		}
	}

	ConsoleService::~ConsoleService()
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, mIntialAttributes);
	}

	void ConsoleService::Write(const String& text)
	{
		GetOutput().Write(text.GetData(), text.GetLength());
	}

	void ConsoleService::WriteLine(const String& text)
	{
		static const StringView EndLine = CK_TEXT("\r\n");
		if (text.EndsWith(EndLine))
		{
			Write(text);
		}
		else
		{
			String line = text;
			line.Append(EndLine);

			return Write(line);
		}
	}

	void ConsoleService::Write(StringView text)
	{
		GetOutput().Write(text.GetData(), text.GetLength());
	}

	void ConsoleService::WriteLine(StringView text)
	{
		static const StringView EndLine = CK_TEXT("\r\n");
		if (text.EndsWith(EndLine))
		{
			Write(text);
		}
		else
		{
			String line(text);
			line.Append(EndLine);

			return Write(line);
		}
	}

	Writer& ConsoleService::GetOutput()
	{
		return mOutput;
	}

	Writer& ConsoleService::GetError()
	{
		return mError;
	}

	Reader& ConsoleService::GetInput()
	{
		return mInput;
	}

	void ConsoleService::Clear()
	{
		COORD screen = { 0, 0 };
		CONSOLE_SCREEN_BUFFER_INFO info;
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

		if (GetConsoleScreenBufferInfo(handle, &info) == FALSE)
			throw SystemError::GetLastError();

		DWORD consoleSize = info.dwSize.X * info.dwSize.Y;

		if (FillConsoleOutputCharacter(handle, ' ', consoleSize, screen, nullptr) == FALSE)
			throw SystemError::GetLastError();

		GetConsoleScreenBufferInfo(handle, &info);

		if (FillConsoleOutputAttribute(handle, info.wAttributes, consoleSize, screen, nullptr) == FALSE)
			throw SystemError::GetLastError();

		SetConsoleCursorPosition(handle, screen);
	}

	void ConsoleService::SetColors(ConsoleColor background, ConsoleColor foreground)
	{
		WORD backgroundAttriubte = ConsoleColorToBackgroundAttribute(background);
		WORD foregroundAttribute = ConsoleColorToForegroundAttribute(foreground);

		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD attributes = backgroundAttriubte | foregroundAttribute;

		if (SetConsoleTextAttribute(handle, attributes) == FALSE)
			throw SystemError::GetLastError();
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
