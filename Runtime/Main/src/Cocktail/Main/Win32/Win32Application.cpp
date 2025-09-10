#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Main/main.hpp>
#include <Cocktail/Main/Win32/Win32Application.hpp>

namespace Ck::Main::Win32
{
	Win32Application::Win32Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
	{
		int argc = 0;
		PWSTR* wideArgument = CommandLineToArgvW(pCmdLine, &argc);

		mArgv.Reserve(argc);
		for (int i = 0; i < argc; i++)
			mArgv.Emplace(wideArgument[i]);
	}

	void Win32Application::Exit(unsigned int exitCode, bool force, StringView callSite)
	{
		CK_LOG(MainLogCategory, LogLevel::Info, CK_TEXT("Requested {} exit with code {} from {}"), force ? CK_TEXT("forced") : CK_TEXT("soft"), exitCode, callSite);

		if (force)
		{
			TerminateProcess(GetCurrentProcess(), exitCode);
		}
		else
		{
			Terminate();

			PostQuitMessage(exitCode);
		}
	}

	const Array<String>& Win32Application::GetArgv() const
	{
		return mArgv;
	}

	Optional<String> Win32Application::GetEnvironmentVariable(StringView name)
	{
		DWORD variableLength = ::GetEnvironmentVariableW(name.GetData(), nullptr, 0);
		if (!variableLength)
			return Optional<String>::Empty();

		TextChar* variable = COCKTAIL_STACK_ALLOC(TextChar, variableLength);
		GetEnvironmentVariableW(name.GetData(), variable, variableLength);

		String value;
		value.Append(variable, variableLength);

		return Optional<String>::Of(value);
	}

	bool Win32Application::IsDebuggerPresent() const
	{
		return ::IsDebuggerPresent() == TRUE;
	}
}
