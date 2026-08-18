#include <CocktailEngine/Core/Application/Application.hpp>

#include <CocktailEngine/Graphic/Material/MipMaps/MipMapsLoader.hpp>
#include <CocktailEngine/Graphic/Material/MipMaps/MipMapsServiceProvider.hpp>
#include <CocktailEngine/Graphic/Material/MipMaps/DDS/DDSImporter.hpp>

namespace Ck
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(MipMapsServiceProvider);

	MipMapsServiceProvider::MipMapsServiceProvider(Application* application) :
		ServiceProvider(application)
	{
		/// Nothing
	}

	void MipMapsServiceProvider::DoRegister(Application* application)
	{
		application->Singleton<DDSImporter>();
		application->Singleton<MipMapsLoader>();
	}

	void MipMapsServiceProvider::DoBoot(Application* application)
	{
		application->Resolve<MipMapsLoader>()->RegisterImporter(
			application->Resolve<DDSImporter>()
		);
	}
}
