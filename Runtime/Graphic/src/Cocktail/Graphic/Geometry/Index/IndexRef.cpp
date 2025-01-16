#include <Cocktail/Graphic/Geometry/Index/IndexRef.hpp>

namespace Ck
{
	const void* IndexConstRef::Get() const
	{
		return mIndices->GetData() + ToDataType(mIndexType).GetSize() * mIndex;
	}

	IndexConstRef::IndexConstRef(Renderer::IndexType indexType, const ByteArray* indices, std::size_t index, std::size_t indexCount) :
		mIndexType(indexType),
		mIndex(index),
		mIndexCount(indexCount),
		mIndices(indices)
	{
		/// Nothing
	}

	IndexRef::IndexRef(Renderer::IndexType indexType, ByteArray* storage, std::size_t index, std::size_t indexCount) :
		IndexConstRef(indexType, storage, index, indexCount),
		mIndices(storage)
	{
		/// Nothing
	}
}
