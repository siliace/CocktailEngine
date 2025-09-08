#include <Cocktail/Core/IO/Output/Writer/FileWriter.hpp>

namespace Ck
{
    FileWriter::FileWriter(File& file, EncodingMode encodingMode):
        mFile(&file),
        mEncodingMode(encodingMode)
    {
        /// Nothing
    }

    void FileWriter::Write(const TextChar* text, std::size_t length)
    {
    }

    void FileWriter::Flush()
    {
        mFile->Flush();
    }
}
