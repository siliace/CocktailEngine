#include <CocktailEngine/Core/Memory/Ptr.hpp>
#include <CocktailEngine/Core/Log/LogManager.hpp>

namespace Ck
{
	void LogManager::RegisterChannel(const String& name, UniquePtr<LogChannel> logChannel)
	{
		mChannels.Put(name, Move(logChannel));
	}

	void LogManager::GetEntries(Array<LogEntry*>& entries) const
	{
		entries.Clear();
		for (Ptr<LogEntry> entry : mEntries)
			entries.Add(entry.Get());
	}

	Signal<LogEntry*>& LogManager::OnTraceEntry()
	{
		return mOnTraceEntry;
	}
}
