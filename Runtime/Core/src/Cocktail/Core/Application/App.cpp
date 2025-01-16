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
}
