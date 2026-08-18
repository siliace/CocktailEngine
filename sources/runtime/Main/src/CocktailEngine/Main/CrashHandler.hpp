#ifndef COCKTAILENGINE_MAIN_CRASHHANDLER_HPP
#define COCKTAILENGINE_MAIN_CRASHHANDLER_HPP

#include <CocktailEngine/Core/Application/Application.hpp>

#include <CocktailEngine/Main/CrashReport.hpp>

namespace Ck::Main
{
    /**
     * \brief Install the traps that turn a crash into a report
     *
     * What a crash is caught with is not portable, and there is no abstraction worth
     * building over it: Unix delivers a signal, Windows raises a structured exception, and
     * the two differ in what may be done once caught far more than in how they are armed.
     * What is shared is the part that matters to whoever reads the result, the report
     * itself, and the one trap that is the same everywhere, the C++ terminate handler.
     *
     * Call this before the application is booted. A failure while booting is exactly the
     * kind a report is wanted for, and nothing here needs a service: the report path comes
     * from Application::GetExecutablePath, which the platform layer answers on its own.
     *
     * \param application The application the process belongs to
     */
    void InstallCrashHandler(Application* application);

    /**
     * \brief Remove the traps installed by InstallCrashHandler
     *
     * Restores what was there before, so that a debugger or a test harness that installed
     * its own handlers gets them back.
     */
    void UninstallCrashHandler();

    namespace Detail
    {
        /**
         * \brief What a report needs, prepared while allocating is still allowed
         *
         * A handler cannot build a path, resolve a service or format a string. Everything
         * it reads is computed here, at install time, and left in storage of a fixed size
         * that no crash can invalidate.
         */
        struct CrashContext
        {
            /**
             * \brief Room for the report path
             *
             * A fixed capacity rather than a String: whoever reads it may be running on
             * an alternate stack after a stack overflow, with a heap that a heap
             * corruption bug already ruined.
             */
            static constexpr Uint64 PathCapacity = 1024;

            AnsiChar ReportPath[PathCapacity]; /*!< NUL terminated path of the report file, empty when there is none */
            Application* Owner; /*!< Application the process belongs to, for the terminate handler */
            bool Installed; /*!< Whether the traps are currently armed */
        };

        /**
         * \brief Get the process wide crash context
         *
         * \return The context
         */
        CrashContext& GetCrashContext();

        /**
         * \brief Fill the crash context from an application
         *
         * \param application The application the process belongs to
         */
        void PrepareCrashContext(Application* application);

        /**
         * \brief Open the file the report is written to
         *
         * Opened when a crash happens rather than at install time, so that a run without
         * one leaves nothing behind. Safe to call from a handler: the path was built ahead
         * and open is async-signal-safe.
         *
         * \return The descriptor, or CrashReport::NoDescriptor
         */
        int OpenCrashReportFile();

        /**
         * \brief Close a descriptor returned by OpenCrashReportFile
         *
         * \param descriptor The descriptor to close
         */
        void CloseCrashReportFile(int descriptor);

        /**
         * \brief Write the part of a report that does not depend on the platform
         *
         * \param report The report being written
         * \param reason What was caught, as a literal
         */
        void WriteCrashReportHeader(const CrashReport& report, const AnsiChar* reason);

        /**
         * \brief Write the closing part of a report
         *
         * \param report The report being written
         */
        void WriteCrashReportFooter(const CrashReport& report);

        /**
         * \brief Arm the traps that are the same on every platform
         *
         * The C++ terminate handler: an exception escaping a noexcept function, a throw
         * during unwinding, a rethrow with nothing to rethrow.
         */
        void InstallPortableTraps();

        /**
         * \brief Disarm what InstallPortableTraps armed
         */
        void UninstallPortableTraps();

        /**
         * \brief Arm the traps that only this platform has
         *
         * Implemented once per platform, and the only part of a crash handler that has to
         * be.
         */
        void InstallPlatformTraps();

        /**
         * \brief Disarm what InstallPlatformTraps armed
         */
        void UninstallPlatformTraps();

        /**
         * \brief Write the identity of the process and of the running thread
         *
         * Platform specific for want of any portable way of asking.
         *
         * \param report The report being written
         */
        void WriteCrashReportIdentity(const CrashReport& report);

        /**
         * \brief Write the stack of the running thread into a report
         *
         * \param report The report being written
         */
        void WriteCrashReportBacktrace(const CrashReport& report);
    }
}

#endif // COCKTAILENGINE_MAIN_CRASHHANDLER_HPP
