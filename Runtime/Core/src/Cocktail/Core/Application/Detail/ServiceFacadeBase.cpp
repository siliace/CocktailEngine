#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>

namespace Ck::Detail
{
	Application* ServiceFacadeBase::sFacadeApplication = nullptr;
	
	Application* ServiceFacadeBase::GetApplicationFacade()
	{
		return sFacadeApplication;
	}

	void ServiceFacadeBase::SetApplicationFacade(Application* application)
	{
		sFacadeApplication = application;
	}
}
