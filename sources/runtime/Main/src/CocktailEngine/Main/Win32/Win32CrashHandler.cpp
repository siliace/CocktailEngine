#include <cstdlib>

#include <Windows.h>

#include <DbgHelp.h>

#include <CocktailEngine/Main/CrashHandler.hpp>

namespace Ck::Main::Detail
{
    namespace
    {
        LPTOP_LEVEL_EXCEPTION_FILTER sPreviousFilter = nullptr;

        /**
         * \brief The context of the exception being reported
         *
         * A stack walk needs the register state the exception was raised with, and the
         * signature of WriteCrashReportBacktrace does not carry it: on Unix the unwinder
         * reads the current stack and needs nothing passed. Left here for the walker to
         * pick up, which is sound because the filter reports one exception at a time.
         */
        CONTEXT* sExceptionContext = nullptr;

        const AnsiChar* DescribeException(DWORD code)
        {
            switch (code)
            {
                case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION, an invalid memory access";
                case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW, the stack ran out";
                case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION, an illegal instruction";
                case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION, a privileged instruction";
                case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT, a misaligned access";
                case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR, a page could not be brought in";
                case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO, an integer division by zero";
                case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW, an integer overflow";
                case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO, a floating point division by zero";
                case EXCEPTION_FLT_INVALID_OPERATION: return "EXCEPTION_FLT_INVALID_OPERATION, an invalid floating point operation";
                case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION, an exception that cannot be resumed";
                case EXCEPTION_INVALID_HANDLE: return "EXCEPTION_INVALID_HANDLE, an invalid handle was used";
                default: return "a structured exception";
            }
        }

        /**
         * \brief Write a minidump beside the report
         *
         * The counterpart of the core dump the Unix side gets for free by dying of its own
         * signal. Nothing here can produce one implicitly, so it is asked for.
         */
        void WriteMiniDump(const CrashReport& report, EXCEPTION_POINTERS* pointers)
        {
            const CrashContext& context = GetCrashContext();
            if (!context.ReportPath[0])
                return;

            // The report path with the extension swapped, so a report and its dump are one
            // name apart.
            AnsiChar dumpPath[CrashContext::PathCapacity];
            Uint64 length = 0;
            while (context.ReportPath[length] && length < CrashContext::PathCapacity - 5)
            {
                dumpPath[length] = context.ReportPath[length];
                length++;
            }

            static const AnsiChar Extension[] = ".dmp";
            for (Uint64 index = 0; Extension[index] && length < CrashContext::PathCapacity - 1; index++)
                dumpPath[length++] = Extension[index];

            dumpPath[length] = '\0';

            const HANDLE file = CreateFileA(dumpPath, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (file == INVALID_HANDLE_VALUE)
                return;

            MINIDUMP_EXCEPTION_INFORMATION information{};
            information.ThreadId = GetCurrentThreadId();
            information.ExceptionPointers = pointers;
            information.ClientPointers = FALSE;

            // WithIndirectlyReferencedMemory keeps what the registers point at, which is the
            // difference between a dump naming the crash and a dump explaining it.
            const MINIDUMP_TYPE type = static_cast<MINIDUMP_TYPE>(MiniDumpWithIndirectlyReferencedMemory | MiniDumpWithDataSegs | MiniDumpWithThreadInfo | MiniDumpWithHandleData);

            const BOOL written = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, type, &information, nullptr, nullptr);

            CloseHandle(file);

            if (written)
            {
                report.Write("  minidump          : ");
                report.WriteLine(dumpPath);
            }
        }

        /**
         * \brief What runs when nothing in the program handled a structured exception
         *
         * Unlike a Unix signal handler this runs on a thread that is merely executing code,
         * so allocating and locking are allowed. The report is still written the restricted
         * way, so that a report reads the same whichever platform produced it.
         */
        LONG WINAPI OnUnhandledException(EXCEPTION_POINTERS* pointers)
        {
            static volatile LONG reporting = 0;
            if (InterlockedExchange(&reporting, 1) != 0)
                return EXCEPTION_CONTINUE_SEARCH;

            const int file = OpenCrashReportFile();
            const CrashReport report(2, file);

            const DWORD code = pointers && pointers->ExceptionRecord ? pointers->ExceptionRecord->ExceptionCode : 0;

            WriteCrashReportHeader(report, DescribeException(code));

            report.Write("  exception code    : ");
            report.WriteHex(static_cast<Uint64>(code));
            report.EndLine();

            if (pointers && pointers->ExceptionRecord)
            {
                report.Write("  faulting address  : ");
                report.WritePointer(pointers->ExceptionRecord->ExceptionAddress);
                report.EndLine();

                // An access violation says what it was doing and where, which the code alone
                // does not.
                if (code == EXCEPTION_ACCESS_VIOLATION && pointers->ExceptionRecord->NumberParameters >= 2)
                {
                    const ULONG_PTR operation = pointers->ExceptionRecord->ExceptionInformation[0];

                    report.Write("  access            : ");
                    report.Write(operation == 0 ? "read of " : operation == 1 ? "write to " : "execution of ");
                    report.WriteHex(static_cast<Uint64>(pointers->ExceptionRecord->ExceptionInformation[1]));
                    report.EndLine();
                }
            }

            sExceptionContext = pointers ? pointers->ContextRecord : nullptr;
            WriteCrashReportBacktrace(report);
            sExceptionContext = nullptr;

            WriteMiniDump(report, pointers);
            WriteCrashReportFooter(report);

            CloseCrashReportFile(file);

            // Handed back rather than swallowed, so that Windows Error Reporting and a
            // debugger still see it. This is the counterpart of re-raising the signal with
            // the default disposition on Unix: the process dies of what killed it.
            return EXCEPTION_CONTINUE_SEARCH;
        }
    }

