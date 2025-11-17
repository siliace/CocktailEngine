#ifndef COCKTAIL_CORE_IO_TEXTWRITER_HPP
#define COCKTAIL_CORE_IO_TEXTWRITER_HPP

#include <Cocktail/Core/Utility/Encoding/Encoders.hpp>

namespace Ck
{
    template <typename TEncoding = Encoders::Text>
    class Writer
    {
    public:

        using EncodingType = TEncoding;

        using CharType = typename EncodingType::CharType;

        using SizeType = typename EncodingType::SizeType;

        virtual ~Writer() = default;

        virtual void Write(const CharType* text, SizeType length) = 0;

        virtual void Flush() = 0;
    };
}

#endif // COCKTAIL_CORE_IO_TEXTWRITER_HPP
