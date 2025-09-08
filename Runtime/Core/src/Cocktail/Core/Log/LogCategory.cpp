#include <Cocktail/Core/Log/LogCategory.hpp>

namespace Ck
{
	bool LogCategory::IsSuppressed(LogLevel level) const
	{
		return Enum<LogLevel>::UnderlyingCast(level) > Enum<LogLevel>::UnderlyingCast(mLogLevel);
	}

	const String& LogCategory::GetName() const
	{
		return mName;
	}

	LogLevel LogCategory::GetDefaultLevel() const
	{
		return mDefaultLogLevel;
	}

	LogLevel LogCategory::GetLevel() const
	{
		return mLogLevel;
	}

	void LogCategory::SetLevel(LogLevel level)
	{
		mLogLevel = level;
	}

	LogCategory::LogCategory(String name, LogLevel defaultLevel):
		mName(std::move(name)),
		mDefaultLogLevel(defaultLevel)
	{
		mLogLevel = mDefaultLogLevel;
	}
}
