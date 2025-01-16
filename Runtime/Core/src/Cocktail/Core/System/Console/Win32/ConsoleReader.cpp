#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleReader.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	bool ConsoleReader::Read(char& c)
	{
		return Read(&c, 1) == 1;
	}

	std::size_t ConsoleReader::Read(char* buffer, std::size_t length)
	{
		DWORD read;
		HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);

		if (ReadConsole(handle, buffer, length, &read, nullptr) == FALSE)
			throw SystemError::GetLastError();

		return read;
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
