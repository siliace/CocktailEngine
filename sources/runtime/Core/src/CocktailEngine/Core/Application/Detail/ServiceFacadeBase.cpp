#include <cassert>

#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Application/Detail/ServiceFacadeBase.hpp>

namespace Ck::Detail
{
	Application* ServiceFacadeBase::sFacadeApplication = nullptr;
	Signal<Application*> ServiceFacadeBase::sOnFacadeApplicationReady;
	Array<Connection> ServiceFacadeBase::sReadyConnections;
	
	Application* ServiceFacadeBase::GetApplicationFacade()
	{
		return sFacadeApplication;
	}

	void ServiceFacadeBase::Boot(Application* application)
	{
		assert(application && !sFacadeApplication);
		sFacadeApplication = application;
		sOnFacadeApplicationReady.Emit(application);
		sReadyConnections.Clear();
	}

	void ServiceFacadeBase::Terminate()
	{
		sFacadeApplication = nullptr;
	}
}