    void WriteCrashReportIdentity(const CrashReport& report)
    {
        report.Write("  process           : ");
        report.WriteUnsigned(static_cast<Uint64>(GetCurrentProcessId()));
        report.EndLine();

        report.Write("  thread            : ");
        report.WriteUnsigned(static_cast<Uint64>(GetCurrentThreadId()));
        report.EndLine();
    }

    void WriteCrashReportBacktrace(const CrashReport& report)
    {
        const HANDLE process = GetCurrentProcess();

        // Symbols are loaded here and not at install time: a debug build resolves them from
        // pdb files that may be megabytes, and paying for that on every start to serve a
        // crash that may never come is the wrong trade. Allowed because a filter is not a
        // signal handler.
        SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES);
        const BOOL symbols = SymInitialize(process, nullptr, TRUE);

        CONTEXT walked{};
        if (sExceptionContext)
        {
            walked = *sExceptionContext;
        }
        else
        {
            // No exception context, which is the terminate handler case: walk from here.
            RtlCaptureContext(&walked);
        }

        STACKFRAME64 frame{};
#if defined(_M_X64) || defined(__x86_64__)
        const DWORD machine = IMAGE_FILE_MACHINE_AMD64;
        frame.AddrPC.Offset = walked.Rip;
        frame.AddrFrame.Offset = walked.Rbp;
        frame.AddrStack.Offset = walked.Rsp;
#elif defined(_M_ARM64) || defined(__aarch64__)
        const DWORD machine = IMAGE_FILE_MACHINE_ARM64;
        frame.AddrPC.Offset = walked.Pc;
        frame.AddrFrame.Offset = walked.Fp;
        frame.AddrStack.Offset = walked.Sp;
#else
        const DWORD machine = IMAGE_FILE_MACHINE_I386;
        frame.AddrPC.Offset = walked.Eip;
        frame.AddrFrame.Offset = walked.Ebp;
        frame.AddrStack.Offset = walked.Esp;
#endif
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Mode = AddrModeFlat;

        report.WriteLine("  stack             :");

        // A symbol name goes in the same block as the record it belongs to, which is why the
        // buffer is sized rather than the structure declared on its own.
        Byte symbolStorage[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(AnsiChar)]{};
        SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolStorage);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        for (Uint64 depth = 0; depth < 128; depth++)
        {
            if (!StackWalk64(machine, process, GetCurrentThread(), &frame, &walked, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
                break;

            if (!frame.AddrPC.Offset)
                break;

            report.Write("    ");
            report.WriteHex(static_cast<Uint64>(frame.AddrPC.Offset));

            DWORD64 displacement = 0;
            if (symbols && SymFromAddr(process, frame.AddrPC.Offset, &displacement, symbol))
            {
                report.Write("  ");
                report.Write(symbol->Name);
                report.Write(" + ");
                report.WriteUnsigned(static_cast<Uint64>(displacement));

                IMAGEHLP_LINE64 line{};
                line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

                DWORD lineDisplacement = 0;
                if (SymGetLineFromAddr64(process, frame.AddrPC.Offset, &lineDisplacement, &line))
                {
                    report.Write("  ");
                    report.Write(line.FileName);
                    report.Write(":");
                    report.WriteUnsigned(static_cast<Uint64>(line.LineNumber));
                }
            }
            else
            {
                report.Write("  no symbol");
            }

            report.EndLine();
        }

        if (symbols)
            SymCleanup(process);
    }

    void InstallPlatformTraps()
    {
        sPreviousFilter = SetUnhandledExceptionFilter(&OnUnhandledException);
    }

    void UninstallPlatformTraps()
    {
        SetUnhandledExceptionFilter(sPreviousFilter);
        sPreviousFilter = nullptr;
    }
}
