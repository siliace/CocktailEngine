#ifndef COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/File.hpp>

namespace Ck
{
    template <typename TAllocator = SizedHeapAllocator<32>>
    class FileOutputStream : public OutputStream<TAllocator>
    {
    public:

        using AllocatorType = TAllocator;

        using SizeType = typename AllocatorType::SizeType;

        explicit FileOutputStream(File& file) :
            mFile(&file)
        {
            /// Nothing
        }

        SizeType Write(const Byte* data, SizeType length) override
        {
            return mFile->Write(data, length);
        }

        void Flush() override
        {
            mFile->Flush();
        }

    private:

        File* mFile;
    };
}

#endif // COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP
