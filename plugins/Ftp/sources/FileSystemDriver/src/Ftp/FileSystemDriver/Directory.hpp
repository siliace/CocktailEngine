#ifndef COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_DIRECTORY_HPP
#define COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_DIRECTORY_HPP

#include <CocktailEngine/Core/System/FileSystem/Directory.hpp>

#include <Ftp/Core/Client.hpp>

namespace Ck::Ftp
{
    /**
     * \brief Directory of an FTP server
     *
     * The directory must not outlive the Client it has been created from.
     */
    class Directory : public Ck::Directory
    {
    public:

        Directory(const Client& client, Path path);

        /**
         * \brief List the entries of the directory on the server
         *
         * The content is fetched at each call.
         */
        Array<Path> GetContent() const override;
        const Path& GetPath() const override;
        void* GetSystemHandle() const override;

    private:

        const Client* mClient;
        Path mPath;
    };
}

#endif // COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_DIRECTORY_HPP
