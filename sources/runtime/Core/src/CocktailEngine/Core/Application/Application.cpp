#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Application/ServiceProvider.hpp>
#include <CocktailEngine/Core/Application/Detail/ServiceFacadeBase.hpp>
#include <CocktailEngine/Core/Log/Log.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_LOG_CATEGORY(ApplicationLogCategory, LogLevel::Info);
	COCKTAIL_DEFINE_LOG_CATEGORY(ApplicationLogCategory);

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

	void Application::Exit(unsigned int exitCode, StringView callSite)
	{
		// The first request wins, and it wins atomically with the code it carries: a
		// loop that already read the request must not see the code it is about to
		// return change under it, and a shutdown asking to exit again while unwinding
		// must not overwrite the original reason.
		Int64 noRequest = -1;
		if (!mExitRequest.compare_exchange_strong(noRequest, static_cast<Int64>(exitCode)))
			return;

		// Guarded: a facade asserts on an application that is not booted, and an exit
		// can legitimately be requested from a service provider being booted.
		if (mBooted)
			CK_LOG(ApplicationLogCategory, LogLevel::Info, CK_TEXT("Exit requested with code %u from %s"), exitCode, callSite);

		// Deliberately no teardown here: whoever owns the loop leaves it, and the
		// unwinding of the entry point is what shuts the engine down in order.
		mOnExitRequested.Emit(this);
	}

	bool Application::IsExitRequested() const
	{
		return mExitRequest.load() >= 0;
	}

	unsigned int Application::GetExitCode() const
	{
		const Int64 request = mExitRequest.load();

		return request < 0 ? 0 : static_cast<unsigned int>(request);
	}

	Signal<Application*>& Application::OnExitRequested()
	{
		return mOnExitRequested;
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
		mBooted(false),
		mExitRequest(-1)
	{
		/// Nothing
	}
}
