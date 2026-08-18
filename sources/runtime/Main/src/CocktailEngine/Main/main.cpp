#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/Image/ImageServiceProvider.hpp>
#include <CocktailEngine/Core/Log/Log.hpp>
#include <CocktailEngine/Core/Log/LogServiceProvider.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/System/FileSystem/StorageService.hpp>
#include <CocktailEngine/Core/System/SystemServiceProvider.hpp>

#include <CocktailEngine/Main/CrashHandler.hpp>
#include <CocktailEngine/Main/Embedded/EmbeddedFileSystemDriver.hpp>
#include <CocktailEngine/Main/ExitCode.hpp>
#include <CocktailEngine/Main/main.hpp>

extern Ck::Main::ExitCode ApplicationMain(Ck::Application* application);

/// Files the build compiled into this program: the shaders a renderer is written
/// against, and anything else a module cannot run without. Every module declares
/// what it embeds on its own, and the build gathers all of it into this one
/// filesystem, so this is the only declaration a program needs and no module has
/// one to write.
CMRC_DECLARE(Builtin);

namespace Ck::Main
{
    namespace
    {
        class MainSystemServiceProvider : public SystemServiceProvider
        {
        public:

            /**
             * \brief Constructor
             * \param application The application where register services
             */
            explicit MainSystemServiceProvider(Application* application) :
                SystemServiceProvider(application)
            {
                /// Nothing
            }

        protected:

            void DoBoot(Application* application) override
            {
                SystemServiceProvider::DoBoot(application);

                application->Invoke([&](StorageService* storage) {
                    storage->Mount(CK_TEXT("builtin"), MakeUnique<EmbeddedFileSystemDriver>(cmrc::Builtin::get_filesystem()));
                });
            }
        };
    }

    COCKTAIL_DEFINE_LOG_CATEGORY(MainLogCategory);

    int InvokeMain(UniquePtr<Application> application)
    {
        ExitCode exitCode = ExitCode::Success;

        try
        {
#ifndef NDEBUG
            const bool waitedForDebugger = application->GetEnvironmentVariable(CK_TEXT("COCKTAIL_WAIT_FOR_DEBUGGER")).Map([](const String& variable) {
                return variable == CK_TEXT("1");
            }).GetOr(false);

            if (waitedForDebugger)
            {
                Thread::WaitUntil([&]() {
                    return application->IsDebuggerPresent();
                });
            }
#endif

            // Before the boot, and before anything else the program does: a failure while
            // booting is exactly the kind a report is wanted for. Nothing here needs a
            // service, only the executable path the platform layer answers on its own.
            InstallCrashHandler(application.Get());

            application->Instance(application.Get());

            application->RegisterServiceProvider<ImageServiceProvider>();
            application->RegisterServiceProvider<LogServiceProvider>();
            application->RegisterServiceProvider<MainSystemServiceProvider>();

            application->Boot();

            CK_LOG(MainLogCategory, LogLevel::Info, CK_TEXT("Application has been booted"));
#ifndef NDEBUG
            if (waitedForDebugger)
                CK_LOG(MainLogCategory, LogLevel::Info, CK_TEXT("Debugger connected"));
#endif

            if (application->IsExitRequested())
            {
                // A service provider gave up while booting. The program never starts, so
                // it has no exit code of its own and the requested one stands alone,
                // including when it is 0: asking to stop with a success is how a boot
                // that had nothing left to do says so.
                CK_LOG(MainLogCategory, LogLevel::Warning, CK_TEXT("Exit requested with code %u while booting, the application entry point is not called"), application->GetExitCode());
            }
            else
            {
                exitCode = ApplicationMain(application.Get());
            }
        }
        catch (const Exception& e)
        {
            CK_LOG(MainLogCategory, LogLevel::Critical, CK_TEXT("Process terminated with exception of type %s with message %s"), e.GetName(), e.GetMessage());
            exitCode = ExitCode::GeneralError;
        }
        catch (const std::exception& e)
        {
            CK_LOG(MainLogCategory, LogLevel::Critical, CK_TEXT("Process terminated with exception %s with message %s"), typeid(e).name(), e.what());
            exitCode = ExitCode::GeneralError;
        }

        // What the process returns. An entry point reaching its end returns Success for
        // having finished, which says nothing about why it was asked to stop, so an
        // explicit request is more specific and wins. A failure the entry point reports
        // itself wins over both: it knows something the caller of Exit did not.
        int processExitCode = static_cast<int>(exitCode);
        if (exitCode == ExitCode::Success && application->IsExitRequested())
        {
            const unsigned int requested = application->GetExitCode();

            // Unix keeps only the low eight bits of what main returns, so a requested
            // code that is a non-zero multiple of 256 would reach a shell as a success
            // there while reaching a caller on Windows as a failure. Reported as a
            // general error rather than turned into the opposite of what was asked.
            if (requested != 0 && (requested & 0xFFu) == 0)
            {
                CK_LOG(MainLogCategory, LogLevel::Error, CK_TEXT("Exit code %u cannot be carried by a process status, only its low eight bits would survive. Reporting a general error instead"), requested);
                processExitCode = static_cast<int>(ExitCode::GeneralError);
            }
            else
            {
                processExitCode = static_cast<int>(requested);
            }
        }

        if (application->IsExitRequested())
        {
            CK_LOG(MainLogCategory, processExitCode == 0 ? LogLevel::Info : LogLevel::Error, CK_TEXT("Application exited with code %d, on request"), processExitCode);
        }
        else
        {
            CK_LOG(MainLogCategory, processExitCode == 0 ? LogLevel::Info : LogLevel::Error, CK_TEXT("Application exited with code %d (%hs)"), processExitCode, Enum<ExitCode>::ToString(exitCode));
        }

        // The one place the application is torn down: the entry point has returned, so
        // everything it owned is already destroyed and no service is pulled from under a
        // live object.
        application.Reset();

        // After the application, since the handlers hold a pointer to it and a fault during
        // the teardown is still worth a report.
        UninstallCrashHandler();

        return processExitCode;
    }
}
