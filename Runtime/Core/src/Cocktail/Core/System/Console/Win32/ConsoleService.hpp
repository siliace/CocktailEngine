#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Console/ConsoleService.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleReader.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleWriter.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class ConsoleService final : public Inherit<ConsoleService, Object, Ck::ConsoleService>
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

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Writer> GetOutput() override;

		/**
		 * \brief
		 * \return
		 */
		Ref<Writer> GetError() override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Reader> GetInput() override;

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

		WORD mIntialAttributes;
		Ref<ConsoleWriter> mOutput;
		Ref<ConsoleWriter> mError;
		Ref<ConsoleReader> mInput;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLESERVICE_HPP
