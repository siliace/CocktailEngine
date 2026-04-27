#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLESERVICE_HPP

#include <Cocktail/Core/IO/Output/Writer/LineWriter.hpp>
#include <Cocktail/Core/System/Console/ConsoleService.hpp>
#include <Cocktail/Core/System/Console/Unix/ConsoleReader.hpp>
#include <Cocktail/Core/System/Console/Unix/ConsoleWriter.hpp>

namespace Ck::Detail::Unix
{
	class ConsoleService final : public Ck::ConsoleService
	{
	public:

		ConsoleService();

		LineWriter<> GetOutput() override;
		LineWriter<> GetError() override;
		LineReader<> GetInput() override;

		void Clear() override;

		void SetColors(ConsoleColor text, ConsoleColor background, ConsoleStyle style) override;

		void Beep() const override;
		void Beep(unsigned int frequency, const Duration& duration) const override;


	private:

		ConsoleWriter mOutput;
		ConsoleWriter mError;
		ConsoleReader mInput;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLESERVICE_HPP
