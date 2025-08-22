#include <Cocktail/Core/Application/App.hpp>

namespace Ck
{
	void App::RegisterServiceProvider(std::unique_ptr<ServiceProvider> serviceProvider)
	{
		ResolveFacadeInstance()->RegisterServiceProvider(std::move(serviceProvider));
	}

	bool App::IsBooted()
	{
		return ResolveFacadeInstance()->IsBooted();
	}

	void App::Boot()
	{
		ResolveFacadeInstance()->Boot();
	}

	void App::Exit(unsigned int exitCode, bool force, std::string_view callSite)
	{
		ResolveFacadeInstance()->Exit(exitCode, force, callSite);
	}

	Duration App::Uptime()
	{
		return ResolveFacadeInstance()->Uptime();
	}

	const Array<std::string>& App::GetArgv()
	{
		return ResolveFacadeInstance()->GetArgv();
	}

	std::string App::GetEnvironmentVariable(std::string_view name)
	{
		return ResolveFacadeInstance()->GetEnvironmentVariable(name);
	}

	bool App::IsDebuggerPresent()
	{
		return ResolveFacadeInstance()->IsDebuggerPresent();
	}

	Signal<Application*>& App::OnTerminate()
	{
		return ResolveFacadeInstance()->OnTerminate();
	}
}
