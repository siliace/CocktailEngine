#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/Utility/Finally.hpp>

#include <CocktailEngine/Main/Unix/UnixApplication.hpp>

namespace Ck::Main::Unix
{
    UnixApplication::UnixApplication(int argc, char** argv)
    {
        mArgv.Reserve(argc);
        for (int i = 0; i < argc; i++)
            mArgv.Emplace(reinterpret_cast<const Utf8Char*>(argv[i]));
    }

    void UnixApplication::Abort(unsigned int exitCode)
    {
        // _exit and not exit: exit runs the static destructors and the atexit
        // handlers, which is precisely what a process that can no longer unwind must
        // not do. It would re-enter an X display already closed, or block on a mutex
        // held by the thread that just died. This is what TerminateProcess does on
        // the other platform, so the two agree on what forcing an exit means.
        _exit(static_cast<int>(exitCode));
    }

    const Array<String>& UnixApplication::GetArgv() const
    {
        return mArgv;
    }

    Optional<String> UnixApplication::GetEnvironmentVariable(StringView name)
    {
        Utf8Char* variable = reinterpret_cast<Utf8Char*>(secure_getenv(reinterpret_cast<const AnsiChar*>(name.GetData())));
        if (!variable)
            return Optional<String>::Empty();

        return Optional<String>::Of(InPlace, variable);
    }

    bool UnixApplication::IsDebuggerPresent() const
    {
        static const ssize_t BufferSize = 4096;

        int fd = ::open("/proc/self/status", O_RDONLY);
        if (fd == -1)
            return false;

        Finally finally([&] {
            close(fd);
        });

        char buffer[BufferSize];
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0)
            return false;

        buffer[bytesRead] = '\0';

        const char* line = buffer;
        while (line < buffer + bytesRead)
        {
            if (std::strncmp(line, "TracerPid:", 10) == 0)
                return std::atoi(line + 10) != 0;

            const char* newline = std::strchr(line, '\n');
            if (!newline)
                break;

            line = newline + 1;
        }

        return false;
    }

    Path UnixApplication::GetExecutablePath() const
    {
        static const ssize_t BufferSize = 4096;

        Utf8Char buffer[BufferSize];
        ssize_t bufferLength = readlink("/proc/self/exe", reinterpret_cast<char*>(buffer), BufferSize);
        if (bufferLength == -1)
            throw SystemError::GetLastError();

        return Path::Parse(buffer, bufferLength);
    }

    String UnixApplication::GetCommandLine() const
    {
        static const ssize_t BufferSize = 4096;

        int file = ::open("/proc/self/cmdline", O_RDONLY);
        if (file == -1)
            throw SystemError::GetLastError();

        Finally finally([&] {
            close(file);
        });

        Utf8Char buffer[BufferSize];
        ssize_t n = read(file, buffer, BufferSize);
        if (n == -1)
            throw SystemError::GetLastError();

        return String(buffer, n);
    }
}
