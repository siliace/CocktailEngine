#ifndef COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP

#include <Cocktail/Core/Meta/Extends.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Graphic/Material/Shading/VertexInterface.hpp>

#include <Cocktail/Renderer/Shader/VertexAttributeLocation.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API MaterialProgram : public Extends<MaterialProgram, Object>
	{
	public:

		/**
		 * \brief
		 * \param name
		 * \param shaderProgram
		 * \param vertexInterface
		 */
		MaterialProgram(std::string name, Ref<Renderer::ShaderProgram> shaderProgram, Ref<VertexInterface> vertexInterface = VertexInterface::FromNames());

		/**
		 * \brief 
		 * \param semantic 
		 * \return 
		 */
		Renderer::VertexAttributeLocation* GetAttributeLocation(VertexAttributeSemantic semantic) const;

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
		Ref<VertexInterface> mVertexInterface;
		EnumMap<VertexAttributeSemantic, Renderer::VertexAttributeLocation*> mVertexAttributeLocations;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_SHADING_MATERIALPROGRAM_HPP
