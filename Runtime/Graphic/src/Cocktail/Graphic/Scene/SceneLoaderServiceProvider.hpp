#ifndef COCKTAIL_GRAPHIC_SCENE_SCENELOADERSERVICEPROVIDER_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENELOADERSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class SceneLoaderServiceProvider : public Extends<SceneLoaderServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief 
		 * \param application 
		 */
		explicit SceneLoaderServiceProvider(const Ref<Application>& application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(const Ref<Application>& application) override;

		/**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(const Ref<Application>& application) override;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENELOADERSERVICEPROVIDER_HPP
