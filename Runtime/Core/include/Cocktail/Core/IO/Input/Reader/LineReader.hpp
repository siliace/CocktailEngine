#ifndef COCKTAIL_CORE_IO_INPUT_READER_LINEREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_LINEREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>
#include <Cocktail/Core/String.hpp>

namespace Ck
{
    /**
     * \brief Reader that extracts lines of text from a character-based Reader
     *
     * LineReader wraps any Reader and provides convenient line-oriented access.
     * It reads characters until a newline (`\n`) is encountered, optionally
     * ignoring empty lines.
     *
     * Handles optional carriage return (`\r`) stripping on Windows.
     *
     * \tparam TEncoding The character encoding used by the underlying Reader
     * \tparam TAllocator Allocator type used for line string storage
     */
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class LineReader
    {
    public:

        /**
         * \brief Character type associated with the encoding
         */
        using CharType = typename Reader<TEncoding>::CharType;

        /**
         * \brief Size type associated with the encoding
         */
        using SizeType = typename Reader<TEncoding>::SizeType;

        /**
         * \brief Encoding type used by this reader
         */
        using EncodingType = typename Reader<TEncoding>::EncodingType;

        /**
         * \brief Constructor
         *
         * Wraps a character-based reader as the source for line reading.
         *
         * \param source Reference to the underlying Reader
         */
        explicit LineReader(Reader<TEncoding>& source) :
            mSource(&source)
        {
            /// Nothing to initialize
        }

        /**
         * \brief Read a single line from the source
         *
         * Reads characters from the source until a newline (`\n`) is found.
         * Optionally ignores empty lines.
         *
         * \param ignoreEmptyLines If true, empty lines are skipped
         *
         * \return Optional containing the line if successfully read, or empty if EOF
         */
        Optional<BasicString<EncodingType, TAllocator>> ReadLine(bool ignoreEmptyLines)
        {
            BasicString<EncodingType, TAllocator> line;
            line.Reserve(1024); ///< Reserve initial capacity for typical lines

            if (!ReadLine(line, ignoreEmptyLines))
                return Optional<BasicString<EncodingType, TAllocator>>::Empty();

            return Optional<BasicString<EncodingType, TAllocator>>::Of(std::move(line));
        }

        /**
         * \brief Read a single line into a pre-allocated string
         *
         * Reads characters into \p line until a newline (`\n`) is found.
         * Carriage return (`\r`) is removed on Windows.
         *
         * \param line Reference to string to store the line
         * \param ignoreEmptyLines If true, empty lines are skipped
         *
         * \return True if a line was successfully read, false if EOF is reached
         */
        bool ReadLine(BasicString<EncodingType, TAllocator>& line, bool ignoreEmptyLines = true)
        {
            line.Clear();

            CharType c;
            while (mSource->Read(c))
            {
                if (c == static_cast<CharType>('\n'))
                {
#ifdef COCKTAIL_OS_WINDOWS
                    line.TryLast().Then([&](CharType last) {
                        if (last == static_cast<CharType>('\r'))
                            line.PopLast();
                    });
#endif
                    if (!line.IsEmpty() || !ignoreEmptyLines)
                        return true;
                }
                else
                {
                    line.Append(c);
                }
            }

            return false;
        }

    private:

        /** \brief Underlying character reader */
        Reader<EncodingType>* mSource;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_LINEREADER_HPP
