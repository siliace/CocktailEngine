#include <utility>

#include <Cocktail/Core/Utility/ByteArray.hpp>
#include <Cocktail/Core/Utility/ByteArrayView.hpp>

namespace Ck
{
	ByteArrayView::ByteArrayView() :
		mSize(0),
		mData(nullptr)
	{
		/// Nothing
	}

	ByteArrayView::ByteArrayView(const Byte* data, std::size_t size) :
		mData(data),
		mSize(size)
	{
		assert(mData != nullptr);
	}

	ByteArrayView::ByteArrayView(const ByteArray& byteArray, std::size_t offset) :
		ByteArrayView(byteArray, offset, byteArray.GetSize() - offset)
	{
		/// Nothing
	}

	ByteArrayView::ByteArrayView(const ByteArray& byteArray, std::size_t offset, std::size_t length)
	{
		assert(offset + length <= byteArray.GetSize());

		mSize = length;
		mData = byteArray.GetData() + offset;
	}

	ByteArrayView::ByteArrayView(ByteArrayView&& other) noexcept
	{
		*this = std::move(other);
	}

	ByteArrayView& ByteArrayView::operator=(ByteArrayView&& other) noexcept
	{
		mSize = std::exchange(other.mSize, 0);
		mData = std::exchange(other.mData, nullptr);

		return *this;
	}

	ByteArrayView ByteArrayView::Slice(std::size_t offset) const
	{
		return Slice(offset, mSize - offset);
	}

	ByteArrayView ByteArrayView::Slice(std::size_t offset, std::size_t length) const
	{
		assert(offset + length < mSize);

		ByteArrayView view;
		view.mSize = length;
		view.mData = mData + offset;

		return view;
	}

	const Byte& ByteArrayView::At(std::size_t index) const
	{
		assert(index < mSize);
		return mData[index];
	}

	bool ByteArrayView::IsEmpty() const
	{
		return mSize == 0;
	}

	std::size_t ByteArrayView::GetSize() const
	{
		return mSize;
	}

	const Byte* ByteArrayView::GetData() const
	{
		return mData;
	}

	bool ByteArrayView::operator==(const ByteArray& rhs) const
	{
		return *this == ByteArrayView(rhs);
	}

	bool ByteArrayView::operator==(const ByteArrayView& rhs) const
	{
		if (GetSize() != rhs.GetSize())
			return false;

		for (std::size_t i = 0; i < GetSize(); i++)
		{
			if (At(i) != rhs.At(i))
				return false;
		}

		return true;
	}

	bool ByteArrayView::operator!=(const ByteArray& rhs) const
	{
		return !(*this == rhs);
	}

	bool ByteArrayView::operator!=(const ByteArrayView& rhs) const
	{
		return !(*this == rhs);
	}
}
