#include <cstdlib>

#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Main/main.hpp>
#include <Cocktail/Main/Unix/UnixApplication.hpp>

#include "../../../../../Core/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemDriver.hpp"
#include "Cocktail/Core/IO/Input/Stream/FileInputStream.hpp"
#include "Cocktail/Core/System/FileSystem/Local/LocalFileSystemService.hpp"
#include "Cocktail/Core/Utility/FileUtils.hpp"

namespace Ck::Main::Unix
{
    UnixApplication::UnixApplication(int argc, char** argv)
    {
        mArgv.Reserve(argc - 1);
        for (int i = 1; i < argc; i++)
            mArgv.Emplace(CK_ANSI_TO_TEXT(argv[i]));
    }

    void UnixApplication::Exit(unsigned int exitCode, bool force, StringView callSite)
    {
		CK_LOG(MainLogCategory, LogLevel::Info, CK_TEXT("Requested %s exit with code %u from %s"), force ? CK_TEXT("forced") : CK_TEXT("soft"), exitCode, callSite);

        exit(static_cast<int>(exitCode));
    }

    const Array<String>& UnixApplication::GetArgv() const
    {
        return mArgv;
    }

    Optional<String> UnixApplication::GetEnvironmentVariable(StringView name)
    {
        char* variable = secure_getenv(CK_TEXT_TO_ANSI(name.GetData()));
        if (!variable)
            return Optional<String>::Empty();

        return Optional<String>::Of(InPlace, CK_ANSI_TO_TEXT(variable));
    }

    bool UnixApplication::IsDebuggerPresent() const
    {
        FILE* file = fopen("/proc/self/status", "r");
        if (!file)
            return false;

        char line[256];
        while (fgets(line, sizeof(line), file))
        {
            if (strncmp(line, "TracerPid:", 10) == 0)
            {
                int tracer = atoi(line + 10);
                fclose(file);

                return tracer != 0;
            }
        }
        fclose(file);

        return false;
    }
}
