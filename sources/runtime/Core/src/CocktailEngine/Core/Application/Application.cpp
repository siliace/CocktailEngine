#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Application/ServiceProvider.hpp>
#include <CocktailEngine/Core/Application/Detail/ServiceFacadeBase.hpp>

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

		mServiceProviders.Add(Move(serviceProvider));
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

		mServiceProviders.ForEach([](const UniquePtr<ServiceProvider>& serviceProvider) {
			serviceProvider->Boot();
		});

	    mBooted = true;
	    Detail::ServiceFacadeBase::Boot(this);

		mStart = Instant::Now();
	}

    void Application::Terminate()
	{
	    if (mBooted)
	    {
	        // Emit termination signals
	        // After this point, calls to this instance of application is invalid
	        mOnTerminate.Emit(this);

	        // Disconnect the application from facades classes
	        Detail::ServiceFacadeBase::Terminate();

	        // Disconnect any signals managed by the application
	        DisconnectAll();

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
