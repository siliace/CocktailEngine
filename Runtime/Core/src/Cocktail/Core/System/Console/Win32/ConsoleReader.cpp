#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleReader.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	bool ConsoleReader::Read(TextChar& c)
	{
		return Read(&c, 1) == 1;
	}

	ConsoleReader::SizeType ConsoleReader::Read(TextChar* buffer, SizeType length)
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

	ConsoleReader::SizeType ConsoleReader::GetSize() const
	{
		throw NotImplementedException();
	}

	bool ConsoleReader::IsEof() const
	{
		throw NotImplementedException();
	}
}
