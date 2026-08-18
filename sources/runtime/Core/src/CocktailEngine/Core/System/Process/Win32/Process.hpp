#ifndef COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PROCESS_HPP
#define COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PROCESS_HPP

#include <CocktailEngine/Core/System/Process/Process.hpp>
#include <CocktailEngine/Core/System/Process/Win32/Pipe.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    class Process : public Ck::Process
    {
    public:

        explicit Process(const String& commandLine, ProcessIOStream streamMode, const HashMap<String, String>& environment, const Path& workingDirectory);

        ~Process() override;

        void Wait() override;
        bool WaitFor(const Duration& timeout) override;
        bool WaitUnit(const Instant& deadline) override;
        void Kill(unsigned exitCode) override;
        Reader<>& GetOutputStream() const override;
        Reader<>& GetErrorStream() const override;
        Writer<>& GetInputStream() const override;
        Optional<int> GetExitCode() const override;
        const String& GetCommandLine() const override;

    private:

        String mCommandLine;
        PROCESS_INFORMATION mProcessInfo;
        UniquePtr<Pipe> mStreamPipe;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_PROCESS_WIN32_PROCESS_HPP
