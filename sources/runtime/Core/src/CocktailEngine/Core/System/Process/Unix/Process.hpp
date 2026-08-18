#ifndef COCKTAILENGINE_CORE_SYSTEM_PROCESS_UNIX_PROCESS_HPP
#define COCKTAILENGINE_CORE_SYSTEM_PROCESS_UNIX_PROCESS_HPP

#include <CocktailEngine/Core/System/Process/Process.hpp>

namespace Ck::Detail::Unix
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
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_PROCESS_UNIX_PROCESS_HPP
