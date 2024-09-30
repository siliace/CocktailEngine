#ifndef COCKTAIL_CORE_LOG_NULLLOGCHANNEL_HPP
#define COCKTAIL_CORE_LOG_NULLLOGCHANNEL_HPP

#include <Cocktail/Core/Log/LogChannel.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class NullLogChannel : public Extends<NullLogChannel, LogChannel>
	{
	public:

		/**
		 * \brief 
		 * \param logLevel 
		 */
		explicit NullLogChannel(LogLevel logLevel = LogLevel::None);

	protected:

		/**
		 * \brief 
		 * \param entry
		 */
		void WriteEntry(const LogEntry& entry) override;
	};
}

#endif // COCKTAIL_CORE_LOG_NULLLOGCHANNEL_HPP
