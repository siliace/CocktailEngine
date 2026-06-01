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
#ifndef NDEBUG
    session.configData().verbosity = Catch::Verbosity::High;
#endif

    CK_LOG(Catch2, LogLevel::Info, CK_TEXT("Starting Catch2 test session"));

#ifdef COCKTAIL_OS_WINDOWS
    Array<const TextChar*> arguments = application->GetArgv().Transform([](const String& argument) {
        return argument.GetData();
    });
#else
    Array<const AnsiChar*> arguments = application->GetArgv().Transform([](const String& argument) {
        return reinterpret_cast<const AnsiChar*>(argument.GetData());
    });
#endif

    int parseResult = session.applyCommandLine(static_cast<int>(arguments.GetSize()), arguments.GetData());
    if (parseResult != 0)
    {
        CK_LOG(Catch2, LogLevel::Error, CK_TEXT("Failed to parse application arguments"));
        return ExitCode::GeneralError;
    }

    int result = session.run();
    if (result == 0)
    {
        CK_LOG(Catch2, LogLevel::Info, CK_TEXT("All tests passed"));
    }
    else if (result > 0)
    {
        CK_LOG(Catch2, LogLevel::Warning, CK_TEXT("Tests finished with %d failed test(s)"), result);
    }
    else
    {
        CK_LOG(Catch2, LogLevel::Error, CK_TEXT("Catch2 aborted due to internal error"));
        return ExitCode::GeneralError;
    }

    return ExitCode::Success;
}
