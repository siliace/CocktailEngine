#include <Cocktail/Graphic/Geometry/Vertex/VertexRef.hpp>

namespace Ck
{
	void VertexRef::Set(VertexAttributeSemantic semantic, const LinearColor& color)
	{
		float values[] = { color.R, color.G, color.B, color.A };
		Set<float>(semantic, values, 4);
	}

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
