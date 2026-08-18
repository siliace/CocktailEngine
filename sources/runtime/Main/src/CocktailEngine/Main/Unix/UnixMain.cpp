#include <CocktailEngine/Main/Unix/UnixApplication.hpp>
#include <CocktailEngine/Main/main.hpp>

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    Ck::UniquePtr<Ck::Application> application = Ck::MakeUnique<Ck::Main::Unix::UnixApplication>(argc, argv);

    return Ck::Main::InvokeMain(Move(application));
}
