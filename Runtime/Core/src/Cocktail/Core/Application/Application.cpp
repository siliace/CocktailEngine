#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>

namespace Ck
{
	Application::~Application()
	{
		Terminate();
	}
	
	void Application::RegisterServiceProvider(const Ref<ServiceProvider>& serviceProvider)
	{
		serviceProvider->Register();

		if (mBooted)
			serviceProvider->Boot();

		mServiceProviders.emplace_back(serviceProvider);
	}

	bool Application::IsBooted() const
	{
		return mBooted;
	}

	void Application::Boot()
	{
		for (Ref<ServiceProvider>& serviceProvider : mServiceProviders)
			serviceProvider->Register();
		
		mBooted = true;
		Detail::ServiceFacadeBase::SetApplicationFacade(this);

		for (Ref<ServiceProvider>& serviceProvider : mServiceProviders)
			serviceProvider->Boot();
	}

	void Application::Terminate()
	{
		mOnTerminate.Emit(this);

		DisconnectAll();
	}

	Signal<Application*>& Application::OnTerminate()
	{
		return mOnTerminate;
	}

	Application::Application() :
		mBooted(false)
	{
		/// Nothing
	}
}
