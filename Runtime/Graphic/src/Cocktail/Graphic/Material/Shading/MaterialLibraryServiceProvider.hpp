#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALLIBRARYERVICEPROVIDER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALLIBRARYERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	class MaterialLibraryServiceProvider : public Extends<MaterialLibraryServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit MaterialLibraryServiceProvider(Ref<Application> application);

	protected:

		/**
		 * \brief
		 * \param application
		 */
		void DoRegister(const Ref<Application>& application) override;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALLIBRARYERVICEPROVIDER_HPP
