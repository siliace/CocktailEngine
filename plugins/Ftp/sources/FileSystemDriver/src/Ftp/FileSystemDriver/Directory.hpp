#ifndef COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_DIRECTORY_HPP
#define COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_DIRECTORY_HPP

#include <CocktailEngine/Core/System/FileSystem/Directory.hpp>

namespace Ck::Ftp
{
    class Directory : public Ck::Directory
    {
    public:

        Array<Path> GetContent() const override;
        const Path& GetPath() const override;
        void* GetSystemHandle() const override;
    };
}

#endif // COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_DIRECTORY_HPP
