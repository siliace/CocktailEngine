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

	bool ServiceProvider::IsBooted() const
	{
		return mBooted;
	}

	Signal<Ref<Application>, Ref<ServiceProvider>>& ServiceProvider::OnBoot()
	{
		return mOnBoot;
	}

	Signal<Ref<Application>, Ref<ServiceProvider>>& ServiceProvider::OnBooted()
	{
		return mOnBooted;
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
