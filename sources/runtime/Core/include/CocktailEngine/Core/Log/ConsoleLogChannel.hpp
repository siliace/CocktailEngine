#ifndef COCKTAILENGINE_CORE_LOG_CONSOLELOGCHANNEL_HPP
#define COCKTAILENGINE_CORE_LOG_CONSOLELOGCHANNEL_HPP

#include <CocktailEngine/Core/IO/Output/Writer/LineWriter.hpp>
#include <CocktailEngine/Core/Log/LogChannel.hpp>
#include <CocktailEngine/Core/Log/LogLevel.hpp>
#include <CocktailEngine/Core/System/Console/Console.hpp>
#include <CocktailEngine/Core/Utility/EnumMap.hpp>

namespace Ck
{

	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API ConsoleLogChannel : public LogChannel
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
	    LineWriter<> mLineWriter;
		EnumMap<LogLevel, LevelColors> mColors;
	};
}

#endif // COCKTAILENGINE_CORE_LOG_CONSOLELOGCHANNEL_HPP
