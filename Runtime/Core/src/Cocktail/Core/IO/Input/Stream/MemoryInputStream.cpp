#include <algorithm>
#include <cstring>

#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>

namespace Ck
{
	MemoryInputStream::MemoryInputStream(const void* data, std::size_t length) :
		mData(static_cast<const unsigned char*>(data)),
		mSize(length),
		mCursor(0)
	{
		/// Nothing
	}

	bool MemoryInputStream::HasCursor() const
	{
		return true;
	}

	std::size_t MemoryInputStream::Read(void* data, std::size_t length)
	{
		std::size_t available = mSize - mCursor;
		std::size_t copyLength = std::min(available, length);

		std::memcpy(data, mData + mCursor, copyLength);

		mCursor += copyLength;

		return copyLength;
	}

	Uint64 MemoryInputStream::Seek(Uint64 position)
	{
		mCursor = std::min(position, mSize);

		return mCursor;
	}

	Uint64 MemoryInputStream::Tell() const
	{
		return mCursor;
	}

	void MemoryInputStream::Rewind()
	{
		mCursor = 0;
	}

	std::size_t MemoryInputStream::GetSize() const
	{
		return mSize;
	}

	bool MemoryInputStream::IsEof() const
	{
		return mCursor == mSize;
	}
}
