#ifndef COCKTAIL_CORE_LOG_CONSOLELOGCHANNEL_HPP
#define COCKTAIL_CORE_LOG_CONSOLELOGCHANNEL_HPP

#include <Cocktail/Core/Log/LogChannel.hpp>
#include <Cocktail/Core/Log/LogLevel.hpp>
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
		 * \brief Configure the colors and style of log entries written into the console
		 *
		 * \param level The LogLevel to configure
		 * \param text The color of the text
		 * \param background The color of the background
		 * \param style The style of the text
		 */
		void SetLevelColors(LogLevel level, ConsoleColor text, ConsoleColor background, ConsoleStyle style = ConsoleStyle::Normal);

	protected:

		/**
		 * \brief 
		 * \param entry 
		 */
		void WriteEntry(const LogEntry& entry) override;

	private:

		struct LevelColors
		{
			ConsoleColor Text;
			ConsoleColor Background;
			ConsoleStyle Style;
		};

		ConsoleService* mConsole;
		EnumMap<LogLevel, LevelColors> mColors;
	};
}

#endif // COCKTAIL_CORE_LOG_CONSOLELOGCHANNEL_HPP
