#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEWRITER_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEWRITER_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class ConsoleWriter : public Inherit<ConsoleWriter, Object, Writer>
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static Ref<ConsoleWriter> FromOutputHandle();

		/**
		 * \brief 
		 * \return 
		 */
		static Ref<ConsoleWriter> FromErrorHandle();

		/**
		 * \brief
		 * \param handle
		 */
		explicit ConsoleWriter(HANDLE handle);

		/**
		 * \brief 
		 * \param string 
		 * \param length 
		 */
		void Write(const char* string, std::size_t length) override;

		/**
		 * \brief 
		 */
		void Flush() override;
		
	private:

		HANDLE mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEWRITER_HPP
