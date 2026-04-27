#include <Cocktail/Core/System/Console/Console.hpp>

namespace Ck
{
	LineWriter<> Console::GetOutput()
	{
		return ResolveFacadeInstance()->GetOutput();
	}

	LineWriter<> Console::GetError()
	{
		return ResolveFacadeInstance()->GetError();
	}

	LineReader<> Console::GetInput()
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
