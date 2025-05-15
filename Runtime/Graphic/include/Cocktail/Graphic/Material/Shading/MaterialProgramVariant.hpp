#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMVARIANT_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMVARIANT_HPP

#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>

#include <Cocktail/Renderer/Shader/ShaderProgram.hpp>
#include <Cocktail/Renderer/Shader/VertexAttributeLocation.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	class COCKTAIL_GRAPHIC_API MaterialProgramVariant : public Extends<MaterialProgramVariant, Object>
	{
	public:

		MaterialProgramVariant(const MaterialProgramInterface& interface, Ref<Renderer::ShaderProgram> shaderProgram);

		/**
		 * \brief
		 * \param semantic
		 * \return
		 */
		Renderer::VertexAttributeLocation* GetVertexAttributeLocation(VertexAttributeSemantic semantic) const;

		/**
		 * \brief
		 * \param textureType
		 * \return
		 */
		Renderer::UniformSlot* GetMaterialTextureSlot(Material::TextureType textureType) const;

		/**
		 * \brief
		 * \return
		 */
		const Ref<Renderer::ShaderProgram>& GetShaderProgram() const;

	private:

		/**
		 * \brief
		 * \param stage
		 * \param name
		 * \return
		 */
		Renderer::VertexAttributeLocation* FindVertexAttributeLocation(Renderer::ShaderType stage, std::string_view name) const;

		Ref<Renderer::ShaderProgram> mShaderProgram;
		EnumMap<VertexAttributeSemantic, Renderer::VertexAttributeLocation*> mVertexAttributeLocations;
		EnumMap<Material::TextureType, Renderer::UniformSlot*> mTextureUniformSlots;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMVARIANT_HPP
