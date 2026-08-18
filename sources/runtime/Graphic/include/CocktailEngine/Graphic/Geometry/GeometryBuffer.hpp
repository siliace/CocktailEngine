#ifndef COCKTAIL_GRAPHIC_GEOMETRY_GEOMETRYBUFFER_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_GEOMETRYBUFFER_HPP

#include <CocktailEngine/Graphic/Geometry/Index/IndexArray.hpp>
#include <CocktailEngine/Graphic/Geometry/Vertex/VertexArray.hpp>

namespace Ck
{
	class GeometryBuffer
	{
	public:

	private:

		SharedPtr<VertexArray> mVertices;
		SharedPtr<IndexArray> mIndices;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_GEOMETRYBUFFER_HPP
