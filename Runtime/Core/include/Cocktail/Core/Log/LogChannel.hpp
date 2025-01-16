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
	class COCKTAIL_CORE_API LogChannel
	{
	public:

		/**
		 * \brief 
		 * \param logLevel 
		 */
		explicit LogChannel(LogLevel logLevel = LogLevel::None);

		/**
		 * \brief Destructor
		 */
		virtual ~LogChannel() = default;

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
