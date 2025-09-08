#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_FILEWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_FILEWRITER_HPP

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>
#include <Cocktail/Core/System/FileSystem/File.hpp>

namespace Ck
{
    class COCKTAIL_CORE_API FileWriter : public Writer
    {
    public:

        FileWriter(File& file, EncodingMode encodingMode);

        void Write(const TextChar* text, std::size_t length) override;

        void Flush() override;

    private:

        File* mFile;
        EncodingMode mEncodingMode;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_FILEWRITER_HPP
