#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>

namespace Ck
{
	Application::~Application()
	{
	    Terminate();
	}
	
	void Application::RegisterServiceProvider(UniquePtr<ServiceProvider> serviceProvider)
	{
		if (mBooted)
		{
			serviceProvider->Register();
			serviceProvider->Boot();
		}

		mServiceProviders.Add(std::move(serviceProvider));
	}

	bool Application::IsBooted() const
	{
		return mBooted;
	}

	void Application::Boot()
	{
		mServiceProviders.ForEach([](const UniquePtr<ServiceProvider>& serviceProvider) {
			serviceProvider->Register();
		});
		
		mBooted = true;
		Detail::ServiceFacadeBase::Boot(this);

		mServiceProviders.ForEach([](const UniquePtr<ServiceProvider>& serviceProvider) {
			serviceProvider->Boot();
		});

		mStart = Instant::Now();
	}

    void Application::Terminate()
	{
	    if (mBooted)
	    {
	        // Emit termination signals
	        // After this point, calls to this instance of application is invalid
	        mOnTerminate.Emit(this);

	        // Disconnect any signals managed by the application
	        DisconnectAll();

	        // Disconnect the application from facades classes
	        Detail::ServiceFacadeBase::Terminate();

            ServiceContainer::Clear();
	        mServiceProviders.Clear();
	        
	        mBooted = false;
	    }
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
