#include <Cocktail/Graphic/Geometry/Vertex/VertexRef.hpp>

namespace Ck
{
	VertexRef::VertexRef(VertexLayout* vertexLayout, unsigned char* storage, std::size_t index, std::size_t vertexCount, bool interlaced) :
		mVertexLayout(vertexLayout),
		mStorage(storage),
		mIndex(index),
		mVertexCount(vertexCount),
		mInterlaced(interlaced)
	{
		/// Nothing
	}
}
