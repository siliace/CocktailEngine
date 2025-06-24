#ifndef COCKTAIL_CORE_LOG_LOGENTRY_HPP
#define COCKTAIL_CORE_LOG_LOGENTRY_HPP

#include <string>

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Log/LogLevel.hpp>

namespace Ck
{
	class LogCategory;

	/**
	 * \brief Structure containing information about a log message
	 */
	struct LogEntry
	{
		/**
		 * \brief The LogCategory that emitted this entry
		 */
		const LogCategory* Category = nullptr;

		/**
		 * \brief The formatted message of the entry 
		 */
		std::string Message;

		/**
		 * \brief The level of the entry
		 */
		LogLevel Level = LogLevel::None;
		
		/**
		 * \brief The source file where the entry has been emitted
		 */
		std::string_view File;
		
		/**
		 * \brief The line where the entry has been emitted
		 */
		Uint64 Line;
	};
}

#endif // COCKTAIL_CORE_LOG_LOGENTRY_HPP
