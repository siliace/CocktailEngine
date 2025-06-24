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

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API LogManager
	{
	public:

		/**
		 * \brief
		 * \tparam T
		 * \tparam Args
		 * \param name
		 * \param logChannel
		 * \return
		 */
		void RegisterChannel(const std::string& name, std::unique_ptr<LogChannel> logChannel);

		/**
		 * \brief 
		 * \param category 
		 * \param level 
		 * \param message 
		 * \param file 
		 * \param line 
		 * \param args 
		 */
		template <typename... Args>
		void Trace(const LogCategory& category, LogLevel level, std::string_view message, std::string_view file, Uint64 line, Args&&... args)
		{
			if (category.IsSuppressed(level))
				return;

			LogEntry* logEntry = mEntryPool.AllocateUnsafe();
			logEntry->Category = &category;
			logEntry->Message = fmt::format(message, std::forward<Args>(args)...);
			logEntry->Level = level;
			logEntry->File = file;
			logEntry->Line = line;

			for (const auto& [name, channel] : mChannels)
				channel->WriteEntry(*logEntry);

			mEntries.push_back(logEntry);

			mOnTraceEntry.Emit(logEntry);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const std::vector<LogEntry*>& GetEntries() const;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<LogEntry*>& OnTraceEntry();

	private:

		std::unordered_map<std::string, std::unique_ptr<LogChannel>> mChannels;
		std::vector<LogEntry*> mEntries;
		Signal<LogEntry*> mOnTraceEntry;
		ObjectPool<LogEntry> mEntryPool;
	};
}

#endif // COCKTAIL_CORE_LOG_LOGMANAGER_HPP
