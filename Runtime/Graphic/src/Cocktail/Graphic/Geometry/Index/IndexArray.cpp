#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>

namespace Ck
{
	IndexArray::IndexArray(Renderer::IndexType indexType, std::size_t indexCount):
		mIndexType(indexType),
		mIndexCount(indexCount)
	{
		DataType dataType = Renderer::ToDataType(mIndexType);
		mStorage = std::make_unique<unsigned char[]>(mIndexCount * dataType.GetSize());
	}

	IndexRef IndexArray::At(std::size_t index) const
	{
		if (index >= mIndexCount)
			throw std::out_of_range("VertexArray subscript out of range");

		return IndexRef(mIndexType, mStorage.get(), index, mIndexCount);
	}

	Renderer::IndexType IndexArray::GetIndexType() const
	{
		return mIndexType;
	}

	std::size_t IndexArray::GetIndexCount() const
	{
		return mIndexCount;
	}

	const void* IndexArray::GetPointer() const
	{
		return mStorage.get();
	}
}
