#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleWriter.hpp>

namespace Ck::Detail::Win32
{
	Ref<ConsoleWriter> ConsoleWriter::FromOutputHandle()
	{
		return ConsoleWriter::New(GetStdHandle(STD_OUTPUT_HANDLE));
	}

	Ref<ConsoleWriter> ConsoleWriter::FromErrorHandle()
	{
		return ConsoleWriter::New(GetStdHandle(STD_ERROR_HANDLE));
	}

	ConsoleWriter::ConsoleWriter(HANDLE handle) :
		mHandle(handle)
	{
		/// Nothing
	}

	void ConsoleWriter::Write(const char* string, std::size_t length)
	{
		if (WriteConsole(mHandle, string, length, nullptr, nullptr) == FALSE)
			throw SystemError::GetLastError();
	}

	void ConsoleWriter::Flush()
	{
	}
}
