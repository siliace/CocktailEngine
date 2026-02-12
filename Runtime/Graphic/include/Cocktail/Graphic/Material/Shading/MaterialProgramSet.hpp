#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP

#include <Cocktail/Graphic/Material/Material.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramManager.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API MaterialProgramSet
	{
	public:

		/**
		 * \brief 
		 * \param materialProgramManager
		 */
		MaterialProgramSet(std::shared_ptr<MaterialProgramManager> materialProgramManager);

		/**
		 * \brief 
		 * \param renderableType 
		 * \return 
		 */
		MaterialProgram* GetMaterialProgram(RenderableType renderableType, Material::ShadingMode shadingMode) const;

	private:

		std::shared_ptr<MaterialProgramManager> mMaterialProgramManager;
		EnumMap<RenderableType, EnumMap<Material::ShadingMode, MaterialProgram*>> mMaterialPrograms;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP
