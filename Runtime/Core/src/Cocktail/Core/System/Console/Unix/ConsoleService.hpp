#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLESERVICE_HPP

#include <Cocktail/Core/System/Console/ConsoleService.hpp>
#include <Cocktail/Core/System/Console/Unix/ConsoleReader.hpp>
#include <Cocktail/Core/System/Console/Unix/ConsoleWriter.hpp>

namespace Ck::Detail::Unix
{
	/**
	 * \brief 
	 */
	class ConsoleService final : public Ck::ConsoleService
	{
	public:

		/**
		 * \brief 
		 */
		ConsoleService();
		
		/**
		 * \brief 
		 * \return 
		 */
		Writer& GetOutput() override;

		/**
		 * \brief
		 * \return
		 */
		Writer& GetError() override;

		/**
		 * \brief 
		 * \return 
		 */
		Reader& GetInput() override;

		/**
		 * \brief 
		 */
		void Clear() override;

		/**
		 * \brief 
		 * \param background 
		 * \param foreground 
		 */
		void SetColors(ConsoleColor background, ConsoleColor foreground) override;

		/**
		 * \brief 
		 */
		void Beep() const override;

		/**
		 * \brief 
		 * \param frequency 
		 * \param duration 
		 */
		void Beep(unsigned int frequency, const Duration& duration) const override;

	private:

		ConsoleWriter mOutput;
		ConsoleWriter mError;
		ConsoleReader mInput;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLESERVICE_HPP
