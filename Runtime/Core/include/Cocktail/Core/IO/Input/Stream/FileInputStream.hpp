#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_FILEINPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_FILEINPUTSTREAM_HPP

#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp>

namespace Ck
{
    template <typename TAllocator = SizedHeapAllocator<32>>
    class FileInputStream : public InputStream<TAllocator>
    {
    public:

        using AllocatorType = TAllocator;

        using SizeType = typename InputStream<TAllocator>::SizeType;

        explicit FileInputStream(const Path& path, FileSystemDriver* driver = LocalFileSystem::GetRootDriver()) :
            mFile(driver->OpenFile(path, FileOpenFlagBits::Read))
        {
            /// Nothing
        }

        bool HasCursor() const override
        {
            return true;
        }

        SizeType Read(Byte* buffer, SizeType length) override
        {
            return mFile->Read(buffer, length);
        }

        Uint64 Seek(Uint64 position) override
        {
            return mFile->SetCursor(FileCursorMode::Current, position);
        }

        Uint64 Tell() const override
        {
            return mFile->GetCursor();
        }

        void Rewind() override
        {
            mFile->SetCursor(FileCursorMode::Begin, 0);
        }

        SizeType GetSize() const override
        {
            return mFile->GetSize();
        }

        bool IsEof() const override
        {
            return mFile->GetCursor() == mFile->GetSize();
        }

        File* GetFile() const
        {
            return mFile.Get();
        }

    private:

        UniquePtr<File> mFile;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_FILEINPUTSTREAM_HPP
