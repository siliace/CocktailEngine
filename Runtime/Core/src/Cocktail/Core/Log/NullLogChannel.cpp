#include <Cocktail/Core/Log/NullLogChannel.hpp>

namespace Ck
{
	NullLogChannel::NullLogChannel(LogLevel logLevel) :
		Super(logLevel)
	{
		/// Nothing
	}

	void NullLogChannel::WriteEntry(const LogEntry& entry)
	{
	}
}
