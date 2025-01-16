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

	void ServiceFacadeBase::Boot(Application* application)
	{
		assert(application && !sFacadeApplication);
		sFacadeApplication = application;
		sOnFacadeApplicationReady.Emit(application);
	}

	void ServiceFacadeBase::Terminate()
	{
		sReadyConnections.clear();
		sFacadeApplication = nullptr;
	}
}
