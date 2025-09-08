#include <utility>

#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
	ByteArray ByteArray::FromString(const String& string)
	{
		assert(!string.IsEmpty());
		return ByteArray(string.GetData(), string.GetLength());
	}

	ByteArray ByteArray::FromString(StringView string)
	{
		assert(!string.IsEmpty());
		return ByteArray(string.GetData(), string.GetLength());
	}

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

		mData = std::make_unique<Uint8[]>(mSize);
		std::memset(mData.get(), value, mSize);
	}

	ByteArray::ByteArray(const ByteArray& other)
	{
		*this = other;
	}

	ByteArray::ByteArray(ByteArray&& other) noexcept
	{
		*this = std::move(other);
	}

	ByteArray& ByteArray::operator=(const ByteArray& other)
	{
		mSize = other.mSize;
		mData = nullptr;

		if (mSize)
		{
			mData = std::make_unique<Uint8[]>(mSize);
			std::memcpy(mData.get(), other.GetData(), mSize);
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
		std::unique_ptr<Uint8[]> fullData = std::make_unique<Uint8[]>(fullSize);

		if (mSize)
		{
			if (where == 0)
			{
				std::memcpy(fullData.get(), data, length);
				std::memcpy(fullData.get() + length, mData.get(), mSize);
			}
			else if (where == mSize)
			{
				std::memcpy(fullData.get(), mData.get(), mSize);
				std::memcpy(fullData.get() + mSize, data, length);
			}
			else
			{
				std::size_t offset = 0;
				std::memcpy(fullData.get() + offset, mData.get(), where);

				offset += where;
				std::memcpy(fullData.get() + offset, data, length);

				offset += length;
				std::memcpy(fullData.get() + offset, mData.get() + where, mSize - where);
			}
		}
		else
		{
			std::memcpy(fullData.get(), data, fullSize);
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
		return ByteArray(mData.get() + offset, length);
	}

	ByteArray& ByteArray::Remove(std::size_t offset)
	{
		return Remove(offset, mSize - offset);
	}

	ByteArray& ByteArray::Remove(std::size_t offset, std::size_t length)
	{
		assert(offset + length < mSize);

		std::size_t fullSize = mSize - length;
		std::unique_ptr<Uint8[]> fullData = std::make_unique<Uint8[]>(fullSize);

		if (offset == 0)
		{
			std::memcpy(fullData.get(), mData.get() + length, fullSize);
		}
		else if (offset + length == mSize - 1)
		{
			std::memcpy(fullData.get(), mData.get(), fullSize);
		}
		else
		{
			std::memcpy(fullData.get(), mData.get(), offset);
			std::memcpy(fullData.get() + offset, mData.get() + offset + length, fullSize - offset);
		}

		mSize = fullSize;
		mData = std::move(fullData);

		return *this;
	}

	void ByteArray::Resize(std::size_t newSize, Uint8 value)
	{
		if (newSize == mSize)
			return;

		std::unique_ptr<Uint8[]> newData = std::make_unique<Uint8[]>(newSize);
		if (newSize > mSize)
		{
			std::memcpy(newData.get(), mData.get(), mSize);
			std::memset(newData.get() + mSize, value, newSize - mSize);
		}
		else
		{
			std::memcpy(newData.get(), mData.get(), newSize);
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

	String ByteArray::ToString() const
	{
		return String(
			reinterpret_cast<const TextChar*>(mData.get()),
			mSize / sizeof(TextChar)
		);
	}

	std::size_t ByteArray::GetSize() const
	{
		return mSize;
	}

	Uint8* ByteArray::GetData()
	{
		return mData.get();
	}

	const Uint8* ByteArray::GetData() const
	{
		return mData.get();
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
