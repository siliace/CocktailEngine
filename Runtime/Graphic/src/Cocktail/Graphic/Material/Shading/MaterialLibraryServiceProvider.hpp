#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALLIBRARYERVICEPROVIDER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALLIBRARYERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	class MaterialLibraryServiceProvider : public ServiceProvider
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit MaterialLibraryServiceProvider(Application* application);

	protected:

		/**
		 * \brief
		 * \param application
		 */
		void DoRegister(Application* application) override;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALLIBRARYERVICEPROVIDER_HPP
