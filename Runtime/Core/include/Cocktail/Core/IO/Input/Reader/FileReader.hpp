#ifndef COCKTAIL_CORE_IO_INPUT_READER_FILEREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_FILEREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>

namespace Ck
{
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class FileReader : public InputStreamReader<TEncoding, FileInputStream<TAllocator>>
    {
    public:

        using CharType = typename InputStreamReader<TEncoding, FileInputStream<TAllocator>>::CharType;

        using SizeType = typename InputStreamReader<TEncoding, FileInputStream<TAllocator>>::SizeType;

        using EncodingType = typename InputStreamReader<TEncoding, FileInputStream<TAllocator>>::EncodingType;

        explicit FileReader(const Path& path, FileSystemDriver* driver = LocalFileSystem::GetRootDriver()) :
            mFileInputStream(path, driver),
            InputStreamReader<TEncoding, FileInputStream<TAllocator>>(mFileInputStream)
        {
            /// Nothing
        }

        File* GetFile() const
        {
            return mFileInputStream->GetFile();
        }

    private:

        FileInputStream<TAllocator> mFileInputStream;
    };

    using AsciiFileReader = FileReader<Encoders::Ascii>;
    using Utf8FileReader = FileReader<Encoders::Utf8>;
    using Utf16FileReader = FileReader<Encoders::Utf16>;
    using Utf32FileReader = FileReader<Encoders::Utf32>;
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_FILEREADER_HPP
