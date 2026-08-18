#include <cstdlib>

#include <CocktailEngine/Core/Application/App.hpp>

namespace Ck
{
	void App::RegisterServiceProvider(UniquePtr<ServiceProvider> serviceProvider)
	{
		ResolveFacadeInstance()->RegisterServiceProvider(Move(serviceProvider));
	}

	bool App::IsBooted()
	{
		return ResolveFacadeInstance()->IsBooted();
	}

	void App::Boot()
	{
		ResolveFacadeInstance()->Boot();
	}

	void App::Exit(unsigned int exitCode, StringView callSite)
	{
		ResolveFacadeInstance()->Exit(exitCode, callSite);
	}

	bool App::IsExitRequested()
	{
		return ResolveFacadeInstance()->IsExitRequested();
	}

	unsigned int App::GetExitCode()
	{
		return ResolveFacadeInstance()->GetExitCode();
	}

	Signal<Application*>& App::OnExitRequested()
	{
		return ResolveFacadeInstance()->OnExitRequested();
	}

	void App::Abort(unsigned int exitCode)
	{
		ResolveFacadeInstance()->Abort(exitCode);

		// Not reached. Abort does not return, but the call above is a virtual one and the
		// compiler does not take the promise of a declaration for the whole hierarchy,
		// so the end of this function has to be unreachable on its own terms. An
		// implementation that did return would land here, which is the right answer for
		// it anyway.
		std::abort();
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
