#ifndef COCKTAIL_CORE_IO_TEXTWRITER_HPP
#define COCKTAIL_CORE_IO_TEXTWRITER_HPP

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
		 * \brief
		 * \param string
		 * \param length
		 */
		virtual void Write(const char* string, std::size_t length) = 0;

		/**
		 * \brief Flushes the stream
		 */
		virtual void Flush() = 0;
	};
}

#endif // COCKTAIL_CORE_IO_TEXTWRITER_HPP
