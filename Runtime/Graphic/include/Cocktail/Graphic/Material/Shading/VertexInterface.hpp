#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXINTERFACE_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXINTERFACE_HPP

#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>

#include <Cocktail/Renderer/Shader/ShaderProgram.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API VertexInterface : public Implements<VertexInterface, Interface>
	{
	public:

		static constexpr std::string_view DefaultAttributePrefix = "ck_";
		static constexpr std::string_view DefaultAttributeSuffix;

		/**
		 * \brief 
		 * \param prefix 
		 * \param suffix 
		 * \param caseSensitive 
		 * \return 
		 */
		static Ref<VertexInterface> FromNames(const std::string& prefix = std::string(DefaultAttributePrefix), const std::string& suffix = std::string(DefaultAttributeSuffix), bool caseSensitive = true);

		/**
		 * \brief 
		 * \param shaderProgram 
		 * \param semantic 
		 * \return 
		 */
		virtual Renderer::VertexAttributeLocation* GetAttributeLocation(const Renderer::ShaderProgram& shaderProgram, VertexAttributeSemantic semantic) const = 0;
	};
}
#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXINTERFACE_HPP
