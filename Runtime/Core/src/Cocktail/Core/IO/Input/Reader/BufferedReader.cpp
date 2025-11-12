#include <cassert>
#include <cstring>

#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck
{
	BufferedReader::BufferedReader(Reader& reader, std::size_t bufferSize) :
		mIn(reader),
		mPos(0),
		mLimit(0),
		mBufferSize(bufferSize)
	{
		mLineBuffer.Reserve(1024);
		mBuffer = std::make_unique<TextChar[]>(mBufferSize);
	}

	bool BufferedReader::Read(TextChar& c)
	{
		if (mPos == mLimit && Advance() == 0)
			return false;

		c = mBuffer[mPos++];
		return true;
	}

	std::size_t BufferedReader::Read(TextChar* text, std::size_t length)
	{
		std::size_t accumulated = 0;
		while (accumulated < length)
		{
			std::size_t validLength = mLimit - mPos;
			if (validLength == 0)
			{
				validLength = Advance();
				if (validLength == 0)
					return accumulated;
			}

			std::size_t copyLength = std::min(validLength, length - accumulated);
			std::memcpy(text + accumulated, mBuffer.get() + mPos, copyLength);
			accumulated += copyLength;
			mPos += copyLength;
		}

		return accumulated;
	}

	String BufferedReader::ReadLine()
	{
		if (Optional<std::size_t> index = FindLineEnd(mLimit); !index.IsEmpty())
		{
			String string(mBuffer.get() + mPos, index.Get() - mPos);
			mPos = index.Get() + 1;

			return string;
		}

		mLineBuffer.Clear();
		mLineBuffer = String::Empty;

		bool eof = false;
		while (true)
		{
			if (mPos == mLimit)
			{
				const std::size_t advanced = Advance();
				if (advanced == 0)
				{
					eof = true;
					break;
				}
			}
			else
			{
				Optional<std::size_t> index = FindLineEnd(mLimit);
				if (!index.IsEmpty())
				{
					mLineBuffer.Append(mBuffer.get() + mPos, index.Get());
					mPos += index.Get();
					break;
				}

				const std::size_t validLength = mLimit - mPos;
				mLineBuffer.Append(mBuffer.get() + mPos, validLength);
				mPos += validLength;
			}
		}

		return mLineBuffer.IsEmpty() && eof ? String::Empty : mLineBuffer;
	}

	String BufferedReader::ReadAll()
	{
		String content;
		content.Reserve(mIn.GetSize() - mIn.Tell());

		while (!IsEof())
			content.Append(ReadLine());

		return content;
	}

	bool BufferedReader::HasCursor() const
	{
		return mIn.HasCursor();
	}

	Uint64 BufferedReader::Seek(Uint64 position)
	{
		mPos = mLimit = 0;
		return mIn.Seek(position);
	}

	void BufferedReader::Rewind()
	{
		mPos = mLimit = 0;
		mIn.Rewind();
	}

	Uint64 BufferedReader::Tell() const
	{
		return mIn.Tell();
	}

	std::size_t BufferedReader::GetSize() const
	{
		return mIn.GetSize();
	}

	bool BufferedReader::IsEof() const
	{
		return mIn.IsEof();
	}

	EncodingMode BufferedReader::GetEncodingMode() const
	{
		return mIn.GetEncodingMode();
	}

	std::size_t BufferedReader::Advance()
	{
		assert(mPos == mLimit);

		mPos = mLimit = 0;
		std::size_t count = mIn.Read(mBuffer.get(), mBufferSize);
		if (count > 0)
			mLimit = count;

		return count;
	}

	Optional<std::size_t> BufferedReader::FindLineEnd(std::size_t limit) const
	{
		for (std::size_t i = mPos; i < limit; i++)
		{
			const TextChar c = mBuffer[i];
			if (c == CK_CHAR('\n'))
				return Optional<std::size_t>::Of(i);
		}

		return Optional<std::size_t>::Empty();
	}
}
