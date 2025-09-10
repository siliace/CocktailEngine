#ifndef COCKTAIL_CORE_LOG_LOGMANAGER_HPP
#define COCKTAIL_CORE_LOG_LOGMANAGER_HPP

#include <memory>
#include <unordered_map>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/StringView.hpp>
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
		 */
		~LogManager();

		/**
		 * \brief
		 * \tparam T
		 * \tparam Args
		 * \param name
		 * \param logChannel
		 * \return
		 */
		void RegisterChannel(const String& name, std::unique_ptr<LogChannel> logChannel);

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
		void Trace(const LogCategory& category, LogLevel level, StringView message, StringView file, Uint64 line, Args&&... args)
		{
			if (category.IsSuppressed(level))
				return;

			LogEntry* logEntry = mEntryPool.AllocateUnsafe();
			logEntry->Category = &category;
			logEntry->Message = CK_TEXT(""); // TODO: fmt::format(message, std::forward<Args>(args)...);
			logEntry->Level = level;
			logEntry->File = file;
			logEntry->Line = line;

			for (const auto& [name, channel] : mChannels)
				channel->WriteEntry(*logEntry);

			mEntries.Add(logEntry);

			mOnTraceEntry.Emit(logEntry);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const Array<LogEntry*>& GetEntries() const;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<LogEntry*>& OnTraceEntry();

	private:

		std::unordered_map<String, std::unique_ptr<LogChannel>> mChannels;
		Array<LogEntry*> mEntries;
		Signal<LogEntry*> mOnTraceEntry;
		ObjectPool<LogEntry> mEntryPool;
	};
}

#endif // COCKTAIL_CORE_LOG_LOGMANAGER_HPP
