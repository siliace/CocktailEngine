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

	std::unique_ptr<Ck::Application> application = std::make_unique<Ck::Main::Win32::Win32Application>(
		hInstance, hPrevInstance, pCmdLine, nCmdShow
	);

	Ck::Main::ExitCode exitCode = Ck::Main::InvokeMain(application.get());

	return static_cast<int>(exitCode);
}
