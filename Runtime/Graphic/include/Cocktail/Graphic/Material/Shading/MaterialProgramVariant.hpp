#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMVARIANT_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMVARIANT_HPP

#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>

#include <Cocktail/Renderer/Shader/ShaderProgram.hpp>
#include <Cocktail/Renderer/Shader/VertexAttributeLocation.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	class COCKTAIL_GRAPHIC_API MaterialProgramVariant
	{
	public:

		MaterialProgramVariant(const MaterialProgramInterface& interface, std::shared_ptr<Renderer::ShaderProgram> shaderProgram);

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
		const std::shared_ptr<Renderer::ShaderProgram>& GetShaderProgram() const;

        /**
	     * \brief
	     * \return
	     */
	    Flags<VertexAttributeSemantic> GetVertexAttributeUsage() const;

        /**
         * \brief
         * \return
         */
        Flags<Material::TextureType> GetMaterialTextureUsage() const;

    private:

		std::shared_ptr<Renderer::ShaderProgram> mShaderProgram;
		EnumMap<VertexAttributeSemantic, Renderer::VertexAttributeLocation*> mVertexAttributeLocations;
		EnumMap<Material::TextureType, Renderer::UniformSlot*> mTextureUniformSlots;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAMVARIANT_HPP
