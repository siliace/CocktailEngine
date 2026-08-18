#include <limits>
#include <system_error>

#include <CocktailEngine/Core/System/FileSystem/Storage.hpp>
#include <CocktailEngine/Core/Utility/FileUtils.hpp>

namespace Ck
{
    void FileUtils::MakeDirectories(const Path& path, FileSystemDriver& driver)
    {
        if (driver.IsDirectory(path))
            return;

        const Path parent = path.GetParent();
        if (parent != path)
            MakeDirectories(parent, driver);

        driver.CreateDirectory(path);
    }

    ByteArray FileUtils::ReadFile(const Path& path, FileSystemDriver& driver)
    {
        ByteArray content;
        if (!driver.IsFile(path))
            return content;

        UniquePtr<File> file = driver.OpenFile(path, FileOpenFlagBits::Read | FileOpenFlagBits::Existing);

        const std::size_t size = file->GetSize();
        if (!size)
            return content;

        content.Resize(size);

        constexpr std::size_t MaxReadSize = std::numeric_limits<unsigned int>::max();
        std::size_t readSize = 0;
        while (readSize < size)
        {
            const std::size_t remainingSize = size - readSize;
            const unsigned int requestSize = static_cast<unsigned int>(remainingSize > MaxReadSize ? MaxReadSize : remainingSize);
            const unsigned int read = file->Read(content.GetData() + readSize, requestSize);
            if (read == 0)
                throw std::system_error(std::make_error_code(std::errc::io_error));

            readSize += read;
        }

        return content;
    }

    void FileUtils::WriteFile(const Path& path, ByteArrayView content, FileSystemDriver& driver)
    {
        MakeDirectories(path.GetParent(), driver);

        UniquePtr<File> file = driver.OpenFile(path, FileOpenFlagBits::Write | FileOpenFlagBits::Truncate);

        if (!content.IsEmpty())
            file->Write(content.GetData(), content.GetSize());

        file->Flush();
    }

    void FileUtils::AppendFile(const Path& path, ByteArrayView content, FileSystemDriver& driver)
    {
        MakeDirectories(path.GetParent(), driver);

        UniquePtr<File> file = driver.OpenFile(path, FileOpenFlagBits::Write | FileOpenFlagBits::Append);

        if (!content.IsEmpty())
            file->Write(content.GetData(), content.GetSize());

        file->Flush();
    }
}
