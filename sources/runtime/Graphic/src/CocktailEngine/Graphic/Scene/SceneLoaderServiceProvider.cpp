#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Image/ImageLoader.hpp>

#include <CocktailEngine/Graphic/Scene/SceneLoader.hpp>
#include <CocktailEngine/Graphic/Scene/SceneLoaderServiceProvider.hpp>
#include <CocktailEngine/Graphic/Scene/Container/Gltf/GltfImporter.hpp>
#include <CocktailEngine/Graphic/Scene/Container/Obj/ObjImporter.hpp>

namespace Ck
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(SceneLoaderServiceProvider);

	SceneLoaderServiceProvider::SceneLoaderServiceProvider(Application* application) :
		ServiceProvider(application)
	{
		/// Nothing
	}

	void SceneLoaderServiceProvider::DoRegister(Application* application)
	{
		application->Singleton<SceneLoader>();
		application->Singleton<ObjImporter>();
		application->Singleton<GltfImporter>([](ImageLoader* imageLoader) {
			return MakeUnique<GltfImporter>(imageLoader);
		});
	}

	void SceneLoaderServiceProvider::DoBoot(Application* application)
	{
		application->Invoke([](SceneLoader* sceneLoader, ObjImporter* objImporter) {
			sceneLoader->RegisterImporter(objImporter);
		});

		application->Invoke([](SceneLoader* sceneLoader, GltfImporter* gltfImporter) {
			sceneLoader->RegisterImporter(gltfImporter);
		});
	}
}
