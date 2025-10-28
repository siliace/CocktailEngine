#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Console/Unix/ConsoleReader.hpp>

#include "Cocktail/Core/Exception.hpp"

namespace Ck::Detail::Unix
{
	bool ConsoleReader::Read(TextChar& c)
	{
		return Read(&c, 1) == 1;
	}

	std::size_t ConsoleReader::Read(TextChar* buffer, std::size_t length)
	{
		return 0;
	}

	bool ConsoleReader::HasCursor() const
	{
		return false;
	}

	Uint64 ConsoleReader::Seek(Uint64 position)
	{
		throw NotImplementedException();
	}

	void ConsoleReader::Rewind()
	{
		throw NotImplementedException();
	}

	Uint64 ConsoleReader::Tell() const
	{
		throw NotImplementedException();
	}

	std::size_t ConsoleReader::GetSize() const
	{
		throw NotImplementedException();
	}

	bool ConsoleReader::IsEof() const
	{
		throw NotImplementedException();
	}

	EncodingMode ConsoleReader::GetEncodingMode() const
	{
		return EncodingMode::Utf8;
	}

}
