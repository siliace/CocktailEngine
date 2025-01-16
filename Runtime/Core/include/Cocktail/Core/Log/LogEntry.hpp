#ifndef COCKTAIL_CORE_LOG_LOGENTRY_HPP
#define COCKTAIL_CORE_LOG_LOGENTRY_HPP

#include <string>

#include <Cocktail/Core/Log/LogLevel.hpp>

namespace Ck
{
	/**
	 * \brief Structure containing information about a log message
	 */
	struct LogEntry
	{
		/**
		 * \brief The formatted message of the entry 
		 */
		std::string Message;

		/**
		 * \brief The level of the entry
		 */
		LogLevel Level = LogLevel::None;
	};
}

#endif // COCKTAIL_CORE_LOG_LOGENTRY_HPP
