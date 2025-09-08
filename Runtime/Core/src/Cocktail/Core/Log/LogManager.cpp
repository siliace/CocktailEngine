#include <Cocktail/Core/Log/LogManager.hpp>

namespace Ck
{
	LogManager::~LogManager()
	{
		for (LogEntry* logEntry : mEntries)
			mEntryPool.Recycle(logEntry);
	}

	void LogManager::RegisterChannel(const String& name, std::unique_ptr<LogChannel> logChannel)
	{
		mChannels.insert_or_assign(name, std::move(logChannel));
	}

	const Array<LogEntry*>& LogManager::GetEntries() const
	{
		return mEntries;
	}

	Signal<LogEntry*>& LogManager::OnTraceEntry()
	{
		return mOnTraceEntry;
	}
}
