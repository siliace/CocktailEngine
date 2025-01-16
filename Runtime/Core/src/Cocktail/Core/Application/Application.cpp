#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>

namespace Ck
{
	Application::~Application()
	{
		Terminate();
	}
	
	void Application::RegisterServiceProvider(std::unique_ptr<ServiceProvider> serviceProvider)
	{
		serviceProvider->Register();

		if (mBooted)
			serviceProvider->Boot();

		mServiceProviders.emplace_back(std::move(serviceProvider));
	}

	bool Application::IsBooted() const
	{
		return mBooted;
	}

	void Application::Boot()
	{
		for (std::unique_ptr<ServiceProvider>& serviceProvider : mServiceProviders)
			serviceProvider->Register();
		
		mBooted = true;
		Detail::ServiceFacadeBase::Boot(this);

		for (std::unique_ptr<ServiceProvider>& serviceProvider : mServiceProviders)
			serviceProvider->Boot();

		mStart = Instant::Now();
	}

	void Application::Terminate()
	{
		mOnTerminate.Emit(this);
		Detail::ServiceFacadeBase::Terminate();

		DisconnectAll();
	}

	Duration Application::Uptime() const
	{
		return Duration::Between(mStart, Instant::Now());
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
