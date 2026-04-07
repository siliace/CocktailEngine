#ifndef COCKTAIL_CORE_IO_TEXTWRITER_HPP
#define COCKTAIL_CORE_IO_TEXTWRITER_HPP

#include <Cocktail/Core/Utility/Encoding/Encoders.hpp>

namespace Ck
{
    /**
     * \brief Abstract base class for writing text data to various destinations
     *
     * This class defines a generic interface for writing text data (characters)
     * to different outputs, such as files, memory buffers, or streams. The
     * character encoding is defined by the template parameter.
     *
     * \tparam TEncoding The charset encoding used to encode the characters
     */
    template <typename TEncoding = Encoders::Text>
    class Writer
    {
    public:

        /**
         * \brief The encoding type used by this writer
         */
        using EncodingType = TEncoding;

        /**
         * \brief Character type associated with the encoding
         */
        using CharType = typename EncodingType::CharType;

        /**
         * \brief Size type associated with the encoding
         */
        using SizeType = typename EncodingType::SizeType;

        /**
         * \brief Destructor
         *
         * Virtual destructor to allow proper cleanup of derived classes.
         */
        virtual ~Writer() = default;

        /**
         * \brief Write an array of characters to the output
         *
         * \param text Pointer to the characters to write
         * \param length Number of characters to write
         *
         * \note Implementations should respect the character encoding defined
         *       by \ref EncodingType.
         */
        virtual void Write(const CharType* text, SizeType length) = 0;

        /**
         * \brief Flush the output
         *
         * Ensures that all buffered or pending characters are written to the
         * underlying destination. After calling \ref Flush, no data should remain
         * pending in the writer.
         */
        virtual void Flush() = 0;
    };
}

#endif // COCKTAIL_CORE_IO_TEXTWRITER_HPP
