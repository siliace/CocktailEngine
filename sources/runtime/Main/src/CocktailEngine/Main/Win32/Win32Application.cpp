#include <cstdlib>

#include <CocktailEngine/Core/Log/Log.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

#include <CocktailEngine/Main/Win32/DebuggerLogChannel.hpp>
#include <CocktailEngine/Main/Win32/Win32Application.hpp>
#include <CocktailEngine/Main/main.hpp>

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

        void Win32Application::Abort(unsigned int exitCode)
        {
            // No PostQuitMessage counterpart for the soft path any more, and nothing is
            // lost with it: the message loop peeks filtered by its own window handle,
            // which never retrieves a thread message, so the WM_QUIT this used to post
            // could not be read by anyone.
            TerminateProcess(GetCurrentProcess(), exitCode);

            // TerminateProcess only asks, and returns before the process is gone, where
            // the _exit of the other platform never returns at all. Abort promises not
            // to return, so the wait is what makes the promise true here.
            WaitForSingleObject(GetCurrentProcess(), INFINITE);

            // Not reached: the wait above does not complete. Present so that the
            // function cannot fall off its end, which a [[noreturn]] must not do.
            std::abort();
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
