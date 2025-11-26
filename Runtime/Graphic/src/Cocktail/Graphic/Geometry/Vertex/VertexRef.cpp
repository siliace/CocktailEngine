#include <Cocktail/Graphic/Geometry/Vertex/VertexRef.hpp>

namespace Ck
{
	const void* VertexConstRef::Get(VertexAttributeSemantic semantic) const
	{
		const VertexAttribute* vertexAttribute = mVertexLayout->FindAttribute(semantic);
		assert(vertexAttribute);

		std::size_t offset;
		if (mInterlaced)
		{
			offset = mIndex * mVertexLayout->GetStride() + vertexAttribute->GetOffset();
		}
		else
		{
			offset = vertexAttribute->GetOffset() * mVertexCount + vertexAttribute->GetStride() * mIndex;
		}

		return mVertices->GetData() + offset;
	}

	VertexConstRef::VertexConstRef(const VertexLayout* vertexLayout, const ByteArray* vertices, std::size_t index, std::size_t vertexCount, bool interlaced) :
		mVertexLayout(vertexLayout),
		mIndex(index),
		mVertexCount(vertexCount),
		mInterlaced(interlaced),
		mVertices(vertices)
	{
		/// Nothing
	}

	void VertexRef::Set(VertexAttributeSemantic semantic, const void* value, DataType dataType, unsigned int elementCount)
	{
		const VertexAttribute* vertexAttribute = mVertexLayout->FindAttribute(semantic);
		assert(vertexAttribute);
		assert(vertexAttribute->GetType() == dataType);
		assert(vertexAttribute->GetElementCount() * vertexAttribute->GetArrayLength() == elementCount);

		std::size_t offset;
		if (mInterlaced)
		{
			offset = mIndex * mVertexLayout->GetStride() + vertexAttribute->GetOffset();
		}
		else
		{
			offset = vertexAttribute->GetOffset() * mVertexCount + vertexAttribute->GetStride() * mIndex;
		}

		std::memcpy(mVertices->GetData() + offset, value, dataType.GetSize() * elementCount);
	}

	void VertexRef::SetColor(VertexAttributeSemantic semantic, const LinearColor& color)
	{
		float values[] = { color.R, color.G, color.B, color.A };
		Set<float>(semantic, values, 4);
	}

	VertexRef::VertexRef(const VertexLayout* vertexLayout, ByteArray* vertices, std::size_t index, std::size_t vertexCount, bool interlaced) :
		VertexConstRef(vertexLayout, vertices, index, vertexCount, interlaced),
		mVertices(vertices)
	{
		/// Nothing
	}
}
