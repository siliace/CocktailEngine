#ifndef COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp>

namespace Ck
{
    template <typename TAllocator = SizedHeapAllocator<32>>
    class FileOutputStream : public OutputStream<TAllocator>
    {
    public:

        using AllocatorType = TAllocator;

        using SizeType = typename AllocatorType::SizeType;

        explicit FileOutputStream(const Path& path, bool truncate = false, FileSystemDriver* driver = LocalFileSystem::GetRootDriver()) :
            mFile(driver->OpenFile(path, truncate ? FileOpenFlagBits::Write | FileOpenFlagBits::Truncate : FileOpenFlagBits::Write))
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

        File* GetFile() const
        {
            return mFile.Get();
        }

    private:

        UniquePtr<File> mFile;
    };
}

#endif // COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP
