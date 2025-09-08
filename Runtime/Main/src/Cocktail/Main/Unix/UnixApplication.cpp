#include <cstdlib>

#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Main/main.hpp>
#include <Cocktail/Main/Unix/UnixApplication.hpp>

namespace Ck::Main::Unix
{
    UnixApplication::UnixApplication(int argc, char** argv)
    {
        mArgv.Reserve(argc - 1);
        for (int i = 1; i < argc; i++)
            mArgv.Emplace(argv[i]);
    }

    void UnixApplication::Exit(unsigned exitCode, bool force, StringView callSite)
    {
        CK_LOG(MainLogCategory, LogLevel::Info, "Requested {} exit with code {} from {}", force ? "forced" : "soft", exitCode, callSite.IsEmpty() ? "<>" :
            std::string_view(callSite.GetData())
        );

        exit(static_cast<int>(exitCode));
    }

    const Array<String>& UnixApplication::GetArgv() const
    {
        return mArgv;
    }

    Optional<String> UnixApplication::GetEnvironmentVariable(StringView name)
    {
        char* variable = secure_getenv(name.GetData());
        if (!variable)
            return Optional<String>::Empty();

        return Optional<String>::Of(InPlace, variable);
    }

    bool UnixApplication::IsDebuggerPresent() const
    {
        return false;
    }
}
