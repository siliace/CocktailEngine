#include <Cocktail/Renderer/Buffer/IndexType.hpp>

namespace Ck::Renderer
{
	DataType ToDataType(IndexType indexType)
	{
		switch (indexType)
		{
		case IndexType::Short:
			return DataType::UnsignedInt16;

		case IndexType::Integer:
			return DataType::UnsignedInt32;

		case IndexType::Byte:
			return DataType::UnsignedInt8;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}
}
