#include <Cocktail/Core/System/Console/Console.hpp>

namespace Ck
{
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

	void Console::SetColors(ConsoleColor background, ConsoleColor foreground)
	{
		ResolveFacadeInstance()->SetColors(background, foreground);
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
