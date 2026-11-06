#ifndef COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_TRANSFER_HPP
#define COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_TRANSFER_HPP

#include <CocktailEngine/Core/System/FileSystem/PathInfo.hpp>
#include <CocktailEngine/Core/System/FileSystem/Path.hpp>
#include <CocktailEngine/Core/System/Network/TcpClient.hpp>

#include <Ftp/Core/Client.hpp>

namespace Ck::Ftp::Detail
{
        /**
         * \brief A directory entry parsed from an MLSD/MLST fact line (RFC 3659)
         */
        struct ListingEntry
        {
            Utf8String Name;
            PathInfo Info;
            bool Skip = false; // cdir/pdir entries
        };

    Utf8String ToFtpPath(const Path& path);
    bool ParseNumber(const Utf8String& text, std::size_t& position, Uint64& value);
    void ParseListingLines(const Utf8Char* data, std::size_t length, bool onlyLeadingSpace, Array<ListingEntry>& entries);
    Array<ListingEntry> ListDirectory(const Client& client, const Path& path);
    Array<Byte> Download(const Client& client, const Path& path);
    void Upload(const Client& client, const Path& path, const Array<Byte>& content);
    PathInfo GetPathInfoFromFallback(const Client& client, const Path& path);
    Utf8String GetWorkingDirectory(const Client& client);
}

#endif // COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_TRANSFER_HPP

