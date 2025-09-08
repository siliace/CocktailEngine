#ifndef COCKTAIL_CORE_IO_TEXTWRITER_HPP
#define COCKTAIL_CORE_IO_TEXTWRITER_HPP

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/IO/EncodingMode.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	class Writer
	{
	public:
		
		/**
		 * \brief Destructor
		 */
		virtual ~Writer() = default;

		/**
		 * \brief Write text in the stream
		 *
		 * \param text A buffer containing the text to write
		 * \param length The length (in character) of the text buffer to write
		 */
		virtual void Write(const TextChar* text, std::size_t length) = 0;

		/**
		 * \brief Flushes the stream
		 */
		virtual void Flush() = 0;

		/**
		 * \brief Get the encoding mode used to write text
		 * Depending on the system and the encoding mode used, conversion
		 * between two encoding modes might occur and hurt performances.
		 *
		 * \return The encoding mode
		 */
		virtual EncodingMode GetEncodingMode() = 0;
	};
}

#endif // COCKTAIL_CORE_IO_TEXTWRITER_HPP
