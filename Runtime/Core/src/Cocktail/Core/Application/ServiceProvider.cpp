#include <functional>

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	ServiceProvider::ServiceProvider(Application* application) :
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

		mOnBoot.Emit(mApplication, this);

		DoBoot(mApplication);

		mOnBooted.Emit(mApplication, this);

		mBooted = true;	
	}

	bool ServiceProvider::IsBooted() const
	{
		return mBooted;
	}

	Signal<Application*, ServiceProvider*>& ServiceProvider::OnBoot()
	{
		return mOnBoot;
	}

	Signal<Application*, ServiceProvider*>& ServiceProvider::OnBooted()
	{
		return mOnBooted;
	}

	void ServiceProvider::DoRegister(Application* application)
	{
		/// Nothing
	}

	void ServiceProvider::DoBoot(Application* application)
	{
		/// Nothing
	}
}
