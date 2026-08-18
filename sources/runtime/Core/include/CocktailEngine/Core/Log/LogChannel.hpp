#ifndef COCKTAILENGINE_CORE_LOG_LOGCHANNEL_HPP
#define COCKTAILENGINE_CORE_LOG_LOGCHANNEL_HPP

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

#endif // COCKTAILENGINE_CORE_LOG8LOGCHANNEL_HPP
