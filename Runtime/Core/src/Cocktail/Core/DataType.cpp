#include <Cocktail/Core/DataType.hpp>

namespace Ck
{
	const DataType DataType::Int8 = Of<char>();
	const DataType DataType::UnsignedInt8 = Of<unsigned char>();
	const DataType DataType::Int16 = Of<short>();
	const DataType DataType::UnsignedInt16 = Of<unsigned short>();
	const DataType DataType::Int32 = Of<int>();
	const DataType DataType::UnsignedInt32 = Of<unsigned int>();
	const DataType DataType::Int64 = Of<long long>();
	const DataType DataType::UnsignedInt64 = Of<unsigned long long>();
	const DataType DataType::Float16 = DataType(2, true, false);
	const DataType DataType::Float32 = Of<float>();
	const DataType DataType::Float64 = Of<double>();

	DataType::DataType() :
		DataType(0, false, false)
	{
		/// Nothing
	}

	DataType::DataType(std::size_t size, bool floatingPoint, bool unsigned_) :
		mSize(size),
		mFloatingPoint(floatingPoint),
		mUnsigned(unsigned_)
	{
		/// Nothing
	}

	std::size_t DataType::GetSize() const
	{
		return mSize;
	}

	bool DataType::IsFloatingPoint() const
	{
		return mFloatingPoint;
	}

	bool DataType::IsUnsigned() const
	{
		return mUnsigned;
	}

	bool operator==(const DataType& lhs, const DataType& rhs)
	{
		return lhs.GetSize() == rhs.GetSize() &&
			lhs.IsFloatingPoint() == rhs.IsFloatingPoint() &&
			lhs.IsUnsigned() == rhs.IsUnsigned();
	}

	bool operator!=(const DataType& lhs, const DataType& rhs)
	{
		return !(lhs == rhs);
	}
}
