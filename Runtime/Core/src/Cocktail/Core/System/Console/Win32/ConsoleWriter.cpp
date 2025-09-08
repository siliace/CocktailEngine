#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleWriter.hpp>

namespace Ck::Detail::Win32
{
	ConsoleWriter ConsoleWriter::FromOutputHandle()
	{
		return ConsoleWriter(GetStdHandle(STD_OUTPUT_HANDLE));
	}

	ConsoleWriter ConsoleWriter::FromErrorHandle()
	{
		return ConsoleWriter(GetStdHandle(STD_ERROR_HANDLE));
	}

	ConsoleWriter::ConsoleWriter(HANDLE handle) :
		mHandle(handle)
	{
		/// Nothing
	}

	void ConsoleWriter::Write(const TextChar* string, std::size_t length)
	{
		if (WriteConsole(mHandle, string, length, nullptr, nullptr) == FALSE)
			throw SystemError::GetLastError();
	}

	void ConsoleWriter::Flush()
	{
	}

	EncodingMode ConsoleWriter::GetEncodingMode()
	{
		return EncodingMode::Utf16LittleEndian;
	}
}
