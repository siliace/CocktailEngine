#ifndef COCKTAIL_CORE_LOG_CONSOLELOGCHANNEL_HPP
#define COCKTAIL_CORE_LOG_CONSOLELOGCHANNEL_HPP

#include <Cocktail/Core/Log/LogChannel.hpp>
#include <Cocktail/Core/System/Console/Console.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API ConsoleLogChannel : public LogChannel
	{
	public:

		/**
		 * \brief 
		 * \param console 
		 */
		explicit ConsoleLogChannel(ConsoleService* console);

		/**
		 * \brief 
		 * \param level 
		 * \param background 
		 * \param foreground 
		 */
		void SetLevelColors(LogLevel level, ConsoleColor background, ConsoleColor foreground);

	protected:

		/**
		 * \brief 
		 * \param entry 
		 */
		void WriteEntry(const LogEntry& entry) override;

	private:

		ConsoleService* mConsole;
		EnumMap<LogLevel, std::pair<ConsoleColor, ConsoleColor>> mColors;
	};
}

#endif // COCKTAIL_CORE_LOG_CONSOLELOGCHANNEL_HPP
