#include <Cocktail/Vulkan/Shader/VertexAttributeLocation.hpp>

namespace Ck::Vulkan
{
	VertexAttributeLocation::VertexAttributeLocation(unsigned int location, const DataType& dataType, unsigned int elementCount, std::string name) :
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

	const std::string& VertexAttributeLocation::GetName() const
	{
		return mName;
	}
}
