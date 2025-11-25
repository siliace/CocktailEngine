#include <Cocktail/Core/Application/App.hpp>

namespace Ck
{
	void App::RegisterServiceProvider(UniquePtr<ServiceProvider> serviceProvider)
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

	void App::Exit(unsigned int exitCode, bool force, StringView callSite)
	{
		ResolveFacadeInstance()->Exit(exitCode, force, callSite);
	}

	Duration App::Uptime()
	{
		return ResolveFacadeInstance()->Uptime();
	}

	const Array<String>& App::GetArgv()
	{
		return ResolveFacadeInstance()->GetArgv();
	}

	Optional<String> App::GetEnvironmentVariable(StringView name)
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
