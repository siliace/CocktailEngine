#ifndef COCKTAIL_CORE_LOG_LOGMANAGER_HPP
#define COCKTAIL_CORE_LOG_LOGMANAGER_HPP

#include <memory>
#include <unordered_map>

#include <fmt/core.h>

#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/Log/LogChannel.hpp>
#include <Cocktail/Core/Log/LogEntry.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API LogManager
	{
	public:

		/**
		 * \brief 
		 */
		LogManager();

		LogManager(const LogManager& other) = delete;
		LogManager(LogManager&& other) noexcept = default;

		LogManager& operator=(const LogManager& other) = delete;
		LogManager& operator=(LogManager&& other) noexcept = default;

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
		 * \param logChannel
		 * \return
		 */
		void RegisterChannel(const std::string& name, std::unique_ptr<LogChannel> logChannel);

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
		std::unordered_map<std::string, std::unique_ptr<LogChannel>> mChannels;
	};
}

#endif // COCKTAIL_CORE_LOG_LOGMANAGER_HPP
