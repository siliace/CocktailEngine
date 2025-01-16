#include <Cocktail/Graphic/Geometry/Vertex/VertexAttribute.hpp>

namespace Ck
{
	bool VertexAttribute::IsCompatibleWith(const VertexAttribute& other) const
	{
		return std::tie(mSemantic, mType, mElementCount, mArrayLength, mNormalized) ==
			std::tie(other.mSemantic, other.mType, other.mElementCount, other.mArrayLength, other.mNormalized);
	}

	unsigned int VertexAttribute::GetStride() const
	{
		return static_cast<unsigned int>(mType.GetSize()) * mElementCount * mArrayLength;
	}

	VertexAttributeSemantic VertexAttribute::GetSemantic() const
	{
		return mSemantic;
	}

	DataType VertexAttribute::GetType() const
	{
		return mType;
	}

	unsigned int VertexAttribute::GetOffset() const
	{
		return mOffset;
	}

	unsigned int VertexAttribute::GetElementCount() const
	{
		return mElementCount;
	}

	unsigned int VertexAttribute::GetArrayLength() const
	{
		return mArrayLength;
	}

	bool VertexAttribute::IsNormalized() const
	{
		return mNormalized;
	}

	bool VertexAttribute::operator==(const VertexAttribute& rhs) const
	{
		return std::tie(mSemantic, mType, mOffset, mElementCount, mArrayLength, mNormalized) == 
			std::tie(rhs.mSemantic, rhs.mType, rhs.mOffset, rhs.mElementCount, rhs.mArrayLength, rhs.mNormalized);
	}

	bool VertexAttribute::operator!=(const VertexAttribute& rhs) const
	{
		return !(*this == rhs);
	}

	VertexAttribute::VertexAttribute(VertexAttributeSemantic semantic, DataType type, unsigned int offset, unsigned int elementCount, unsigned int arrayLength, bool normalized) :
		mSemantic(semantic),
		mType(type),
		mOffset(offset),
		mElementCount(elementCount),
		mArrayLength(arrayLength),
		mNormalized(normalized)
	{
		/// Nothing
	}
}
