#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Utility/Worker/WorkerManager.hpp>
#include <Cocktail/Core/Utility/Worker/WorkerServiceProvider.hpp>

namespace Ck
{
	WorkerServiceProvider::WorkerServiceProvider(const Ref<Application>& application) :
		Super(application)
	{
		/// Nothing
	}

	void WorkerServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<WorkerManager>();
	}
}
