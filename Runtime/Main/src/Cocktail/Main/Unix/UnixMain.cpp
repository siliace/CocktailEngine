#include <Cocktail/Main/main.hpp>
#include <Cocktail/Main/Unix/UnixApplication.hpp>

int main(int argc, char* argv[])
{
    Ck::Ref<Ck::Application> application = Ck::Main::Unix::UnixApplication::New(
		argc, argv
	);

	Ck::Main::ExitCode exitCode = Ck::Main::InvokeMain(application);

	return static_cast<int>(exitCode);
}