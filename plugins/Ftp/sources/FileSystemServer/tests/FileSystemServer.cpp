#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/System/FileSystem/FileSystemException.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemDriver.hpp>

#include <Ftp/Core/Exception.hpp>

#include <Ftp/FileSystemDriver/FileSystemDriver.hpp>
#include <Ftp/FileSystemServer/FileSystemServer.hpp>

namespace
{
    constexpr unsigned short TestPort = 21210;

    std::string ReadAll(Ck::FileSystemDriver& driver, const Ck::Path& path)
    {
        Ck::UniquePtr<Ck::File> file = driver.OpenFile(path, Ck::FileOpenFlagBits::Read);

        std::string content;
        char buffer[256];
        while (unsigned int count = file->Read(buffer, sizeof(buffer)))
            content.append(buffer, count);

        return content;
    }

    void WriteAll(Ck::FileSystemDriver& driver, const Ck::Path& path, const std::string& content)
    {
        const Ck::FileOpenFlags flags = Ck::FileOpenFlags::Of(Ck::FileOpenFlagBits::Write, Ck::FileOpenFlagBits::Truncate);

        Ck::UniquePtr<Ck::File> file = driver.OpenFile(path, flags);
        file->Write(content.data(), static_cast<unsigned int>(content.size()));
        file->Flush();
    }
}

TEST_CASE("A FileSystemServer exposes a driver through FTP", "[FileSystemServer]")
{
    Ck::IpAddressV4 loopback(127, 0, 0, 1);

    Ck::MemoryFileSystemDriver memory;
    memory.CreateDirectory(CK_TEXT("docs"));
    WriteAll(memory, CK_TEXT("docs/hello.txt"), "Hello FTP");

    Ck::Ftp::FileSystemServerOptions options;
    options.Port = TestPort;
    options.AllowAnonymous = true;
    options.Accounts.Add({ Ck::Utf8String(CK_UTF8_TEXT("bob")), Ck::Utf8String(CK_UTF8_TEXT("secret")) });

    Ck::Ftp::FileSystemServer server(&memory, options, loopback);

    SECTION("Login is required and checked")
    {
        Ck::Ftp::LoginInfo wrong{ Ck::Utf8String(CK_UTF8_TEXT("bob")), Ck::Utf8String(CK_UTF8_TEXT("wrong")) };
        REQUIRE_THROWS_AS(Ck::Ftp::FileSystemDriver(loopback, &wrong, TestPort), Ck::Ftp::Exception);

        Ck::Ftp::LoginInfo good{ Ck::Utf8String(CK_UTF8_TEXT("bob")), Ck::Utf8String(CK_UTF8_TEXT("secret")) };
        REQUIRE_NOTHROW(Ck::Ftp::FileSystemDriver(loopback, &good, TestPort));
    }

    Ck::Ftp::LoginInfo login{ Ck::Utf8String(CK_UTF8_TEXT("anonymous")), Ck::Utf8String(CK_UTF8_TEXT("guest")) };
    Ck::Ftp::FileSystemDriver client(loopback, &login, TestPort);

    SECTION("Entries are described")
    {
        REQUIRE(client.IsDirectory(CK_TEXT("docs")));
        REQUIRE_FALSE(client.IsFile(CK_TEXT("docs")));
        REQUIRE(client.IsFile(CK_TEXT("docs/hello.txt")));
        REQUIRE(client.GetPathInfo(CK_TEXT("docs/hello.txt")).Size == 9);
        REQUIRE(client.GetPathInfo(CK_TEXT("missing")).Type == Ck::PathType::None);
        REQUIRE(client.GetPathInfo(CK_TEXT("../../docs/hello.txt")).Type == Ck::PathType::File);
    }

    SECTION("Directories are listed")
    {
        Ck::UniquePtr<Ck::DirectoryIterator> iterator = client.CreateDirectoryIterator(CK_TEXT("docs"));
        REQUIRE(iterator);
        REQUIRE_FALSE(iterator->IsEnd());
        REQUIRE(iterator->GetPath().GetFilename().ToString() == CK_TEXT("hello.txt"));
        REQUIRE(iterator->GetPathInfo().Type == Ck::PathType::File);
        iterator->Next();
        REQUIRE(iterator->IsEnd());
    }

    SECTION("Files are downloaded")
    {
        REQUIRE(ReadAll(client, CK_TEXT("docs/hello.txt")) == "Hello FTP");
    }

    SECTION("Files are uploaded")
    {
        std::string content(100000, 'x');
        WriteAll(client, CK_TEXT("docs/big.bin"), content);

        REQUIRE(ReadAll(memory, CK_TEXT("docs/big.bin")) == content);
    }

    SECTION("Entries are created, moved and removed")
    {
        client.CreateDirectory(CK_TEXT("new"));
        REQUIRE(memory.IsDirectory(CK_TEXT("new")));

        client.MoveFile(CK_TEXT("docs/hello.txt"), CK_TEXT("new/moved.txt"), Ck::FileMoveOptions());
        REQUIRE_FALSE(memory.IsFile(CK_TEXT("docs/hello.txt")));
        REQUIRE(ReadAll(memory, CK_TEXT("new/moved.txt")) == "Hello FTP");

        client.RemoveFile(CK_TEXT("new/moved.txt"), Ck::FileRemoveOptions());
        REQUIRE_FALSE(memory.IsFile(CK_TEXT("new/moved.txt")));

        client.RemoveDirectory(CK_TEXT("new"), Ck::DirectoryRemoveOptions());
        REQUIRE_FALSE(memory.IsDirectory(CK_TEXT("new")));
    }

    SECTION("Several clients are served at the same time")
    {
        Ck::Ftp::FileSystemDriver other(loopback, &login, TestPort);

        REQUIRE(server.GetConnectionCount() == 2);
        REQUIRE(ReadAll(other, CK_TEXT("docs/hello.txt")) == ReadAll(client, CK_TEXT("docs/hello.txt")));
    }
}

TEST_CASE("A read-only FileSystemServer refuses modifications", "[FileSystemServer]")
{
    Ck::IpAddressV4 loopback(127, 0, 0, 1);

    Ck::MemoryFileSystemDriver memory;
    memory.CreateDirectory(CK_TEXT("docs"));

    Ck::Ftp::FileSystemServerOptions options;
    options.Port = TestPort + 1;
    options.AllowAnonymous = true;
    options.ReadOnly = true;

    Ck::Ftp::FileSystemServer server(&memory, options, loopback);

    Ck::Ftp::LoginInfo login{ Ck::Utf8String(CK_UTF8_TEXT("ftp")), Ck::Utf8String(CK_UTF8_TEXT("")) };
    Ck::Ftp::FileSystemDriver client(loopback, &login, TestPort + 1);

    REQUIRE_THROWS(client.CreateDirectory(CK_TEXT("forbidden")));
    REQUIRE_FALSE(memory.IsDirectory(CK_TEXT("forbidden")));
}
