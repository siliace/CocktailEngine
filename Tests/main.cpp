#include <catch2/catch_session.hpp>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Main/ExitCode.hpp>

using namespace Ck;
using namespace Ck::Main;

COCKTAIL_DECLARE_LOG_CATEGORY(Catch2, LogLevel::Info);
COCKTAIL_DEFINE_LOG_CATEGORY(Catch2);

ExitCode ApplicationMain(Application* application)
{
    Catch::Session session;
    Array<const TextChar*> arguments = application->GetArgv().Transform([](const String& argument) {
        return argument.GetData();
    });

    int result = session.applyCommandLine(arguments.GetSize(), arguments.GetData());
    if (result != 0)
    {
        CK_LOG(Catch2, LogLevel::Error, CK_TEXT("Failed to parse application arguments"));
        return ExitCode::GeneralError;
    }

    int failCount = session.run();

    application->Exit(failCount);
    return ExitCode::Success;
}