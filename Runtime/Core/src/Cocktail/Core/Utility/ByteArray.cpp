#include <utility>

#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
	ByteArray::ByteArray() :
		mSize(0),
		mData(nullptr)
	{
		/// Nothing
	}

	ByteArray::ByteArray(std::size_t size, Uint8 value) :
		mSize(size)
	{
		assert(mSize);

		mData = MakeUnique<Uint8[]>(mSize);
		std::memset(mData.Get(), value, mSize);
	}

    ByteArray::ByteArray(const ByteArray& other) :
        mSize(0)
	{
		*this = other;
	}

	ByteArray::ByteArray(ByteArray&& other) noexcept :
        mSize(0)
	{
		*this = std::move(other);
	}

	ByteArray& ByteArray::operator=(const ByteArray& other)
	{
		mSize = other.mSize;
		mData.Reset();

		if (mSize)
		{
			mData = MakeUnique<Uint8[]>(mSize);
			std::memcpy(mData.Get(), other.GetData(), mSize);
		}

		return *this;
	}

	ByteArray& ByteArray::operator=(ByteArray&& other) noexcept
	{
		mSize = std::exchange(other.mSize, 0);
		mData = std::move(other.mData);

		return *this;
	}

	ByteArray& ByteArray::Append(const ByteArray& other)
	{
		return Insert(mSize, other);
	}

	ByteArray& ByteArray::Append(const ByteArrayView& other)
	{
		return Insert(mSize, other);
	}

	ByteArray& ByteArray::Append(const void* data, std::size_t length)
	{
		return Insert(mSize, data, length);
	}

	ByteArray& ByteArray::Prepend(const ByteArray& other)
	{
		return Insert(0, other);
	}

	ByteArray& ByteArray::Prepend(const ByteArrayView& other)
	{
		return Insert(0, other);
	}

	ByteArray& ByteArray::Prepend(const void* data, std::size_t length)
	{
		return Insert(0, data, length);
	}

	ByteArray& ByteArray::Insert(std::size_t where, const ByteArray& other)
	{
		return Insert(where, other.GetData(), other.GetSize());
	}

	ByteArray& ByteArray::Insert(std::size_t where, const ByteArrayView& other)
	{
		return Insert(where, other.GetData(), other.GetSize());
	}

	ByteArray& ByteArray::Insert(std::size_t where, const void* data, std::size_t length)
	{
		assert(where <= mSize);
		assert(data != nullptr);
		assert(length > 0);

		std::size_t fullSize = mSize + length;
		UniquePtr<Uint8[]> fullData = MakeUnique<Uint8[]>(fullSize);

		if (mSize)
		{
			if (where == 0)
			{
				std::memcpy(fullData.Get(), data, length);
				std::memcpy(fullData.Get() + length, mData.Get(), mSize);
			}
			else if (where == mSize)
			{
				std::memcpy(fullData.Get(), mData.Get(), mSize);
				std::memcpy(fullData.Get() + mSize, data, length);
			}
			else
			{
				std::size_t offset = 0;
				std::memcpy(fullData.Get() + offset, mData.Get(), where);

				offset += where;
				std::memcpy(fullData.Get() + offset, data, length);

				offset += length;
				std::memcpy(fullData.Get() + offset, mData.Get() + where, mSize - where);
			}
		}
		else
		{
			std::memcpy(fullData.Get(), data, fullSize);
		}

		mSize = fullSize;
		mData = std::move(fullData);

		return *this;
	}

	ByteArray ByteArray::Slice(std::size_t offset) const
	{
		return Slice(offset, mSize - offset);
	}

	ByteArray ByteArray::Slice(std::size_t offset, std::size_t length) const
	{
		assert(offset + length < mSize);
		return ByteArray(mData.Get() + offset, length);
	}

	ByteArray& ByteArray::Remove(std::size_t offset)
	{
		return Remove(offset, mSize - offset);
	}

	ByteArray& ByteArray::Remove(std::size_t offset, std::size_t length)
	{
		assert(offset + length < mSize);

		std::size_t fullSize = mSize - length;
		UniquePtr<Uint8[]> fullData = MakeUnique<Uint8[]>(fullSize);

		if (offset == 0)
		{
			std::memcpy(fullData.Get(), mData.Get() + length, fullSize);
		}
		else if (offset + length == mSize - 1)
		{
			std::memcpy(fullData.Get(), mData.Get(), fullSize);
		}
		else
		{
			std::memcpy(fullData.Get(), mData.Get(), offset);
			std::memcpy(fullData.Get() + offset, mData.Get() + offset + length, fullSize - offset);
		}

		mSize = fullSize;
		mData = std::move(fullData);

		return *this;
	}

	void ByteArray::Resize(std::size_t newSize, Uint8 value)
	{
		if (newSize == mSize)
			return;

		UniquePtr<Uint8[]> newData = MakeUnique<Uint8[]>(newSize);
		if (newSize > mSize)
		{
			std::memcpy(newData.Get(), mData.Get(), mSize);
			std::memset(newData.Get() + mSize, value, newSize - mSize);
		}
		else
		{
			std::memcpy(newData.Get(), mData.Get(), newSize);
		}

		mSize = newSize;
		mData = std::move(newData);
	}

	Uint8& ByteArray::At(std::size_t index)
	{
		assert(index < mSize);
		return mData[index];
	}

	const Uint8& ByteArray::At(std::size_t index) const
	{
		assert(index < mSize);
		return mData[index];
	}

	bool ByteArray::IsEmpty() const
	{
		return mSize == 0;
	}

	std::size_t ByteArray::GetSize() const
	{
		return mSize;
	}

	Uint8* ByteArray::GetData()
	{
		return mData.Get();
	}

	const Uint8* ByteArray::GetData() const
	{
		return mData.Get();
	}

	bool ByteArray::operator==(const ByteArray& rhs) const
	{
		return *this == ByteArrayView(rhs);
	}

	bool ByteArray::operator==(const ByteArrayView& rhs) const
	{
		return ByteArrayView(*this) == ByteArrayView(rhs);
	}

	bool ByteArray::operator!=(const ByteArray& rhs) const
	{
		return !(*this == rhs);
	}

	bool ByteArray::operator!=(const ByteArrayView& rhs) const
	{
		return !(*this == rhs);
	}
}
