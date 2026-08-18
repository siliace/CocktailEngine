#ifndef COCKTAILENGINE_CORE_SYSTEM_PROCESS_PROCESS_HPP
#define COCKTAILENGINE_CORE_SYSTEM_PROCESS_PROCESS_HPP

#include <CocktailEngine/Core/HashMap.hpp>
#include <CocktailEngine/Core/IO/Input/Reader/Reader.hpp>
#include <CocktailEngine/Core/IO/Output/Writer/Writer.hpp>
#include <CocktailEngine/Core/Memory/UniquePtr.hpp>
#include <CocktailEngine/Core/System/FileSystem/Path.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

namespace Ck
{
    /**
     * \brief Defines how the child process standard streams are handled
     */
    enum class ProcessIOStream
    {
        None, /**< Child process standard streams are not explicitly redirected */
        Pipe, /**< Child process standard streams are exposed through Reader/Writer interfaces */
        Inherit /**< Child process inherits the calling process standard streams */
    };

    /**
     * \brief Represents an operating system process
     */
    class COCKTAILENGINE_CORE_API Process
    {
    public:

        /**
         * \brief Creates a process
         *
         * \param commandLine Command line used to launch the process
         * \param streamMode Specifies how the child process standard streams are handled
         * \param environment Environment variables passed to the child process
         * \param workingDirectory Working directory of the child process
         *
         * \return A unique pointer to the created process
         */
        static UniquePtr<Process> Create(const String& commandLine, ProcessIOStream streamMode = ProcessIOStream::Pipe,
                                         const HashMap<String, String>& environment = HashMap<String, String>::Empty, const Path& workingDirectory = Path::Empty);

        /**
         * \brief Destroys the process object
         */
        virtual ~Process() = default;

        /**
         * \brief Waits for the process to terminate
         */
        virtual void Wait() = 0;

        /**
         * \brief Waits for the process to terminate for a specified duration
         *
         * \param timeout Maximum duration to wait
         *
         * \return True if the process terminated before the timeout expired, false otherwise
         */
        virtual bool WaitFor(const Duration& timeout) = 0;

        /**
         * \brief Waits for the process to terminate until a specified deadline
         * 
         * \param deadline Deadline at which the wait operation stops
         * 
         * \return True if the process terminated before the deadline, false otherwise
         */
        virtual bool WaitUnit(const Instant& deadline) = 0;

        /**
         * \brief Terminates the process
         * 
         * \param exitCode Exit code assigned to the terminated process
         */
        virtual void Kill(unsigned int exitCode) = 0;

        /**
         * \brief Gets the standard output stream of the process
         * 
         * \return Reader used to read the process standard output
         */
        virtual Reader<>& GetOutputStream() const = 0;

        /**
         * \brief Gets the standard error stream of the process
         * 
         * \return Reader used to read the process standard error
         */
        virtual Reader<>& GetErrorStream() const = 0;

        /**
         * \brief Gets the standard input stream of the process
         * 
         * \return Writer used to write to the process standard input
         */
        virtual Writer<>& GetInputStream() const = 0;

        /**
         * \brief Gets the process exit code if the process has terminated
         * 
         * \return The process exit code, or an empty optional if the process is still running
         */
        virtual Optional<int> GetExitCode() const = 0;

        /**
         * \brief Gets the command line used to launch the process
         * 
         * \return The process command line
         */
        virtual const String& GetCommandLine() const = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_PROCESS_PROCESS_HPP
