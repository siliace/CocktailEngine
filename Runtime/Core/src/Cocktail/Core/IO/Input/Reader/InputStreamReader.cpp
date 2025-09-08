#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>

namespace Ck
{
	InputStreamReader::InputStreamReader(InputStream& inputStream, EncodingMode encodingMode) :
		mInputStream(inputStream),
		mEncodingMode(encodingMode)
	{
		/// Nothing
	}

	bool InputStreamReader::Read(TextChar& c)
	{
		return mInputStream.Read(&c, sizeof(TextChar)) == sizeof(TextChar);
	}

	std::size_t InputStreamReader::Read(TextChar* buffer, std::size_t length)
	{
		return mInputStream.Read(buffer, length);
	}

	bool InputStreamReader::HasCursor() const
	{
		return mInputStream.HasCursor();
	}

	Uint64 InputStreamReader::Seek(Uint64 position)
	{
		return mInputStream.Seek(position);
	}

	void InputStreamReader::Rewind()
	{
		mInputStream.Rewind();
	}

	Uint64 InputStreamReader::Tell() const
	{
		return mInputStream.Tell();
	}

	std::size_t InputStreamReader::GetSize() const
	{
		return mInputStream.GetSize();
	}

	bool InputStreamReader::IsEof() const
	{
		return mInputStream.IsEof();
	}
}
