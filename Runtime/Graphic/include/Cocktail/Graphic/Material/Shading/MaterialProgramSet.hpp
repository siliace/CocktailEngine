#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP

#include <Cocktail/Graphic/Material/Shading/MaterialProgramManager.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API MaterialProgramSet : public Extends<MaterialProgramSet, Object>
	{
	public:

		/**
		 * \brief 
		 * \param materialProgramManager
		 * \param shadingMode
		 */
		MaterialProgramSet(Ref<MaterialProgramManager> materialProgramManager, Material::ShadingMode shadingMode);

		/**
		 * \brief 
		 * \param renderableType 
		 * \return 
		 */
		Ref<MaterialProgram> GetMaterialProgram(RenderableType renderableType) const;

	private:

		Ref<MaterialProgramManager> mMaterialProgramManager;
		EnumMap<RenderableType, Ref<MaterialProgram>> mMaterialPrograms;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP
