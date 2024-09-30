#include <Cocktail/Graphic/Geometry/Index/IndexRef.hpp>

namespace Ck
{
	IndexRef::IndexRef(Renderer::IndexType indexType, unsigned char* storage, std::size_t index, std::size_t indexCount):
		mIndexType(indexType),
		mStorage(storage),
		mIndex(index),
		mIndexCount(indexCount)
	{
		/// Nothing
	}
}
