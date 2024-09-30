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
		 * \param renderableType 
		 * \param shadingMode 
		 */
		MaterialProgramSet(Ref<MaterialProgramManager> materialProgramManager, RenderableType renderableType, Material::ShadingMode shadingMode);

		/**
		 * \brief
		 * \param vertexAttributes
		 * \param materialTextures
		 * \param materialProgram
		 */
		void Register(Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures, Ref<MaterialProgram> materialProgram);

		/**
		 * \brief 
		 * \param vertexAttributes 
		 * \param materialTextures 
		 * \return 
		 */
		Ref<MaterialProgram> FindMaterialProgram(Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures) const;

	private:

		using MaterialProgramKey = CompositeKey<Flags<VertexAttributeSemantic>, Flags<Material::TextureType>>;

		Ref<MaterialProgramManager> mMaterialProgramManager;
		std::unordered_map<MaterialProgramKey, Ref<MaterialProgram>> mMaterialPrograms;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMSET_HPP
