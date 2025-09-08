#ifndef COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP

#include <utility>

namespace Ck
{
    /**
     * \brief Base interface for custom output streams
     */
    class OutputStream
    {
    public:
		
		/**
		 * \brief Destructor
		 */
		virtual ~OutputStream() = default;

        /**
         * \brief Write data to the stream
         *
    	 * \param data A buffer containing data to write in the stream
		 * \param length The number of bytes to write to the stream
		 *
         * \return The number of bytes actually written
         */
        virtual std::size_t Write(const void* data, std::size_t length) = 0;
        
        /**
         * \brief Ensure every pending write operation is terminated
         * Depending on the implementation, this function might be a no-op or
         */
        virtual void Flush() = 0;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP