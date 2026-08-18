#ifndef COCKTAILENGINE_CORE_LOG_LOGMANAGER_HPP
#define COCKTAILENGINE_CORE_LOG_LOGMANAGER_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/HashMap.hpp>
#include <CocktailEngine/Core/Log/LogCategory.hpp>
#include <CocktailEngine/Core/Log/LogChannel.hpp>
#include <CocktailEngine/Core/Log/LogEntry.hpp>
#include <CocktailEngine/Core/Signal/Signal.hpp>
#include <CocktailEngine/Core/Utility/ObjectPool.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API LogManager
	{
	public:

		/**
		 * \brief 
		 */
		~LogManager();

		/**
		 * \brief
		 * \param name
		 * \param logChannel
		 * \return
		 */
		void RegisterChannel(const String& name, UniquePtr<LogChannel> logChannel);

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
			if constexpr (sizeof...(Args) > 0)
			{
				logEntry->Message = String::Format(message, Forward<Args>(args)...);
			}
			else
			{
				logEntry->Message = String::FromView(message);
			}
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

		HashMap<String, UniquePtr<LogChannel>> mChannels;
		Array<LogEntry*> mEntries;
		Signal<LogEntry*> mOnTraceEntry;
		ObjectPool<LogEntry> mEntryPool;
	};
}

#endif // COCKTAILENGINE_CORE_LOG_LOGMANAGER_HPP
