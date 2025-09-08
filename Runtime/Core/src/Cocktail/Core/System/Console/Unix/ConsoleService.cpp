#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/System/Console/Unix/ConsoleService.hpp>

namespace Ck::Detail::Unix
{
	namespace
	{
		int ConsoleColorToTextAttribute(ConsoleColor color)
        {
            switch(color)
            {
                case ConsoleColor::Red:
                    return 0;
                case ConsoleColor::Blue:
                    return 91;
                case ConsoleColor::Cyan:
                    return 96;
                case ConsoleColor::Gray:
                    return 37;
                case ConsoleColor::Black:
                    return 30;
                case ConsoleColor::Green:
                    return 92;
                case ConsoleColor::White:
                    return 39;
                case ConsoleColor::Yellow:
                    return 93;
                case ConsoleColor::DarkRed:
                    return 31;
                case ConsoleColor::Magenta:
                    return 95;
                case ConsoleColor::DarkBlue:
                    return 34;
                case ConsoleColor::DarkCyan:
                    return 36;
                case ConsoleColor::DarkGray:
                    return 90;
                case ConsoleColor::DarkGreen:
                    return 32;
                case ConsoleColor::DarkYellow:
                    return 33;
                case ConsoleColor::DarkMagenta:
                    return 35;
            }

            COCKTAIL_UNREACHABLE();
		    return 0;
        }

        int ConsoleColorToBackgroundAttribute(ConsoleColor color)
        {
            switch(color)
            {
                case ConsoleColor::Red:
                    return 101;
                case ConsoleColor::Blue:
                    return 104;
                case ConsoleColor::Cyan:
                    return 106;
                case ConsoleColor::Gray:
                    return 47;
                case ConsoleColor::Black:
                    return 49;
                case ConsoleColor::Green:
                    return 102;
                case ConsoleColor::White:
                    return 107;
                case ConsoleColor::Yellow:
                    return 103;
                case ConsoleColor::DarkRed:
                    return 41;
                case ConsoleColor::Magenta:
                    return 105;
                case ConsoleColor::DarkBlue:
                    return 44;
                case ConsoleColor::DarkCyan:
                    return 46;
                case ConsoleColor::DarkGray:
                    return 100;
                case ConsoleColor::DarkGreen:
                    return 42;
                case ConsoleColor::DarkYellow:
                    return 43;
                case ConsoleColor::DarkMagenta:
                    return 45;
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
		static const StringView ClearString = CK_TEXT("\e[1;1H\e[2J");
		Write(ClearString);
	}

	void ConsoleService::SetColors(ConsoleColor background, ConsoleColor foreground)
	{
		Write(String::Format(CK_TEXT("\033[%dm"), ConsoleColorToTextAttribute(background)));
		Write(String::Format(CK_TEXT("\033[%dm"), ConsoleColorToBackgroundAttribute(background)));
	}

	void ConsoleService::Beep() const
	{
	}

	void ConsoleService::Beep(unsigned int frequency, const Duration& duration) const
	{
	}
}
