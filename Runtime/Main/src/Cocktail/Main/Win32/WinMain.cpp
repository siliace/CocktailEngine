#include <Cocktail/Main/main.hpp>
#include <Cocktail/Main/Win32/Win32Application.hpp>

int main(int argc, char* argv[])
{
	HINSTANCE hInstance = GetModuleHandleW(nullptr);
	HINSTANCE hPrevInstance = nullptr;
	PWSTR pCmdLine = GetCommandLineW();  
	int nCmdShow = 0;

	Ck::Ref<Ck::Application> application = Ck::Main::Win32::Win32Application::New(
		hInstance, hPrevInstance, pCmdLine, nCmdShow
	);

	Ck::Main::ExitCode exitCode = Ck::Main::InvokeMain(application);

	return static_cast<int>(exitCode);
}
