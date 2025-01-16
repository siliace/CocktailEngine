#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>

namespace Ck
{
	IndexArray::IndexArray(Renderer::IndexType indexType, std::size_t indexCount) :
		mIndexType(indexType),
		mIndexCount(indexCount)
	{
		assert(mIndexCount > 0);

		DataType dataType = Renderer::ToDataType(mIndexType);
		mIndices.Resize(mIndexCount * dataType.GetSize());
	}

	void IndexArray::Merge(const IndexArray& other)
	{
		assert(mIndexType == other.GetIndexType());

		mIndexCount += other.GetIndexCount();
		mIndices.Append(other.GetIndices());
	}

	IndexRef IndexArray::At(std::size_t index)
	{
		assert(index < mIndexCount);
		return IndexRef(mIndexType, &mIndices, index, mIndexCount);
	}

	IndexConstRef IndexArray::At(std::size_t index) const
	{
		assert(index < mIndexCount);
		return IndexConstRef(mIndexType, &mIndices, index, mIndexCount);
	}

	Renderer::IndexType IndexArray::GetIndexType() const
	{
		return mIndexType;
	}

	std::size_t IndexArray::GetIndexCount() const
	{
		return mIndexCount;
	}

	const ByteArray& IndexArray::GetIndices() const
	{
		return mIndices;
	}
}
