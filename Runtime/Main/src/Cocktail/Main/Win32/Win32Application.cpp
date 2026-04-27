#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/System/SystemError.hpp>

#include <Cocktail/Main/Win32/DebuggerLogChannel.hpp>
#include <Cocktail/Main/Win32/Win32Application.hpp>
#include <Cocktail/Main/main.hpp>

namespace Ck::Main
{
#ifndef NDEBUG
    class DebugLogServiceProvider : public ServiceProvider
    {
    public:

        explicit DebugLogServiceProvider(Application* application) :
            ServiceProvider(application)
        {
            /// Nothing
        }

    protected:

        void DoBoot(Application* application) override
        {
            application->Invoke([](LogManager* logManager) {
                logManager->RegisterChannel(CK_TEXT("debug"), MakeUnique<DebuggerLogChannel>());
            });
        }
    };

    COCKTAIL_REGISTER_SERVICE_PROVIDER(DebugLogServiceProvider);
#endif

    namespace Win32
    {
        Win32Application::Win32Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) :
            mInstanceHandle(hInstance)
        {
            int argc = 0;
            PWSTR* arguments = CommandLineToArgvW(pCmdLine, &argc);

            mArgv.Reserve(argc);
            for (int i = 0; i < argc; i++)
                mArgv.Emplace(arguments[i]);

            LocalFree(arguments);
        }

        void Win32Application::Exit(unsigned int exitCode, bool force, StringView callSite)
        {
            CK_LOG(MainLogCategory, LogLevel::Info, CK_TEXT("Requested %s exit with code %d from %s"), force ? CK_TEXT("forced") : CK_TEXT("soft"), exitCode, callSite);

            if (force)
            {
                TerminateProcess(GetCurrentProcess(), exitCode);
            }
            else
            {
                Terminate();

                PostQuitMessage(exitCode);
            }
        }

        const Array<String>& Win32Application::GetArgv() const
        {
            return mArgv;
        }

        Optional<String> Win32Application::GetEnvironmentVariable(StringView name)
        {
            DWORD variableLength = ::GetEnvironmentVariableW(name.GetData(), nullptr, 0);
            if (!variableLength)
                return Optional<String>::Empty();

            TextChar* variable = COCKTAIL_STACK_ALLOC(TextChar, variableLength);
            GetEnvironmentVariableW(name.GetData(), variable, variableLength);

            String value;
            value.Append(variable, variableLength);

            return Optional<String>::Of(value);
        }

        bool Win32Application::IsDebuggerPresent() const
        {
            return ::IsDebuggerPresent() == TRUE;
        }

        Path Win32Application::GetExecutablePath() const
        {
            TextChar executablePath[MAX_PATH];
            DWORD executablePathLength = GetModuleFileName(mInstanceHandle, executablePath, MAX_PATH);
            if (executablePathLength == 0 || executablePathLength > MAX_PATH)
                throw SystemError::GetLastError();

            return Path::Parse(executablePath, executablePathLength);
        }

        String Win32Application::GetCommandLine() const
        {
            return ::GetCommandLineW();
        }
    }
}
