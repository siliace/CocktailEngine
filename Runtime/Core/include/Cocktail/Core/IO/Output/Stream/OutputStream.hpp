#ifndef COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP

#include <Cocktail/Core/Meta/Implements.hpp>

namespace Ck
{
    /**
     * \brief Base interface for custom output streams
     */
    class OutputStream : public Implements<OutputStream, Interface>
    {
    public:

        /**
         * \brief
         * \param
         * \param
         */
        virtual std::size_t Write(const void* data, std::size_t length) = 0;
        
        /**
         * \brief
         */
        virtual void Flush() = 0;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP