#include <Cocktail/Core/Log/LogManager.hpp>

namespace Ck
{
	void LogManager::RegisterChannel(const std::string& name, std::unique_ptr<LogChannel> logChannel)
	{
		mChannels.insert_or_assign(name, std::move(logChannel));
	}

	const std::vector<LogEntry*>& LogManager::GetEntries() const
	{
		return mEntries;
	}

	Signal<LogEntry*>& LogManager::OnTraceEntry()
	{
		return mOnTraceEntry;
	}
}
