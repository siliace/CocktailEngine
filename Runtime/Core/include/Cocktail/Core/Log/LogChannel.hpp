#ifndef COCKTAIL_CORE_LOG_LOGCHANNEL_HPP
#define COCKTAIL_CORE_LOG_LOGCHANNEL_HPP

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/Log/LogLevel.hpp>

namespace Ck
{
	struct LogEntry;

	/**
	 * \brief 
	 */
	class LogChannel
	{
	public:
		
		/**
		 * \brief Destructor
		 */
		virtual ~LogChannel() = default;

		/**
		 * \brief 
		 * \param entry 
		 */
		virtual void WriteEntry(const LogEntry& entry) = 0;
	};
}

#endif // COCKTAIL_CORE_LOG8LOGCHANNEL_HPP
