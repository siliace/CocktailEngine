#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP

#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>

#include <Cocktail/Renderer/Shader/ShaderProgram.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API MaterialProgram : public Extends<MaterialProgram, Object>
	{
	public:

		/**
		 * \brief Constructor
		 * \param name
		 * \param interface
		 * \param shaderProgram
		 */
		MaterialProgram(std::string name, const MaterialProgramInterface& interface, Ref<Renderer::ShaderProgram> shaderProgram);

		/**
		 * \brief 
		 * \param stage 
		 * \param name 
		 * \return 
		 */
		Renderer::VertexAttributeLocation* FindVertexAttributeLocation(Renderer::ShaderType stage, std::string_view name) const;

		/**
		 * \brief
		 * \param name
		 * \return
		 */
		Renderer::UniformSlot* FindUniformSlot(std::string_view name) const;

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
		const std::string& GetName() const;

		/**
		 * \brief 
		 * \return 
		 */
		const Ref<Renderer::ShaderProgram>& GetShaderProgram() const;

	private:

		std::string mName;
		Ref<Renderer::ShaderProgram> mShaderProgram;
		EnumMap<VertexAttributeSemantic, Renderer::VertexAttributeLocation*> mVertexAttributeLocations;
		EnumMap<Material::TextureType, Renderer::UniformSlot*> mTextureUniformSlots;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP
