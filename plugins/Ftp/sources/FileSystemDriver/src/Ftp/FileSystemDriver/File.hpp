#ifndef COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_FILE_HPP
#define COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_FILE_HPP

#include <CocktailEngine/Core/System/FileSystem/File.hpp>

namespace Ck::Ftp
{
    class File : public Ck::File
    {
    public:

        unsigned Read(void* buffer, unsigned length) override;
        unsigned Write(const void* buffer, unsigned length) override;
        Uint64 GetCursor() const override;
        Uint64 SetCursor(FileCursorMode mode, Int64 offset) override;
        std::size_t GetSize() const override;
        const Path& GetPath() const override;
        void Flush() const override;
        void* GetSystemHandle() const override;
    };
}

#endif // COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_FILE_HPP
