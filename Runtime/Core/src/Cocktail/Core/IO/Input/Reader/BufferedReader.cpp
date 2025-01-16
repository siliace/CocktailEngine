#include <cassert>
#include <cstring>

#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>

namespace Ck
{
	BufferedReader::BufferedReader(Reader& reader, std::size_t bufferSize) :
		mIn(reader),
		mPos(0),
		mLimit(0),
		mBufferSize(bufferSize)
	{
		mLineBuffer.reserve(1024);
		mBuffer = std::make_unique<char[]>(mBufferSize);
	}

	bool BufferedReader::Read(char& c)
	{
		if (mPos == mLimit && Advance() == 0)
			return false;

		c = mBuffer[mPos++];
		return true;
	}

	std::size_t BufferedReader::Read(char* buffer, std::size_t length)
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
			std::memcpy(buffer + accumulated, mBuffer.get() + mPos, copyLength);
			accumulated += copyLength;
			mPos += copyLength;
		}

		return accumulated;
	}

	std::string BufferedReader::ReadLine()
	{
		if (const std::size_t i = FindLineEnd(mLimit); i != std::string::npos)
		{
			std::string string(mBuffer.get() + mPos, i - mPos);
			mPos = i + 1;

			return string;
		}

		mLineBuffer.clear();

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
				std::size_t i = FindLineEnd(mLimit);
				if (i != std::string::npos)
				{
					mLineBuffer.append(mBuffer.get() + mPos, i);
					mPos += i;
					break;
				}

				const std::size_t validLength = mLimit - mPos;
				mLineBuffer.append(mBuffer.get() + mPos, validLength);
				mPos += validLength;
			}
		}

		return mLineBuffer.length() == 0 && eof ? "" : mLineBuffer;
	}

	std::string BufferedReader::ReadAll()
	{
		std::string content;
		content.reserve(mIn.GetSize() - mIn.Tell());

		while (!IsEof())
			content.append(ReadLine());

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

	std::size_t BufferedReader::Advance()
	{
		assert(mPos == mLimit);

		mPos = mLimit = 0;
		std::size_t count = mIn.Read(mBuffer.get(), mBufferSize);
		if (count > 0)
			mLimit = count;

		return count;
	}

	std::size_t BufferedReader::FindLineEnd(std::size_t limit) const
	{
		for (std::size_t i = mPos; i < limit; i++)
		{
			const char c = mBuffer[i];
			if (c == '\n')
				return i;
		}

		return std::string::npos;
	}
}
