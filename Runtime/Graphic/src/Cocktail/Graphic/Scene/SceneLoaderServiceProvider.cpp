#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/Graphic/Scene/SceneLoader.hpp>
#include <Cocktail/Graphic/Scene/SceneLoaderServiceProvider.hpp>
#include <Cocktail/Graphic/Scene/Obj/ObjImporter.hpp>

namespace Ck
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(SceneLoaderServiceProvider);

	SceneLoaderServiceProvider::SceneLoaderServiceProvider(const Ref<Application>& application) :
		Super(application)
	{
		/// Nothing
	}

	void SceneLoaderServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<SceneLoader>();
		application->Singleton<ObjImporter>();
	}

	void SceneLoaderServiceProvider::DoBoot(const Ref<Application>& application)
	{
		application->Invoke([](Ref<SceneLoader> sceneLoader, Ref<ObjImporter> objImporter) {
			sceneLoader->RegisterImporter(objImporter);
		});
	}
}
