#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Image/ImageServiceProvider.hpp>
#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Log/LogServiceProvider.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>

#include <Cocktail/Main/ExitCode.hpp>

extern Ck::Main::ExitCode ApplicationMain(Ck::Ref<Ck::Application> application);

namespace Ck::Main
{
	ExitCode InvokeMain(const Ref<Application>& application)
	{
		application->Instance(application);

		// Core service providers
		application->RegisterServiceProvider<ImageServiceProvider>();
		application->RegisterServiceProvider<LogServiceProvider>();
		application->RegisterServiceProvider<SystemServiceProvider>();

		application->Boot();

		ExitCode exitCode;

		try
		{
			exitCode = ApplicationMain(application);
		}
		catch (const std::exception& e)
		{
			application->Invoke([&](Ref<LogManager> logManager) {
				logManager->Critical("Process terminated with exception {}: {}", typeid(e).name(), e.what());
			});

			exitCode = ExitCode::GeneralError;
		}

		application->Terminate();

		return exitCode;
	}
}
