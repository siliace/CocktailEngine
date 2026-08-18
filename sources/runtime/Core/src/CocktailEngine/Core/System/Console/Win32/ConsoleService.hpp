#ifndef COCKTAILENGINE_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP

#include <CocktailEngine/Core/System/Console/ConsoleService.hpp>
#include <CocktailEngine/Core/System/Console/Win32/ConsoleReader.hpp>
#include <CocktailEngine/Core/System/Console/Win32/ConsoleWriter.hpp>

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

#endif // COCKTAILENGINE_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP
