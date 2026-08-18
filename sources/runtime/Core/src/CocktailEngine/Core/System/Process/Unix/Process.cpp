#include <CocktailEngine/Core/System/Process/Unix/Process.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

namespace Ck::Detail::Unix
{
    Process::Process(const String& commandLine, ProcessIOStream streamMode, const HashMap<String, String>& environment, const Path& workingDirectory)
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    Process::~Process()
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    void Process::Wait()
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    bool Process::WaitFor(const Duration& timeout)
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    bool Process::WaitUnit(const Instant& deadline)
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    void Process::Kill(unsigned exitCode)
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    Reader<>& Process::GetOutputStream() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    Reader<>& Process::GetErrorStream() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    Writer<>& Process::GetInputStream() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    Optional<int> Process::GetExitCode() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    const String& Process::GetCommandLine() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }
}
