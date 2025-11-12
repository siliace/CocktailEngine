#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLESERVICE_HPP

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>
#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>
#include <Cocktail/Core/System/Console/ConsoleColor.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class ConsoleService
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~ConsoleService() = default;

		virtual void Write(const String& text) = 0;
		virtual void WriteLine(const String& text) = 0;

		virtual void Write(StringView text) = 0;
		virtual void WriteLine(StringView text) = 0;

		/**
		 * \brief
		 * \return 
		 */
		virtual Writer& GetOutput() = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Writer& GetError() = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Reader& GetInput() = 0;

		/**
		 * \brief 
		 */
		virtual void Clear() = 0;

		/**
		 * \brief 
		 * \param background 
		 * \param foreground 
		 */
		virtual void SetColors(ConsoleColor background, ConsoleColor foreground) = 0;

		/**
		 * \brief 
		 */
		virtual void Beep() const = 0;

		/**
		 * \brief 
		 * \param frequency 
		 * \param duration 
		 */
		virtual void Beep(unsigned int frequency, const Duration& duration) const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLESERVICE_HPP
