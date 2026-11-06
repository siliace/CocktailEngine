#ifndef COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_FILE_HPP
#define COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_FILE_HPP

#include <CocktailEngine/Core/System/FileSystem/File.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileOpenFlags.hpp>

#include <Ftp/Core/Client.hpp>

namespace Ck::Ftp
{
    /**
     * \brief File of an FTP server
     *
     * FTP can not read or write at an arbitrary offset of a remote file, so the
     * content is held in memory: it is downloaded when the file is opened, all
     * the operations work on this copy and the copy is sent back to the server
     * by Flush (also called when the file is destroyed) if it has been modified.
     *
     * The file must not outlive the Client it has been created from.
     */
    class File : public Ck::File
    {
    public:

        /**
         * \param client The client of the server owning the file
         * \param path The path of the file on the server
         * \param flags The flags the file is opened with
         * \param content The current content of the file on the server
         * \param modified True if \p content differs from the content on the server
         */
        File(const Client& client, Path path, const FileOpenFlags& flags, Array<Byte> content, bool modified);
        ~File() override;

        File(const File&) = delete;
        File& operator=(const File&) = delete;

        unsigned Read(void* buffer, unsigned length) override;
        unsigned Write(const void* buffer, unsigned length) override;
        Uint64 GetCursor() const override;
        Uint64 SetCursor(FileCursorMode mode, Int64 offset) override;
        std::size_t GetSize() const override;
        const Path& GetPath() const override;
        void Flush() const override;
        void* GetSystemHandle() const override;

    private:

        const Client* mClient;
        Path mPath;
        FileOpenFlags mFlags;
        Array<Byte> mContent;
        Uint64 mCursor;
        mutable bool mModified;
    };
}

#endif // COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_FILE_HPP
