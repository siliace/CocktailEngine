#include <unistd.h>

#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Console/Unix/ConsoleWriter.hpp>

namespace Ck::Detail::Unix
{
	ConsoleWriter ConsoleWriter::FromOutputHandle()
	{
		return ConsoleWriter(STDOUT_FILENO);
	}

	ConsoleWriter ConsoleWriter::FromErrorHandle()
	{
		return ConsoleWriter(STDERR_FILENO);
	}

	ConsoleWriter::ConsoleWriter(int handle) :
		mHandle(handle)
	{
		/// Nothing
	}

	void ConsoleWriter::Write(const char* string, std::size_t length)
	{
		int err = ::write(mHandle, string, length);
		if (err == -1)
			throw SystemError::GetLastError();
	}

	void ConsoleWriter::Flush()
	{
		fflush(stdout);
	}
}
