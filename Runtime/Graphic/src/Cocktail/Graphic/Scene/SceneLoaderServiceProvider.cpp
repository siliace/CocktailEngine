#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Image/ImageLoader.hpp>

#include <Cocktail/Graphic/Scene/SceneLoader.hpp>
#include <Cocktail/Graphic/Scene/SceneLoaderServiceProvider.hpp>
#include <Cocktail/Graphic/Scene/Container/Gltf/GltfImporter.hpp>
#include <Cocktail/Graphic/Scene/Container/Obj/ObjImporter.hpp>

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
			return std::make_unique<GltfImporter>(imageLoader);
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
