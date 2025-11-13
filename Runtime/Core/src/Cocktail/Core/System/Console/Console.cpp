#include <Cocktail/Core/System/Console/Console.hpp>

namespace Ck
{
	void Console::Write(const String& text)
	{
		ResolveFacadeInstance()->Write(text);
	}

	void Console::WriteLine(const String& text)
	{ 
		ResolveFacadeInstance()->Write(text);
	}

	void Console::Write(StringView text)
	{ 
		ResolveFacadeInstance()->Write(text);
	}

	void Console::WriteLine(StringView text)
	{
		ResolveFacadeInstance()->Write(text);
	}

	Writer& Console::GetOutput()
	{
		return ResolveFacadeInstance()->GetOutput();
	}

	Writer& Console::GetError()
	{
		return ResolveFacadeInstance()->GetError();
	}

	Reader& Console::GetInput()
	{
		return ResolveFacadeInstance()->GetInput();
	}

	void Console::Clear()
	{
		ResolveFacadeInstance()->Clear();
	}

	void Console::SetColors(ConsoleColor text, ConsoleColor background, ConsoleStyle style)
	{
		ResolveFacadeInstance()->SetColors(text, background, style);
	}

	void Console::Beep()
	{
		ResolveFacadeInstance()->Beep();
	}

	void Console::Beep(unsigned int frequency, const Duration& duration)
	{
		ResolveFacadeInstance()->Beep(frequency, duration);
	}
}
