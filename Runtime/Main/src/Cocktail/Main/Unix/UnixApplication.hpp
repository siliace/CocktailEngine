#ifndef COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP
#define COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP

#include <Cocktail/Core/Application/Application.hpp>

namespace Ck::Main::Unix
{
    /**
     * \brief Unix-specific implementation of the Application class
     *
     * This class provides an implementation of Application for Unix-like systems
     * (Linux, macOS, BSD). It handles command-line arguments, environment access,
     * debugger detection, and process-related system queries using POSIX APIs.
     */
    class UnixApplication : public Application
    {
    public:

        /**
         * \brief Construct a UnixApplication instance
         *
         * Initializes the application from the standard Unix entry point arguments.
         *
         * \param argc Argument count
         * \param argv Argument vector
         */
	    UnixApplication(int argc, char** argv);

        /**
         * \brief Request application exit
         *
         * Terminates the application using Unix/POSIX mechanisms.
         *
         * \param exitCode Exit code returned to the operating system
         * \param force  If true, forces immediate termination
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
         * \return True if a debugger is detected, false otherwise
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

        Array<String> mArgv; /*!< Parsed command-line arguments */
    };
}

#endif // COCKTAIL_MAIN_UNIX_UNIXAPPLICATION_HPP
