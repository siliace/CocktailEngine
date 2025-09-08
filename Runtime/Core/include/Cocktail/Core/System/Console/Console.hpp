#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLE_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLE_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/Console/ConsoleService.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Console : public ServiceFacade<ConsoleService>
	{
	public:

		static void Write(const String& text);
		static void WriteLine(const String& text);
		static void Write(StringView text);
		static void WriteLine(StringView text);

		/**
		 * \brief 
		 * \return 
		 */
		static Writer& GetOutput();

		/**
		 * \brief
		 * \return
		 */
		static Writer& GetError();

		/**
		 * \brief
		 * \return
		 */
		static Reader& GetInput();

		/**
		 * \brief 
		 */
		static void Clear();

		/**
		 * \brief 
		 * \param background 
		 * \param foreground 
		 */
		static void SetColors(ConsoleColor background, ConsoleColor foreground);

		/**
		 * \brief
		 */
		static void Beep();

		/**
		 * \brief
		 * \param frequency
		 * \param duration
		 */
		static void Beep(unsigned int frequency, const Duration& duration);
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLE_HPP
