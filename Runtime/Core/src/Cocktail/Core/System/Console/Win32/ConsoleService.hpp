#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP

#include <Cocktail/Core/System/Console/ConsoleService.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleReader.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleWriter.hpp>

namespace Ck::Detail::Win32
{
	class ConsoleService final : public Ck::ConsoleService
	{
	public:

		ConsoleService();

		~ConsoleService() override;

		LineWriter<> GetOutput() override;
		LineWriter<> GetError() override;
		LineReader<> GetInput() override;

		void Clear() override;

		void SetColors(ConsoleColor text, ConsoleColor background, ConsoleStyle) override;

		void Beep() const override;
		void Beep(unsigned int frequency, const Duration& duration) const override;

	private:

		WORD mInitialAttributes;
		ConsoleWriter mOutput;
		ConsoleWriter mError;
		ConsoleReader mInput;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP
