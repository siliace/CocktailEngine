#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXATTRIBUTESEMANTIC_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXATTRIBUTESEMANTIC_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	enum class VertexAttributeSemantic
	{
		/**
		 * \brief
		 */
		Position = Bit(0),

		/**
		 * \brief
		 */
		Normal = Bit(1),

		/**
		 * \brief
		 */
		TexCoord = Bit(2),

		/**
		 * \brief
		 */
		Color = Bit(3),

		/**
		 * \brief 
		 */
		Tangent = Bit(4),

		/**
		 * \brief 
		 */
		BiTangent = Bit(5),
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXATTRIBUTESEMANTIC_HPP
