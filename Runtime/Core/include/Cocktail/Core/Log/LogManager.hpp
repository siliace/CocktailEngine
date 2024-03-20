#ifndef COCKTAIL_CORE_LOG_LOGMANAGER_HPP
#define COCKTAIL_CORE_LOG_LOGMANAGER_HPP

#include <unordered_map>

#include <fmt/core.h>

#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/Log/LogChannel.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API LogManager : public Extends<LogManager, Object>
	{
	public:

		/**
		 * \brief 
		 */
		LogManager();
		
		/**
		 * \brief
		 * \tparam Args
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		void Debug(std::string_view message, Args&&... args)
		{
			Trace(LogLevel::Debug, message, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 * \tparam Args
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		void Info(std::string_view message, Args&&... args)
		{
			Trace(LogLevel::Info, message, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 * \tparam Args
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		void Warning(std::string_view message, Args&&... args)
		{
			Trace(LogLevel::Warning, message, std::forward<Args>(args)...);
		}
		
		/**
		 * \brief 
		 * \tparam Args 
		 * \param message 
		 * \param args 
		 */
		template <typename... Args>
		void Error(std::string_view message, Args&&... args)
		{
			Trace(LogLevel::Error, message, std::forward<Args>(args)...);
		}

		/**
		 * \brief
		 * \tparam Args
		 * \param message
		 * \param args
		 */
		template <typename... Args>
		void Critical(std::string_view message, Args&&... args)
		{
			Trace(LogLevel::Critical, message, std::forward<Args>(args)...);
		}

		/**
		 * \brief 
		 * \tparam Args 
		 * \param level 
		 * \param message 
		 * \param args 
		 */
		template <typename... Args>
		void Trace(LogLevel level, std::string_view message, Args&&... args)
		{
			if (Enum<LogLevel>::UnderlyingCast(level) >= Enum<LogLevel>::UnderlyingCast(mLevel))
			{
				LogEntry entry = CreateEntry(level, message, std::forward<Args>(args)...);
				for (const auto& [name, channel] : mChannels)
					channel->Trace(entry);
			}
		}

		/**
		 * \brief 
		 * \tparam T 
		 * \tparam Args 
		 * \param name 
		 * \param args 
		 * \return 
		 */
		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<LogChannel, T>>>
		Ref<T> CreateChannel(const std::string& name, Args&&... args)
		{
			Ref<T> channel = T::New(std::forward<Args>(args)...);
			RegisterChannel(name, channel);

			return channel;
		}

		/**
		 * \brief
		 * \tparam T
		 * \tparam Args
		 * \param name
		 * \param logChannel
		 * \return
		 */
		void RegisterChannel(const std::string& name, Ref<LogChannel> logChannel);

		/**
		 * \brief 
		 * \return 
		 */
		LogLevel GetLevel() const;

		/**
		 * \brief 
		 * \param level 
		 */
		void SetLevel(LogLevel level);

	private:

		/**
		 * \brief 
		 * \tparam Args 
		 * \param level 
		 * \param message 
		 * \param args 
		 * \return 
		 */
		template <typename... Args>
		static LogEntry CreateEntry(LogLevel level, std::string_view message, Args&&... args)
		{
			LogEntry entry;
			entry.Message = fmt::format(message, std::forward<Args>(args)...) + '\n';
			entry.Level = level;

			return entry;
		}

		LogLevel mLevel;
		std::unordered_map<std::string, Ref<LogChannel>> mChannels;
	};
}

#endif // COCKTAIL_CORE_LOG_LOGMANAGER_HPP
