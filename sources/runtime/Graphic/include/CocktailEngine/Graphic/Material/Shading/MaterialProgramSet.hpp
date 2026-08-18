#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP

#include <CocktailEngine/Graphic/Material/Material.hpp>
#include <CocktailEngine/Graphic/Material/Shading/MaterialProgramManager.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_GRAPHIC_API MaterialProgramSet
	{
	public:

		/**
		 * \brief 
		 * \param materialProgramManager
		 */
		MaterialProgramSet(SharedPtr<MaterialProgramManager> materialProgramManager);

		/**
		 * \brief 
		 * \param renderableType 
		 * \return 
		 */
		MaterialProgram* GetMaterialProgram(RenderableType renderableType, Material::ShadingMode shadingMode) const;

	private:

		SharedPtr<MaterialProgramManager> mMaterialProgramManager;
		EnumMap<RenderableType, EnumMap<Material::ShadingMode, MaterialProgram*>> mMaterialPrograms;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP
