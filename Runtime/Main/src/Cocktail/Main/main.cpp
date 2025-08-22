#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Image/ImageServiceProvider.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Log/LogServiceProvider.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>
#include <Cocktail/Core/System/Concurrency/ThreadUtils.hpp>

#include <Cocktail/Main/ExitCode.hpp>
#include <Cocktail/Main/main.hpp>

extern Ck::Main::ExitCode ApplicationMain(Ck::Application* application);

namespace Ck::Main
{
	COCKTAIL_DEFINE_LOG_CATEGORY(MainLogCategory);

	ExitCode InvokeMain(Application* application)
	{
#ifndef NDEBUG
		const bool waitedForDebugger = application->GetEnvironmentVariable("COCKTAIL_WAIT_FOR_DEBUGGER") == "1";
		if (waitedForDebugger)
		{
			ThreadUtils::WaitUntil([&]() {
				return application->IsDebuggerPresent();
			});
		}
#endif

		application->Instance(application);

		application->RegisterServiceProvider<ImageServiceProvider>();
		application->RegisterServiceProvider<LogServiceProvider>();
		application->RegisterServiceProvider<SystemServiceProvider>();

		application->Boot();
		  
		CK_LOG(MainLogCategory, LogLevel::Info, "Application has been booted");
#ifndef NDEBUG
		if (waitedForDebugger)
			CK_LOG(MainLogCategory, LogLevel::Info, "Debugger connected");
#endif

		ExitCode exitCode;

		try
		{
			exitCode = ApplicationMain(application);
		}
		catch (const std::exception& e)
		{
			CK_LOG(MainLogCategory, LogLevel::Critical, "Process terminated with exception {}: {}", typeid(e).name(), e.what());
			exitCode = ExitCode::GeneralError;
		}

		application->Terminate();

		CK_LOG(MainLogCategory, exitCode == ExitCode::Success ? LogLevel::Info : LogLevel::Error, "Application exited with code {}", Enum<ExitCode>::ToString(exitCode));

		return exitCode;
	}
}
