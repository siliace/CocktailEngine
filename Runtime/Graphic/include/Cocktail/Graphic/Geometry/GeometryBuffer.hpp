#ifndef COCKTAIL_GRAPHIC_GEOMETRY_GEOMETRYBUFFER_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_GEOMETRYBUFFER_HPP

#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>

namespace Ck
{
	class GeometryBuffer
	{
	public:

	private:

		std::shared_ptr<VertexArray> mVertices;
		std::shared_ptr<IndexArray> mIndices;
	};
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_GEOMETRYBUFFER_HPP
