#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLEWRITER_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLEWRITER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>

namespace Ck::Detail::Unix
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
		 */
		explicit ConsoleWriter(int handle);

		/**
		 * \brief 
		 * \param string 
		 * \param length 
		 */
		void Write(const TextChar* string, std::size_t length) override;

		/**
		 * \brief 
		 */
		void Flush() override;

		/**
		 * \brief Get the encoding mode used to write in the console
		 *
		 * On Unix style systems, this likely always be EncodingMode::Utf8
		 *
		 * \return The encoding mode
		 */
		EncodingMode GetEncodingMode() override;

	private:

		int mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLEWRITER_HPP
