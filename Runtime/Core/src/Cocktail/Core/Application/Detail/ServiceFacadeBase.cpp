#include <cassert>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>

namespace Ck::Detail
{
	Application* ServiceFacadeBase::sFacadeApplication = nullptr;
	Signal<Application*> ServiceFacadeBase::sOnFacadeApplicationReady;
	std::vector<Connection> ServiceFacadeBase::sReadyConnections;
	
	Application* ServiceFacadeBase::GetApplicationFacade()
	{
		return sFacadeApplication;
	}

	void ServiceFacadeBase::SetApplicationFacade(Application* application)
	{
		if (sFacadeApplication)
			sReadyConnections.clear();

		assert(application);
		sFacadeApplication = application;
		sOnFacadeApplicationReady.Emit(application);
	}
}
