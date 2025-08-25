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
		{
			int wideArgumentLength = wcslen(wideArgument[i]);
			int argumentLength = WideCharToMultiByte(CP_UTF8, 0, wideArgument[i], wideArgumentLength, nullptr, 0, nullptr, nullptr);

			std::string argument(argumentLength, 0);
			WideCharToMultiByte(CP_UTF8, 0, wideArgument[i], wideArgumentLength, argument.data(), argumentLength, nullptr, nullptr);

			mArgv.Add(std::move(argument));
		}

		LocalFree(wideArgument);
	}

	void Win32Application::Exit(unsigned exitCode, bool force, std::string_view callSite)
	{
		CK_LOG(MainLogCategory, LogLevel::Info, "Requested {} exit with code {} from {}", force ? "forced" : "soft", exitCode, callSite.empty() ? "<>" : callSite);

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

	const Array<std::string>& Win32Application::GetArgv() const
	{
		return mArgv;
	}

	std::string Win32Application::GetEnvironmentVariable(std::string_view name)
	{
		std::string value;
		DWORD variableLength = ::GetEnvironmentVariableA(name.data(), nullptr, 0);
		if (variableLength)
		{
			char* buffer = COCKTAIL_STACK_ALLOC(char, variableLength);
			::GetEnvironmentVariableA(name.data(), buffer, variableLength);

			value.assign(buffer, variableLength - 1);
		}

		return value;
	}

	bool Win32Application::IsDebuggerPresent() const
	{
		return ::IsDebuggerPresent() == TRUE;
	}
}
