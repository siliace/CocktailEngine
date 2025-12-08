#include <Cocktail/Main/main.hpp>
#include <Cocktail/Main/Unix/UnixApplication.hpp>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");

    Ck::UniquePtr<Ck::Application> application = Ck::MakeUnique<Ck::Main::Unix::UnixApplication>(
		argc, argv
	);

	Ck::Main::ExitCode exitCode = Ck::Main::InvokeMain(std::move(application));

    application.Reset();

	return static_cast<int>(exitCode);
}