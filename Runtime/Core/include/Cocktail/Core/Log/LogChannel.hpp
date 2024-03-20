#ifndef COCKTAIL_CORE_LOG_LOGCHANNEL_HPP
#define COCKTAIL_CORE_LOG_LOGCHANNEL_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Log/LogEntry.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API LogChannel : public Extends<LogChannel, Object>
	{
	public:

		/**
		 * \brief 
		 * \param logLevel 
		 */
		explicit LogChannel(LogLevel logLevel = LogLevel::None);

		/**
		 * \brief 
		 * \param entry 
		 */
		void Trace(const LogEntry& entry);

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

	protected:


		/**
		 * \brief 
		 * \param entry 
		 */
		virtual void WriteEntry(const LogEntry& entry) = 0;

	private:

		LogLevel mLogLevel;
	};
}

#endif // COCKTAIL_CORE_LOG8LOGCHANNEL_HPP
