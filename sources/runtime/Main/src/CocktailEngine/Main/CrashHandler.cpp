#include <exception>

#include <fcntl.h>

#include <CocktailEngine/Core/String.hpp>

#include <CocktailEngine/Main/CrashHandler.hpp>

#ifdef COCKTAIL_OS_WINDOWS
#include <io.h>
#include <process.h>
#else
#include <unistd.h>
#endif

namespace Ck::Main
{
    namespace Detail
    {
        namespace
        {
            CrashContext sContext{};

            std::terminate_handler sPreviousTerminateHandler = nullptr;

            /**
             * \brief What runs when the C++ runtime gives up
             *
             * An exception escaping a noexcept function, one thrown while another is being
             * unwound, a rethrow with nothing to rethrow. Reached with the stack still
             * standing, which is why it is worth reporting separately from a signal: the
             * backtrace here names the throw site.
             *
             * The one trap that needs no platform code at all.
             */
            [[noreturn]] void OnTerminate()
            {
                static constexpr int StdErrDescriptor = 2;

                const int file = OpenCrashReportFile();
                const CrashReport report(StdErrDescriptor, file);

                WriteCrashReportHeader(report, "the C++ runtime called terminate");

                // Unlike a signal handler, this one may allocate and may throw, so the
                // pending exception can be named. Worth the risk of a second failure: what
                // was being thrown is usually the whole answer.
                if (std::exception_ptr pending = std::current_exception())
                {
                    try
                    {
                        std::rethrow_exception(pending);
                    }
                    catch (const std::exception& exception)
                    {
                        report.Write("  pending exception : ");
                        report.WriteLine(exception.what());
                    }
                    catch (...)
                    {
                        report.WriteLine("  pending exception : of a type that does not derive from std::exception");
                    }
                }
                else
                {
                    report.WriteLine("  pending exception : none, terminate was called outright");
                }

                WriteCrashReportBacktrace(report);
                WriteCrashReportFooter(report);

                CloseCrashReportFile(file);

                // Not the previous handler and not a rethrow: a terminate handler must not
                // return, and the application already knows how to end a process that
                // cannot unwind.
                if (sContext.Owner)
                    sContext.Owner->Abort(3);

                std::abort();
            }
        }

        CrashContext& GetCrashContext()
        {
            return sContext;
        }

        void PrepareCrashContext(Application* application)
        {
            sContext.Owner = application;
            sContext.ReportPath[0] = '\0';

            // Beside the executable, named after it and after the process, so that two runs
            // crashing do not overwrite one another and so that the file is found without
            // being looked for. An installation the process cannot write to leaves the
            // report on the standard error alone, which is why this failing is not fatal.
            try
            {
                const Path executable = application->GetExecutablePath();
                const AsciiString base = AsciiString::Convert(executable.ToFormat(Path::Format::Generic).ToString());

                const AnsiChar* data = base.GetData();

                Uint64 length = 0;
                while (data[length] && length < CrashContext::PathCapacity - 1)
                {
                    sContext.ReportPath[length] = data[length];
                    length++;
                }

                // Path::ToString ends on a separator, so appending to it straight away would
                // name a file inside a directory that is really the executable. Dropped here
                // rather than worked around at every use.
                while (length && (sContext.ReportPath[length - 1] == '/' || sContext.ReportPath[length - 1] == '\\'))
                    length--;

                static const AnsiChar Suffix[] = "-crash-";
                for (Uint64 index = 0; Suffix[index] && length < CrashContext::PathCapacity - 1; index++)
                    sContext.ReportPath[length++] = Suffix[index];

#ifdef COCKTAIL_OS_WINDOWS
                Uint64 identifier = static_cast<Uint64>(_getpid());
#else
                Uint64 identifier = static_cast<Uint64>(getpid());
#endif

                AnsiChar digits[20];
                Uint64 digitCount = 0;
                do
                {
                    digits[digitCount++] = static_cast<AnsiChar>('0' + (identifier % 10));
                    identifier /= 10;
                } while (identifier);

                while (digitCount && length < CrashContext::PathCapacity - 1)
                    sContext.ReportPath[length++] = digits[--digitCount];

                static const AnsiChar Extension[] = ".crash.log";
                for (Uint64 index = 0; Extension[index] && length < CrashContext::PathCapacity - 1; index++)
                    sContext.ReportPath[length++] = Extension[index];

                sContext.ReportPath[length] = '\0';
            }
            catch (...)
            {
                // No path, no file. The standard error still gets the report.
                sContext.ReportPath[0] = '\0';
            }
        }

        int OpenCrashReportFile()
        {
            if (!sContext.ReportPath[0])
                return CrashReport::NoDescriptor;

#ifdef COCKTAIL_OS_WINDOWS
            int descriptor = CrashReport::NoDescriptor;
            if (_sopen_s(&descriptor, sContext.ReportPath, _O_WRONLY | _O_CREAT | _O_TRUNC, _SH_DENYNO, _S_IREAD | _S_IWRITE) != 0)
                return CrashReport::NoDescriptor;

            return descriptor;
#else
            // O_CLOEXEC so that a child forked from the handler does not inherit it. Mode
            // 0644: a crash report is not a secret, but it is not for editing either.
            const int descriptor = open(sContext.ReportPath, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);

            return descriptor < 0 ? CrashReport::NoDescriptor : descriptor;
#endif
        }

        void CloseCrashReportFile(int descriptor)
        {
            if (descriptor == CrashReport::NoDescriptor)
                return;

#ifdef COCKTAIL_OS_WINDOWS
            _close(descriptor);
#else
            close(descriptor);
#endif
        }

        void WriteCrashReportHeader(const CrashReport& report, const AnsiChar* reason)
        {
            report.EndLine();
            report.WriteLine("======================= Cocktail Engine crash report =======================");
            report.Write("  caught            : ");
            report.WriteLine(reason);

            WriteCrashReportIdentity(report);
        }

        void WriteCrashReportFooter(const CrashReport& report)
        {
            if (sContext.ReportPath[0])
            {
                report.Write("  written to        : ");
                report.WriteLine(sContext.ReportPath);
            }

            report.WriteLine("===========================================================================");
            report.EndLine();
        }

        void InstallPortableTraps()
        {
            sPreviousTerminateHandler = std::set_terminate(&OnTerminate);
        }

        void UninstallPortableTraps()
        {
            std::set_terminate(sPreviousTerminateHandler);
            sPreviousTerminateHandler = nullptr;
        }
    }

    void InstallCrashHandler(Application* application)
    {
        Detail::CrashContext& context = Detail::GetCrashContext();
        if (context.Installed)
            return;

        Detail::PrepareCrashContext(application);

        // The traps outlive the application on purpose: a fault while the engine is being
        // torn down is one of the kinds most worth a report, destruction order being what it
        // is in a graphic engine. What must not outlive it is the pointer the terminate
        // handler would call Abort through, so it is dropped when the application says it is
        // going away, which is before it destroys anything.
        application->Connect(application->OnTerminate(), [](Application*) {
            Detail::GetCrashContext().Owner = nullptr;
        });

        Detail::InstallPortableTraps();
        Detail::InstallPlatformTraps();

        context.Installed = true;
    }

    void UninstallCrashHandler()
    {
        Detail::CrashContext& context = Detail::GetCrashContext();
        if (!context.Installed)
            return;

        Detail::UninstallPlatformTraps();
        Detail::UninstallPortableTraps();

        context.Owner = nullptr;
        context.Installed = false;
    }
}
