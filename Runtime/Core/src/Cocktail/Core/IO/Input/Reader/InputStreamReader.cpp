#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>

namespace Ck
{
	InputStreamReader::InputStreamReader(InputStream& inputStream):
		mInputStream(inputStream)
	{
		/// Nothing
	}

	bool InputStreamReader::Read(char& c)
	{
		return mInputStream.Read(&c, sizeof(char)) == sizeof(char);
	}

	std::size_t InputStreamReader::Read(char* buffer, std::size_t length)
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
