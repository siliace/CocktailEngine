#include <cstdlib>

#include <Cocktail/Main/Unix/UnixApplication.hpp>

namespace Ck::Main::Unix
{
    UnixApplication::UnixApplication(int argc, char** argv)
    {
        mArgv.Reserve(argc - 1);
        for (int i = 1; i < argc; i++)
            mArgv.Emplace(argv[i]);
    }

    void UnixApplication::Exit(unsigned exitCode, bool force, std::string_view callSite)
    {
        CK_LOG(MainLogCategory, LogLevel::Info, "Requested {} exit with code {} from {}", force ? "forced" : "soft", exitCode, callSite.empty() ? "<>" : callSite);

        _exit(exitCode);
    }

    const Array<std::string>& UnixApplication::GetArgv() const
    {
        return mArgv;
    }

    std::string UnixApplication::GetEnvironmentVariable(std::string_view name)
    {
        char* variable = secure_getenv(name.data());
        if (!variable)
            return "";

        return std::string(variable, std::strlen(variable));
    }

    bool UnixApplication::IsDebuggerPresent() const
    {
        return false;
    }
}
