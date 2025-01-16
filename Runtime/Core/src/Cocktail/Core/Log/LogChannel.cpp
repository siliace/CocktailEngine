#include <Cocktail/Core/Log/LogChannel.hpp>
#include <Cocktail/Core/Log/LogEntry.hpp>

namespace Ck
{
	LogChannel::LogChannel(LogLevel logLevel):
		mLogLevel(logLevel)
	{
		/// Nothing
	}

	void LogChannel::Trace(const LogEntry& entry)
	{
		if (entry.Level < mLogLevel)
			return;

		WriteEntry(entry);
	}

	LogLevel LogChannel::GetLevel() const
	{
		return mLogLevel;
	}

	void LogChannel::SetLevel(LogLevel level)
	{
		mLogLevel = level;
	}
}
