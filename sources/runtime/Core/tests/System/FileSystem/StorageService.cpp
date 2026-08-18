#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemDriver.hpp>
#include <CocktailEngine/Core/System/FileSystem/StorageService.hpp>

namespace
{
    constexpr unsigned int CharacterSize = sizeof(Ck::TextChar);

    Ck::URI SourceUri(const Ck::Path& path)
    {
        return { CK_TEXT("source"), path };
    }

    Ck::URI DestinationUri(const Ck::Path& path)
    {
        return { CK_TEXT("destination"), path };
    }

    Ck::String ReadAll(Ck::FileSystemDriver& driver, const Ck::Path& path)
    {
        Ck::UniquePtr<Ck::File> file = driver.OpenFile(path, Ck::FileOpenFlagBits::Read);

        Ck::String content;
        Ck::TextChar character;
        while (file->Read(&character, CharacterSize) == CharacterSize)
            content.Append(character);

        return content;
    }

    void WriteAll(Ck::FileSystemDriver& driver, const Ck::Path& path, const Ck::String& content)
    {
        const Ck::FileOpenFlags flags = Ck::FileOpenFlags::Of(Ck::FileOpenFlagBits::Write, Ck::FileOpenFlagBits::Truncate);

        Ck::UniquePtr<Ck::File> file = driver.OpenFile(path, flags);
        file->Write(content.GetData(), static_cast<unsigned int>(content.GetLength()) * CharacterSize);
    }
}

TEST_CASE("Transfer entries between storage drivers", "[StorageService]")
{
    Ck::MemoryFileSystemDriver sourceDriver;
    Ck::MemoryFileSystemDriver destinationDriver;
    Ck::StorageService storage(CK_TEXT("source"));
    storage.MountExternal(CK_TEXT("source"), &sourceDriver);
    storage.MountExternal(CK_TEXT("destination"), &destinationDriver);

    SECTION("Copying a file creates a missing destination")
    {
        WriteAll(sourceDriver, CK_TEXT("readme.txt"), CK_TEXT("source"));

        storage.CopyFile(SourceUri(CK_TEXT("readme.txt")), DestinationUri(CK_TEXT("copy.txt")), {});

        REQUIRE(destinationDriver.IsFile(CK_TEXT("copy.txt")));
        REQUIRE(ReadAll(destinationDriver, CK_TEXT("copy.txt")) == CK_TEXT("source"));
    }

    SECTION("Copying over a longer destination truncates it")
    {
        WriteAll(sourceDriver, CK_TEXT("readme.txt"), CK_TEXT("short"));
        WriteAll(destinationDriver, CK_TEXT("copy.txt"), CK_TEXT("a much longer destination"));

        Ck::FileCopyOptions options;
        options.Overwrite = true;
        storage.CopyFile(SourceUri(CK_TEXT("readme.txt")), DestinationUri(CK_TEXT("copy.txt")), options);

        REQUIRE(ReadAll(destinationDriver, CK_TEXT("copy.txt")) == CK_TEXT("short"));
    }

    SECTION("Copying a recursive directory preserves complete and extensionless names")
    {
        sourceDriver.CreateDirectory(CK_TEXT("assets"));
        WriteAll(sourceDriver, CK_TEXT("assets/wood.png"), CK_TEXT("wood"));
        WriteAll(sourceDriver, CK_TEXT("assets/README"), CK_TEXT("readme"));
        sourceDriver.CreateDirectory(CK_TEXT("assets/models"));
        WriteAll(sourceDriver, CK_TEXT("assets/models/cube.gltf"), CK_TEXT("cube"));

        Ck::DirectoryCopyOptions options;
        options.Recursive = true;
        storage.CopyDirectory(SourceUri(CK_TEXT("assets")), DestinationUri(CK_TEXT("backup")), options);

        REQUIRE(ReadAll(destinationDriver, CK_TEXT("backup/wood.png")) == CK_TEXT("wood"));
        REQUIRE(ReadAll(destinationDriver, CK_TEXT("backup/README")) == CK_TEXT("readme"));
        REQUIRE(ReadAll(destinationDriver, CK_TEXT("backup/models/cube.gltf")) == CK_TEXT("cube"));
    }

    SECTION("Moving a file creates its missing destination and removes its source")
    {
        WriteAll(sourceDriver, CK_TEXT("readme.txt"), CK_TEXT("source"));

        storage.MoveFile(SourceUri(CK_TEXT("readme.txt")), DestinationUri(CK_TEXT("moved.txt")), {});

        REQUIRE_FALSE(sourceDriver.IsFile(CK_TEXT("readme.txt")));
        REQUIRE(ReadAll(destinationDriver, CK_TEXT("moved.txt")) == CK_TEXT("source"));
    }

    SECTION("Moving a directory creates its missing destination and removes its source")
    {
        sourceDriver.CreateDirectory(CK_TEXT("assets"));
        WriteAll(sourceDriver, CK_TEXT("assets/README"), CK_TEXT("readme"));
        sourceDriver.CreateDirectory(CK_TEXT("assets/models"));
        WriteAll(sourceDriver, CK_TEXT("assets/models/cube.gltf"), CK_TEXT("cube"));

        storage.MoveDirectory(SourceUri(CK_TEXT("assets")), DestinationUri(CK_TEXT("backup")), {});

        REQUIRE_FALSE(sourceDriver.IsDirectory(CK_TEXT("assets")));
        REQUIRE(ReadAll(destinationDriver, CK_TEXT("backup/README")) == CK_TEXT("readme"));
        REQUIRE(ReadAll(destinationDriver, CK_TEXT("backup/models/cube.gltf")) == CK_TEXT("cube"));
    }

    SECTION("Disabling copy fallback leaves the source untouched")
    {
        WriteAll(sourceDriver, CK_TEXT("readme.txt"), CK_TEXT("source"));

        Ck::FileMoveOptions options;
        options.AllowCopyFallback = false;
        REQUIRE_THROWS_AS(storage.MoveFile(SourceUri(CK_TEXT("readme.txt")), DestinationUri(CK_TEXT("moved.txt")), options), Ck::CrossDeviceLinkException);

        REQUIRE(sourceDriver.IsFile(CK_TEXT("readme.txt")));
        REQUIRE_FALSE(destinationDriver.IsFile(CK_TEXT("moved.txt")));
    }

    SECTION("Moving the root directory is rejected before copying")
    {
        sourceDriver.CreateDirectory(CK_TEXT("assets"));
        WriteAll(sourceDriver, CK_TEXT("assets/README"), CK_TEXT("readme"));

        REQUIRE_THROWS_AS(storage.MoveDirectory(SourceUri(Ck::Path::Empty), DestinationUri(CK_TEXT("backup")), {}), Ck::InvalidParameterException);
        REQUIRE(sourceDriver.IsDirectory(CK_TEXT("assets")));
        REQUIRE_FALSE(destinationDriver.IsDirectory(CK_TEXT("backup")));
    }
}
