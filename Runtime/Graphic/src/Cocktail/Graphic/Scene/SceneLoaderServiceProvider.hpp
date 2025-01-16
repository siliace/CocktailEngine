#ifndef COCKTAIL_GRAPHIC_SCENE_SCENELOADERSERVICEPROVIDER_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENELOADERSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class SceneLoaderServiceProvider : public ServiceProvider
	{
	public:

		/**
		 * \brief 
		 * \param application 
		 */
		explicit SceneLoaderServiceProvider(Application* application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(Application* application) override;

		/**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(Application* application) override;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENELOADERSERVICEPROVIDER_HPP
