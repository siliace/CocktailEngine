#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/Image/ImageServiceProvider.hpp>
#include <CocktailEngine/Core/Log/Log.hpp>
#include <CocktailEngine/Core/Log/LogServiceProvider.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/System/FileSystem/StorageService.hpp>
#include <CocktailEngine/Core/System/SystemServiceProvider.hpp>

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

    ExitCode InvokeMain(UniquePtr<Application> application)
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

        ExitCode exitCode;

        try
        {
            exitCode = ApplicationMain(application.Get());
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

        CK_LOG(MainLogCategory, exitCode == ExitCode::Success ? LogLevel::Info : LogLevel::Error, CK_TEXT("Application exited with code %hs"), Enum<ExitCode>::ToString(exitCode));

        application.Reset();

        return exitCode;
    }
}
