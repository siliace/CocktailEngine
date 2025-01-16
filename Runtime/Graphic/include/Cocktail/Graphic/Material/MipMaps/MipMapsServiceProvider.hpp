#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSSERVICEPROVIDER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API MipMapsServiceProvider : public ServiceProvider
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit MipMapsServiceProvider(Application* application);

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

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSSERVICEPROVIDER_HPP
