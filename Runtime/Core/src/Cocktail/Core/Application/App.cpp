#include <Cocktail/Core/Application/App.hpp>

namespace Ck
{
	void App::RegisterServiceProvider(const Ref<ServiceProvider>& serviceProvider)
	{
		ResolveFacadeInstance()->RegisterServiceProvider(serviceProvider);
	}

	bool App::IsBooted()
	{
		return ResolveFacadeInstance()->IsBooted();
	}

	void App::Boot()
	{
		ResolveFacadeInstance()->Boot();
	}
}
