#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_FILEWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_FILEWRITER_HPP

#include <Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <Cocktail/Core/IO/Output/Writer/OutputStreamWriter.hpp>
#include <Cocktail/Core/System/FileSystem/File.hpp>

namespace Ck
{
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class FileWriter : public OutputStreamWriter<TEncoding>
    {
    public:

        explicit FileWriter(File& file) :
            mFileOutputStream(file),
            OutputStreamWriter<TEncoding>(mFileOutputStream)
        {
            /// Nothing
        }

    private:

        FileOutputStream<TAllocator> mFileOutputStream;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_FILEWRITER_HPP
