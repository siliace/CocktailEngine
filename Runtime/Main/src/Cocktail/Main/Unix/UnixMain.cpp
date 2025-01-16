#include <Cocktail/Main/main.hpp>
#include <Cocktail/Main/Unix/UnixApplication.hpp>

int main(int argc, char* argv[])
{
    std::unique_ptr<Ck::Application> application = std::make_unique<Ck::Main::Unix::UnixApplication>(
		argc, argv
	);

	Ck::Main::ExitCode exitCode = Ck::Main::InvokeMain(application.get());

	return static_cast<int>(exitCode);
}