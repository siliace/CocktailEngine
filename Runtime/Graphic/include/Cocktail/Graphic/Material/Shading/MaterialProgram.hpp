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
		 * \brief Constructor
		 * \param renderDevice
		 * \param createInfo
		 */
		MaterialProgram(const std::shared_ptr<Renderer::RenderDevice>& renderDevice, const MaterialProgramCreateInfo& createInfo);

		std::shared_ptr<MaterialProgramVariant> GetVariant(Flags<VertexAttributeSemantic> vertexAttributes, Flags<Material::TextureType> materialTextures) const;

		const std::string& GetName() const;

	private:

		using VariantKey = CompositeKey<Flags<VertexAttributeSemantic>, Flags<Material::TextureType>>;

		std::string mName;
		std::unordered_map<VariantKey, std::shared_ptr<MaterialProgramVariant>> mVariants;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP
