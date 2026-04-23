#ifndef COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP
#define COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP

#include <Windows.h>

#ifdef GetCommandLine
#undef GetCommandLine
#endif

#ifdef GetEnvironmentVariable
#undef GetEnvironmentVariable
#endif

#include <Cocktail/Core/Application/Application.hpp>

namespace Ck::Main::Win32
{
    /**
     * \brief Win32-specific implementation of the Application class
     *
     * This class provides an implementation of Application using the Win32 API.
     * It handles platform-specific features such as command-line parsing,
     * environment variables, debugger detection, and process termination.
     */
    class Win32Application : public Application
    {
    public:

        /**
         * \brief Construct a Win32Application instance
         *
         * Initializes the application using parameters provided by the Win32 entry point.
         *
         * \param hInstance Handle to the current instance of the application
         * \param hPrevInstance Handle to the previous instance (unused in modern Win32 applications)
         * \param pCmdLine Pointer to the command-line string
         * \param nCmdShow Controls how the window is to be shown
         */
        Win32Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

        /**
         * \brief Request application exit
         *
         * Terminates the application using Win32-specific mechanisms.
         *
         * \param exitCode Exit code returned to the operating system
         * \param force If true, forces immediate termination
         * \param callSite Optional string describing where the exit was requested
         */
        void Exit(unsigned exitCode, bool force, StringView callSite) override;

        /**
         * \brief Get the command-line arguments
         *
         * \return Array of argument strings
         */
        const Array<String>& GetArgv() const override;

        /**
         * \brief Retrieve an environment variable
         *
         * \param name Name of the environment variable
         *
         * \return The value if found, otherwise an empty Optional
         */
        Optional<String> GetEnvironmentVariable(StringView name) override;

        /**
         * \brief Check if a debugger is attached to the process
         *
         * \return True if a debugger is present, false otherwise
         */
        bool IsDebuggerPresent() const override;

        /**
         * \brief Get the path of the executable
         *
         * \return Filesystem path to the running executable
         */
        Path GetExecutablePath() const override;

        /**
         * \brief Get the full command line string
         *
         * \return Command line used to launch the application
         */
        String GetCommandLine() const override;

    private:

        HINSTANCE mInstanceHandle; /*!< Handle to the application instance */
        Array<String> mArgv; /*!< Parsed command-line arguments */
    };
}

#endif // COCKTAIL_MAIN_WIN32_WIN32APPLICATION_HPP
