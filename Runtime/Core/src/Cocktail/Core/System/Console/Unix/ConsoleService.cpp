#include <Cocktail/Core/System/Console/Unix/ConsoleService.hpp>

namespace Ck::Detail::Unix
{
	namespace
	{
		int ConsoleColorToTextAttribute(ConsoleColor color)
		{
			switch (color)
			{
			case ConsoleColor::Black:
			case ConsoleColor::Red:
			case ConsoleColor::Green:
			case ConsoleColor::Yellow:
			case ConsoleColor::Blue:
			case ConsoleColor::Magenta:
			case ConsoleColor::Cyan:
			case ConsoleColor::White:
				return static_cast<int>(color) + 30;

			case ConsoleColor::BrightBlack:
			case ConsoleColor::BrightRed:
			case ConsoleColor::BrightGreen:
			case ConsoleColor::BrightYellow:
			case ConsoleColor::BrightBlue:
			case ConsoleColor::BrightMagenta:
			case ConsoleColor::BrightCyan:
			case ConsoleColor::BrightWhite:
				return 90 + (static_cast<int>(color) - 8);

			case ConsoleColor::Transparent:
				return 0;
			}

			COCKTAIL_UNREACHABLE();
			return 0;
		}

		int ConsoleColorToBackgroundAttribute(ConsoleColor color)
		{
			switch (color)
			{
			case ConsoleColor::Black:
			case ConsoleColor::Red:
			case ConsoleColor::Green:
			case ConsoleColor::Yellow:
			case ConsoleColor::Blue:
			case ConsoleColor::Magenta:
			case ConsoleColor::Cyan:
			case ConsoleColor::White:
				return static_cast<int>(color) + 40;

			case ConsoleColor::BrightBlack:
			case ConsoleColor::BrightRed:
			case ConsoleColor::BrightGreen:
			case ConsoleColor::BrightYellow:
			case ConsoleColor::BrightBlue:
			case ConsoleColor::BrightMagenta:
			case ConsoleColor::BrightCyan:
			case ConsoleColor::BrightWhite:
				return 100 + (static_cast<int>(color) - 8);

			case ConsoleColor::Transparent:
				return 0;
			}

			COCKTAIL_UNREACHABLE();
			return 0;
		}
	}

	ConsoleService::ConsoleService() : 
        mOutput(ConsoleWriter::FromOutputHandle()),
        mError(ConsoleWriter::FromErrorHandle())
	{
        /// Nothing
	}

    void ConsoleService::Write(const String& text)
	{
	    GetOutput().Write(text.GetData(), text.GetLength());
	}

    void ConsoleService::WriteLine(const String& text)
	{
	    static const StringView EndLine = CK_TEXT("\n");
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
	    static const StringView EndLine = CK_TEXT("\n");
	    if (text.EndsWith(EndLine))
	    {
	        Write(text);
	    }
	    else
	    {
	        String line = String::FromView(text);
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
		static const StringView ClearString = CK_TEXT("\e[1;1H\e[2J");
		Write(ClearString);
	}

	void ConsoleService::SetColors(ConsoleColor text, ConsoleColor background, ConsoleStyle style)
	{
		int styleCode = static_cast<int>(style);
		int textCode = ConsoleColorToTextAttribute(text);
		int backgroundCode = ConsoleColorToBackgroundAttribute(background);

		if (text != ConsoleColor::Transparent)
		{
			if (background != ConsoleColor::Transparent)
			{
				Write(String::Format(CK_TEXT("\033[%d;%d;%dm"), styleCode, textCode, backgroundCode));
			}
			else
			{
				Write(String::Format(CK_TEXT("\033[%d;%dm"), styleCode, textCode));
			}
		}
		else if (background != ConsoleColor::Transparent)
		{
			Write(String::Format(CK_TEXT("\033[%dm"), backgroundCode));
		}
	}

	void ConsoleService::Beep() const
	{
	}

	void ConsoleService::Beep(unsigned int frequency, const Duration& duration) const
	{
	}
}
