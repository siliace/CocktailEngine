#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP

#include <Cocktail/Core/System/Console/ConsoleService.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleReader.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleWriter.hpp>

namespace Ck::Detail::Win32
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
		 */
		~ConsoleService() override;

		void Write(const String& text) override;
		void WriteLine(const String& text) override;

		void Write(StringView text) override;
		void WriteLine(StringView text) override;

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
		 * \param text
		 * \param background
		 */
		void SetColors(ConsoleColor text, ConsoleColor background, ConsoleStyle) override;

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

		WORD mIntialAttributes;
		ConsoleWriter mOutput;
		ConsoleWriter mError;
		ConsoleReader mInput;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP
