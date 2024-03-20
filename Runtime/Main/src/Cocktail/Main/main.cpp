#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Image/ImageServiceProvider.hpp>
#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Log/LogServiceProvider.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>

#include <Cocktail/Main/ExitCode.hpp>

#include <Cocktail/Renderer/RendererServiceProvider.hpp>

#include <Cocktail/Vulkan/VulkanServiceProvider.hpp>

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

		// Renderer service providers
		application->RegisterServiceProvider<Renderer::RendererServiceProvider>();

		// Vulkan service providers
		application->RegisterServiceProvider<Vulkan::VulkanServiceProvider>();

		application->Boot();

		ExitCode exitCode;

		try
		{
			exitCode = ApplicationMain(application);
		}
		catch (const std::exception& e)
		{
			Ref<LogManager> log = application->Resolve<LogManager>();
			log->Critical("Process terminated with exception {} : {}", typeid(e).name(), e.what());

			exitCode = ExitCode::GeneralError;
		}

		application->Terminate();

		return exitCode;
	}
}
