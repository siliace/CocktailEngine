#include <CocktailEngine/Core/IO/Output/Writer/StringWriter.hpp>
#include <CocktailEngine/Core/System/Process/Win32/Process.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

namespace Ck::Detail::Win32
{
    Process::Process(const String& commandLine, ProcessIOStream streamMode, const HashMap<String, String>& environment, const Path& workingDirectory) :
        mCommandLine(commandLine),
        mProcessInfo()
    {
        StringWriter<> environmentBlockWriter;
        for (const auto& [name, value] : environment)
        {
            assert(!name.IsEmpty());
            environmentBlockWriter.Write(name);
            environmentBlockWriter.Write(CK_CHAR('='));
            if (!value.IsEmpty())
                environmentBlockWriter.Write(value);
            environmentBlockWriter.Write(CK_CHAR('\0'));
        }
        String environmentBlock = environmentBlockWriter.ToString();

        STARTUPINFO startupInfo;
        Memory::Zero(startupInfo);
        startupInfo.cb = sizeof(startupInfo);
        if (streamMode != ProcessIOStream::None)
        {
            startupInfo.dwFlags |= STARTF_USESTDHANDLES;

            if (streamMode == ProcessIOStream::Inherit)
            {
                startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
                startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
                startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
            }
            else
            {
                mStreamPipe = MakeUnique<Pipe>();

                startupInfo.hStdOutput = mStreamPipe->GetReader().GetSystemHandle();
                startupInfo.hStdError = mStreamPipe->GetReader().GetSystemHandle();
                startupInfo.hStdInput = mStreamPipe->GetWriter().GetSystemHandle();
            }
        }

        BOOL result = CreateProcess(nullptr, mCommandLine.GetData(), nullptr, nullptr, streamMode == ProcessIOStream::Inherit, CREATE_UNICODE_ENVIRONMENT,
                                    environmentBlock.GetData(), workingDirectory.ToString().GetData(), &startupInfo, &mProcessInfo);

        if (result == FALSE)
            throw SystemError::GetLastError();
    }

    Process::~Process()
    {
        WaitForSingleObject(mProcessInfo.hProcess, INFINITE);
        CloseHandle(mProcessInfo.hThread);
        CloseHandle(mProcessInfo.hProcess);
    }

    void Process::Wait()
    {
        DWORD result = WaitForSingleObject(mProcessInfo.hProcess, INFINITE);
        if (result == WAIT_FAILED)
            throw SystemError::GetLastError();
    }

    bool Process::WaitFor(const Duration& timeout)
    {
        DWORD timeoutMilliseconds = INFINITE;
        if (!timeout.IsInfinite())
        {
            constexpr Uint64 MaxFiniteTimeout = static_cast<Uint64>(INFINITE) - 1;
            if (timeout >= Duration::Milliseconds(MaxFiniteTimeout))
            {
                timeoutMilliseconds = INFINITE - 1;
            }
            else
            {
                Uint64 milliseconds = timeout.GetCount(TimeUnit::Milliseconds());
                if (Duration::Milliseconds(milliseconds) < timeout)
                    ++milliseconds;

                timeoutMilliseconds = static_cast<DWORD>(milliseconds);
            }
        }

        DWORD result = WaitForSingleObject(mProcessInfo.hProcess, timeoutMilliseconds);
        if (result == WAIT_FAILED)
            throw SystemError::GetLastError();

        return result == WAIT_OBJECT_0;
    }

    bool Process::WaitUnit(const Instant& deadline)
    {
        const Instant now = Instant::Now();
        if (!deadline.IsAfter(now))
            return false;

        return WaitFor(Duration::Between(now, deadline));
    }

    void Process::Kill(unsigned int exitCode)
    {
        if (TerminateProcess(mProcessInfo.hProcess, exitCode) == FALSE)
            throw SystemError::GetLastError();
    }

    Reader<>& Process::GetOutputStream() const
    {
        if (!mStreamPipe)
            ExceptionUtils::ThrowNotImplemented();

        return mStreamPipe->GetReader();
    }

    Reader<>& Process::GetErrorStream() const
    {
        if (!mStreamPipe)
            ExceptionUtils::ThrowNotImplemented();

        return mStreamPipe->GetReader();
    }

    Writer<>& Process::GetInputStream() const
    {
        if (!mStreamPipe)
            ExceptionUtils::ThrowNotImplemented();

        return mStreamPipe->GetWriter();
    }

    Optional<int> Process::GetExitCode() const
    {
        DWORD exitCode;
        if (!GetExitCodeProcess(mProcessInfo.hProcess, &exitCode))
            throw SystemError::GetLastError();

        if (exitCode == STILL_ACTIVE)
            return Optional<int>::Empty();

        return Optional<int>::Of(static_cast<int>(exitCode));
    }

    const String& Process::GetCommandLine() const
    {
        return mCommandLine;
    }
}
