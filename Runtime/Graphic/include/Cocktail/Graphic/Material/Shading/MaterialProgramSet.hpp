#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP

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
		 * \param shadingMode
		 */
		MaterialProgramSet(std::shared_ptr<MaterialProgramManager> materialProgramManager, Material::ShadingMode shadingMode);

		/**
		 * \brief 
		 * \param renderableType 
		 * \return 
		 */
		std::shared_ptr<MaterialProgram> GetMaterialProgram(RenderableType renderableType) const;

	private:

		std::shared_ptr<MaterialProgramManager> mMaterialProgramManager;
		EnumMap<RenderableType, std::shared_ptr<MaterialProgram>> mMaterialPrograms;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP
