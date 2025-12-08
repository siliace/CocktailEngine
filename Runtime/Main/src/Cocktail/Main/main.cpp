#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Image/ImageServiceProvider.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Log/LogServiceProvider.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>
#include <Cocktail/Core/System/Concurrency/Thread.hpp>

#include <Cocktail/Main/ExitCode.hpp>
#include <Cocktail/Main/main.hpp>

extern Ck::Main::ExitCode ApplicationMain(Ck::Application* application);

namespace Ck::Main
{
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
		application->RegisterServiceProvider<SystemServiceProvider>();

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

		CK_LOG(MainLogCategory, exitCode == ExitCode::Success ? LogLevel::Info : LogLevel::Error, CK_TEXT("Application exited with code %hs"), exitCode);

	    application->Terminate();
	    application.Reset();

		return exitCode;
	}
}