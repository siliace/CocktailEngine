#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Console/Unix/ConsoleReader.hpp>

namespace Ck::Detail::Unix
{
	bool ConsoleReader::Read(char& c)
	{
		return Read(&c, 1) == 1;
	}

	std::size_t ConsoleReader::Read(char* buffer, std::size_t length)
	{
		return 0;
	}

	bool ConsoleReader::HasCursor() const
	{
		return false;
	}

	Uint64 ConsoleReader::Seek(Uint64 position)
	{
		/// Todo: create an exception here ?
		throw std::runtime_error("");
	}

	void ConsoleReader::Rewind()
	{
		/// Todo: create an exception here ?
		throw std::runtime_error("");
	}

	Uint64 ConsoleReader::Tell() const
	{
		/// Todo: create an exception here ?
		throw std::runtime_error("");
	}

	std::size_t ConsoleReader::GetSize() const
	{
		return 0;
	}

	bool ConsoleReader::IsEof() const
	{
		return false;
	}
}
