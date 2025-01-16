#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEWRITER_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEWRITER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class ConsoleWriter : public Writer
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static ConsoleWriter FromOutputHandle();

		/**
		 * \brief 
		 * \return 
		 */
		static ConsoleWriter FromErrorHandle();

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
