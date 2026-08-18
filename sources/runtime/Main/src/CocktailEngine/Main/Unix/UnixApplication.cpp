#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include <CocktailEngine/Core/System/SystemError.hpp>

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
        FILE* file = fopen("/proc/self/status", "r");
        if (!file)
            return false;

        char line[256];
        while (fgets(line, sizeof(line), file))
        {
            if (std::strncmp(line, "TracerPid:", 10) == 0)
            {
                int tracer = atoi(line + 10);
                fclose(file);

                return tracer != 0;
            }
        }
        fclose(file);

        return false;
    }

    Path UnixApplication::GetExecutablePath() const
    {
        static const ssize_t BufferSize = 1024;

        Utf8Char buffer[BufferSize];
        ssize_t bufferLength = readlink("/proc/self/exe", reinterpret_cast<char*>(buffer), BufferSize);
        if (bufferLength == -1)
            throw SystemError::GetLastError();

        return Path::Parse(buffer, bufferLength);
    }

    String UnixApplication::GetCommandLine() const
    {
        static const ssize_t BufferSize = 4096;

        int fd = ::open("/proc/self/cmdline", O_RDONLY);
        if (fd == -1)
            throw SystemError::GetLastError();

        Utf8Char buffer[BufferSize];
        ssize_t n = read(fd, buffer, BufferSize);
        if (n == -1)
        {
            std::system_error lastError = SystemError::GetLastError();
            close(fd);

            throw lastError;
        }

        close(fd);

        return String(buffer, n);
    }
}
