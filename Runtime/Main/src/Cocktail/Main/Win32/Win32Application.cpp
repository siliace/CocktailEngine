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

	const Array<std::string>& Win32Application::GetArgv() const
	{
		return mArgv;
	}

	bool Win32Application::IsDebuggerPresent() const
	{
		return ::IsDebuggerPresent() == TRUE;
	}
}
