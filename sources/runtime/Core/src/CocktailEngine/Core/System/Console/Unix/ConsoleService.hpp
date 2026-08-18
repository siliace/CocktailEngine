#ifndef COCKTAILENGINE_CORE_SYSTEM_CONSOLE_UNIX_CONSOLESERVICE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONSOLE_UNIX_CONSOLESERVICE_HPP

#include <CocktailEngine/Core/IO/Output/Writer/LineWriter.hpp>
#include <CocktailEngine/Core/System/Console/ConsoleService.hpp>
#include <CocktailEngine/Core/System/Console/Unix/ConsoleReader.hpp>
#include <CocktailEngine/Core/System/Console/Unix/ConsoleWriter.hpp>

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

#endif // COCKTAILENGINE_CORE_SYSTEM_CONSOLE_UNIX_CONSOLESERVICE_HPP
