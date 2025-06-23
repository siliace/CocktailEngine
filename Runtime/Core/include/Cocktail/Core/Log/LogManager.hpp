#ifndef COCKTAIL_CORE_LOG_LOGMANAGER_HPP
#define COCKTAIL_CORE_LOG_LOGMANAGER_HPP

#include <memory>
#include <unordered_map>

#include <fmt/core.h>

#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/Log/LogCategory.hpp>
#include <Cocktail/Core/Log/LogChannel.hpp>
#include <Cocktail/Core/Log/LogEntry.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/Utility/ObjectPool.hpp>

#define CK_LOG(__Category, __Level, __Message, ...) Log::Trace(__Category, __Level, __Message, __FILE__, __LINE__, __VA_ARGS__);

namespace Ck
{
	class COCKTAIL_CORE_API LogManager
	{
	public:

		/**
		 * \brief 
		 */
		LogManager();

		/**
		 * \brief 
		 * \tparam Args 
		 * \param level 
		 * \param message 
		 * \param args 
		 */
		template <typename... Args>
		void Trace(const BaseLogCategory& category, LogLevel level, std::string_view message, std::string_view file, Uint64 line, Args&&... args)
		{
			if (category.IsSuppressed(level))
				return;

			LogEntry* logEntry = mEntryPool.AllocateUnsafe();
			for (const auto& [name, channel] : mChannels)
				channel->WriteEntry(*logEntry);

			mEntries.push_back(logEntry);

			mOnTraceEntry.Emit(logEntry);
		}

		/**
		 * \brief
		 * \tparam T
		 * \tparam Args
		 * \param name
		 * \param logChannel
		 * \return
		 */
		void RegisterChannel(const std::string& name, std::unique_ptr<LogChannel> logChannel);

		Signal<LogEntry*>& OnTraceEntry()
		{
			return mOnTraceEntry;
		}

	private:

		std::unordered_map<std::string, std::unique_ptr<LogChannel>> mChannels;
		std::vector<LogEntry*> mEntries;
		Signal<LogEntry*> mOnTraceEntry;
		ObjectPool<LogEntry> mEntryPool;
	};
}

#endif // COCKTAIL_CORE_LOG_LOGMANAGER_HPP
