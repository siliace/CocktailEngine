#include <Cocktail/Vulkan/Shader/VertexAttributeLocation.hpp>

namespace Ck::Vulkan
{
	VertexAttributeLocation::VertexAttributeLocation(unsigned int location, const DataType& dataType, unsigned int elementCount, String name) :
		mLocation(location),
		mDataType(dataType),
		mElementCount(elementCount),
		mName(std::move(name))
	{
		/// Nothing
	}

	unsigned int VertexAttributeLocation::GetLocation() const
	{
		return mLocation;
	}

	const DataType& VertexAttributeLocation::GetDataType() const
	{
		return mDataType;
	}

	unsigned int VertexAttributeLocation::GetElementCount() const
	{
		return mElementCount;
	}

	const String& VertexAttributeLocation::GetName() const
	{
		return mName;
	}
}
