#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMapsLoader.hpp>
#include <Cocktail/Graphic/Material/MipMaps/MipMapsServiceProvider.hpp>
#include <Cocktail/Graphic/Material/MipMaps/DDS/DDSImporter.hpp>

namespace Ck
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(MipMapsServiceProvider);

	MipMapsServiceProvider::MipMapsServiceProvider(const Ref<Application>& application) :
		Super(application)
	{
		/// Nothing
	}

	void MipMapsServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<DDSImporter>();
		application->Singleton<MipMapsLoader>();
	}

	void MipMapsServiceProvider::DoBoot(const Ref<Application>& application)
	{
		application->Resolve<MipMapsLoader>()->RegisterImporter(
			application->Resolve<DDSImporter>()
		);
	}
}
