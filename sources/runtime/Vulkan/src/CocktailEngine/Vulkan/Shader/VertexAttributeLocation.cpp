#include <CocktailEngine/Vulkan/Shader/VertexAttributeLocation.hpp>

namespace Ck::Vulkan
{
	VertexAttributeLocation::VertexAttributeLocation(unsigned int location, const DataType& dataType, unsigned int elementCount, AsciiString name) :
		mLocation(location),
		mDataType(dataType),
		mElementCount(elementCount),
		mName(Move(name))
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

	const AsciiString& VertexAttributeLocation::GetName() const
	{
		return mName;
	}
}
