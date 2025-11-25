#include <Cocktail/Core/Memory/UniquePtr.hpp>

#include <Cocktail/Main/main.hpp>
#include <Cocktail/Main/Win32/Win32Application.hpp>

extern "C" unsigned long NvOptimusEnablement = 1;
extern "C" unsigned long AmdPowerXpressRequestHighPerformance = 1;

int main(int argc, char* argv[])
{
	HINSTANCE hInstance = GetModuleHandleW(nullptr);
	HINSTANCE hPrevInstance = nullptr;
	PWSTR pCmdLine = GetCommandLineW();  
	int nCmdShow = 0;

	Ck::UniquePtr<Ck::Application> application = Ck::MakeUnique<Ck::Main::Win32::Win32Application>(
		hInstance, hPrevInstance, pCmdLine, nCmdShow
	);

	Ck::Main::ExitCode exitCode = Ck::Main::InvokeMain(application.Get());

	return static_cast<int>(exitCode);
}
