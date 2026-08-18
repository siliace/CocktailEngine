#include <csignal>
#include <execinfo.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <CocktailEngine/Core/StaticArray.hpp>

#include <CocktailEngine/Main/CrashHandler.hpp>

namespace Ck::Main::Detail
{
    namespace
    {
        /**œ
         * \brief The signals a crash arrives as
         *
         * SIGABRT is in the list because assert and std::abort come through it, and a
         * failed assertion deserves the same trace as a fault. SIGINT and SIGTERM are
         * deliberately absent: asking a program to stop is not a crash, and routing them
         * through Application::Exit is a different job from this one.
         */
        struct TrappedSignal
        {
            int Number;
            const AnsiChar* Description;
        };

        constexpr auto TrappedSignals = MakeStaticArray<TrappedSignal>(
            TrappedSignal{ SIGSEGV, "SIGSEGV, an invalid memory access" },
            TrappedSignal{ SIGBUS, "SIGBUS, a misaligned or unbacked memory access" },
            TrappedSignal{ SIGFPE, "SIGFPE, an arithmetic fault" },
            TrappedSignal{ SIGILL, "SIGILL, an illegal instruction" },
            TrappedSignal{ SIGABRT, "SIGABRT, the process aborted itself" },
            TrappedSignal{ SIGSYS, "SIGSYS, a bad system call" }
        );

        StaticArray<struct sigaction, TrappedSignals.GetSize()> sPreviousActions;

        /**
         * \brief The stack the handlers run on
         *
         * A stack overflow is delivered as SIGSEGV, and delivering it on the stack that
         * just overflowed means the handler faults on entry and the process dies with
         * nothing written. An alternate stack is the only way to report the one crash a
         * report is most needed for.
         *
         * Static storage rather than a heap block: a heap corruption bug is another thing
         * a report is wanted for, and this must not depend on the allocator being sane.
         *
         * Sized by a literal and not by SIGSTKSZ, which stopped being a compile time
         * constant in glibc 2.34 and cannot size an array any more. Generous enough for a
         * handler whose deepest call is into the unwinder.
         */
        alignas(16) Byte sAlternateStack[256 * 1024];

        bool sAlternateStackInstalled = false;

        const AnsiChar* DescribeSignal(int number)
        {
            for (const TrappedSignal& trapped : TrappedSignals)
            {
                if (trapped.Number == number)
                    return trapped.Description;
            }

            return "an untrapped signal";
        }

        /**
         * \brief What runs when a signal arrives
         *
         * Everything called from here is async-signal-safe, which rules out the log
         * manager, anything that allocates and anything that takes a lock the crashing
         * thread may already hold.
         *
         * It does not exit. Once the report is written the default disposition is put back
         * and the signal is raised again, so the process dies of what actually killed it: a
         * core dump is produced if the limits allow one, and a parent waiting on the
         * process reads "killed by SIGSEGV" rather than a made up exit code. Calling _exit
         * here would throw both away.
         */
        void OnSignal(int number, siginfo_t* information, void*)
        {
            // Reentrancy: a fault inside the handler would come back here and write a
            // second report over the first. The second one is let through to the default
            // disposition instead, which is what ends the process.
            static volatile sig_atomic_t reporting = 0;
            if (reporting)
            {
                signal(number, SIG_DFL);
                raise(number);

                return;
            }
            reporting = 1;

            const int file = OpenCrashReportFile();
            const CrashReport report(STDERR_FILENO, file);

            WriteCrashReportHeader(report, DescribeSignal(number));

            if (information)
            {
                // Only a signal the kernel raised from a fault has one. For a signal that was
                // sent, the same union member holds the sender's pid and uid, and printing it
                // as an address is how a report sends someone looking for a bug at 0x3e8.
                if (information->si_code > 0)
                {
                    report.Write("  faulting address  : ");
                    report.WritePointer(information->si_addr);
                    report.EndLine();
                }
                else
                {
                    report.WriteLine("  faulting address  : none, the signal was sent rather than raised by a fault");
                }

                // Signed, and negative for a signal that was sent rather than raised by a
                // fault: -6 is SI_TKILL, which is what a raise looks like. Worth telling
                // apart, since a sent signal carries no faulting address and what looks like
                // one in that case is the sender's identity read through a union.
                report.Write("  signal code       : ");
                report.WriteSigned(static_cast<Int64>(information->si_code));
                report.EndLine();
            }

            WriteCrashReportBacktrace(report);
            WriteCrashReportFooter(report);

            CloseCrashReportFile(file);

            // Die of the original cause. The default disposition and not whatever was
            // installed before: a previous handler could be SIG_IGN, and raising an ignored
            // signal would return here and fault again, forever.
            signal(number, SIG_DFL);
            raise(number);
        }
    }

