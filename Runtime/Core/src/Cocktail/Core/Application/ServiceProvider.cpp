#include <functional>

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	ServiceProvider::ServiceProvider(const Ref<Application>& application) :
		mApplication(application),
		mBooted(false)
	{
		/// Nothing
	}
	
	ServiceProvider::~ServiceProvider()
	{
		DisconnectAll();
	}

	void ServiceProvider::Register()
	{
		DoRegister(mApplication);
	}

	void ServiceProvider::Boot()
	{
		if (mBooted)
			return;

		mOnBoot.Emit(mApplication, Self());

		DoBoot(mApplication);

		mOnBooted.Emit(mApplication, Self());

		mBooted = true;	
	}

	void ServiceProvider::OnBoot(const std::function<void(Ref<Application>, Ref<ServiceProvider>)>& callback)
	{
		Connect(mOnBoot, callback);
	}

	void ServiceProvider::OnBooted(const std::function<void(Ref<Application>, Ref<ServiceProvider>)>& callback)
	{
		Connect(mOnBooted, callback);
	}

	void ServiceProvider::DoRegister(const Ref<Application>& application)
	{
		/// Nothing
	}

	void ServiceProvider::DoBoot(const Ref<Application>& application)
	{
		/// Nothing
	}
}
