#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramCreateInfo.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramVariant.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API MaterialProgram
	{
	public:

	    /**
	     * \brief
	     */
	    MaterialProgram() = delete;

		/**
		 * \brief Constructor
		 * \param renderDevice
		 * \param createInfo
		 */
		MaterialProgram(Renderer::RenderDevice* renderDevice, const MaterialProgramCreateInfo& createInfo);

        MaterialProgram(const MaterialProgram& other) = delete;
        MaterialProgram(MaterialProgram&& other) = default;
        MaterialProgram& operator=(const MaterialProgram& other) = delete;
        MaterialProgram& operator=(MaterialProgram&& other) = default;

        MaterialProgramVariant* GetVariant(Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures) const;

		const String& GetName() const;

	    Material::ShadingMode GetShadingMode() const;

	private:

		using VariantKey = CompositeKey<Flags<VertexAttributeSemantic>, Flags<Material::TextureType>>;

		String mName;
	    Material::ShadingMode mShadingMode;
		std::unordered_map<VariantKey, UniquePtr<MaterialProgramVariant>> mVariants;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP
