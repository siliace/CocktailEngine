#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSSERVICEPROVIDER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API MipMapsServiceProvider : public Extends<MipMapsServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit MipMapsServiceProvider(const Ref<Application>& application);

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

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSSERVICEPROVIDER_HPP