    void WriteCrashReportIdentity(const CrashReport& report)
    {
        report.Write("  process           : ");
        report.WriteUnsigned(static_cast<Uint64>(getpid()));
        report.EndLine();

        // The kernel thread id and not pthread_self: it is what a debugger and the contents
        // of /proc agree on.
        report.Write("  thread            : ");
        report.WriteUnsigned(static_cast<Uint64>(syscall(SYS_gettid)));
        report.EndLine();
    }

    void WriteCrashReportBacktrace(const CrashReport& report)
    {
        // Fixed storage, filled by backtrace, which writes into a buffer the caller owns
        // and allocates nothing itself once it has been called at least once. That first
        // call happens at install time, for exactly this reason.
        static void* frames[128];

        const int count = backtrace(frames, static_cast<int>(sizeof(frames) / sizeof(frames[0])));

        report.Write("  stack             : ");
        report.WriteUnsigned(static_cast<Uint64>(count < 0 ? 0 : count));
        report.WriteLine(" frames");

        if (count <= 0)
        {
            report.WriteLine("    the stack could not be walked");
            return;
        }

        // backtrace_symbols would return an array it allocated, which a signal handler
        // cannot ask for. The _fd form writes the same lines straight to a descriptor and
        // documents that it does not allocate, at the price of writing to one descriptor at
        // a time and of ignoring the indentation of the rest of the report.
        if (report.GetConsoleDescriptor() != CrashReport::NoDescriptor)
            backtrace_symbols_fd(frames, count, report.GetConsoleDescriptor());

        if (report.GetFileDescriptor() != CrashReport::NoDescriptor)
            backtrace_symbols_fd(frames, count, report.GetFileDescriptor());

        report.WriteLine("  ---------------------------------------------------------------------------");
        report.WriteLine("  Symbol names come from the dynamic symbol table, so a static function shows");
        report.WriteLine("  as an offset from the nearest exported one. Resolve a frame with:");
        report.WriteLine("    addr2line -Cfie <binary> <address>");
    }

    void InstallPlatformTraps()
    {
        // Called once, here, where allocating is still allowed: the first call to backtrace
        // resolves the unwinder through the dynamic loader and may allocate, and doing that
        // from a handler is what makes an otherwise correct crash handler hang.
        void* warmup[4];
        backtrace(warmup, 4);

        stack_t alternate{};
        alternate.ss_sp = sAlternateStack;
        alternate.ss_size = sizeof(sAlternateStack);
        alternate.ss_flags = 0;

        sAlternateStackInstalled = sigaltstack(&alternate, nullptr) == 0;

        struct sigaction action{};
        action.sa_sigaction = &OnSignal;
        sigemptyset(&action.sa_mask);

        // SA_SIGINFO for the faulting address, SA_ONSTACK to land on the alternate stack,
        // SA_NODEFER so that a fault inside the handler is delivered rather than held: the
        // guard above turns it into a plain default-disposition death.
        action.sa_flags = SA_SIGINFO | SA_NODEFER;
        if (sAlternateStackInstalled)
            action.sa_flags |= SA_ONSTACK;

        for (unsigned int index = 0; index < TrappedSignals.GetSize(); index++)
            sigaction(TrappedSignals[index].Number, &action, &sPreviousActions[index]);
    }

    void UninstallPlatformTraps()
    {
        for (unsigned int index = 0; index < TrappedSignals.GetSize(); index++)
            sigaction(TrappedSignals[index].Number, &sPreviousActions[index], nullptr);

        if (sAlternateStackInstalled)
        {
            stack_t disable{};
            disable.ss_flags = SS_DISABLE;
            sigaltstack(&disable, nullptr);

            sAlternateStackInstalled = false;
        }
    }
}
