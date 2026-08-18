#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemDriver.hpp>
#include <CocktailEngine/Core/Utility/FileUtils.hpp>

using namespace Ck;

namespace
{
    class ChunkedReadFile final : public File
    {
    public:

        explicit ChunkedReadFile(UniquePtr<File> file) :
            mFile(Move(file))
        {
        }

        unsigned int Read(void* buffer, unsigned int length) override
        {
            return mFile->Read(buffer, length > ReadChunkSize ? ReadChunkSize : length);
        }

        unsigned int Write(const void* buffer, unsigned int length) override
        {
            return mFile->Write(buffer, length);
        }

        Uint64 GetCursor() const override
        {
            return mFile->GetCursor();
        }

        Uint64 SetCursor(FileCursorMode mode, Int64 offset) override
        {
            return mFile->SetCursor(mode, offset);
        }

        std::size_t GetSize() const override
        {
            return mFile->GetSize();
        }

        const Path& GetPath() const override
        {
            return mFile->GetPath();
        }

        void Flush() const override
        {
            mFile->Flush();
        }

        void* GetSystemHandle() const override
        {
            return mFile->GetSystemHandle();
        }

    private:

        static constexpr unsigned int ReadChunkSize = 2;

        UniquePtr<File> mFile;
    };

    class ChunkedReadMemoryFileSystemDriver final : public MemoryFileSystemDriver
    {
    public:

        UniquePtr<File> OpenFile(const Path& path, const FileOpenFlags& flags) override
        {
            return MakeUnique<ChunkedReadFile>(MemoryFileSystemDriver::OpenFile(path, flags));
        }
    };

    void RequireBytes(const ByteArray& actual, const Byte* expected, std::size_t expectedSize)
    {
        REQUIRE(actual.GetSize() == expectedSize);

        for (std::size_t index = 0; index < expectedSize; ++index)
            REQUIRE(actual.GetData()[index] == expected[index]);
    }
}

TEST_CASE("FileUtils creates nested directories on the selected driver", "[Utility][FileUtils]")
{
    MemoryFileSystemDriver driver;

    FileUtils::MakeDirectories(CK_TEXT("cache/shaders/spirv"), driver);

    REQUIRE(driver.IsDirectory(CK_TEXT("cache")));
    REQUIRE(driver.IsDirectory(CK_TEXT("cache/shaders")));
    REQUIRE(driver.IsDirectory(CK_TEXT("cache/shaders/spirv")));
    REQUIRE_NOTHROW(FileUtils::MakeDirectories(CK_TEXT("cache/shaders/spirv"), driver));
}

TEST_CASE("FileUtils completes partial file reads", "[Utility][FileUtils]")
{
    ChunkedReadMemoryFileSystemDriver driver;
    const Path path = CK_TEXT("partial-read.bin");
    const Byte expected[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };

    FileUtils::WriteFile(path, ByteArrayView(expected, sizeof(expected)), driver);

    RequireBytes(FileUtils::ReadFile(path, driver), expected, sizeof(expected));
}

TEST_CASE("FileUtils writes, appends and truncates bytes on the selected driver", "[Utility][FileUtils]")
{
    MemoryFileSystemDriver driver;
    const Path path = CK_TEXT("cache/data/payload.bin");
    const Path appendedPath = CK_TEXT("cache/data/appended.bin");
    const Byte initial[] = { 0x01, 0x02, 0x03 };
    const Byte suffix[] = { 0x04, 0x05 };
    const Byte replaced[] = { 0xfe, 0xff };

    FileUtils::WriteFile(path, ByteArrayView(initial, sizeof(initial)), driver);
    REQUIRE(driver.IsFile(path));
    RequireBytes(FileUtils::ReadFile(path, driver), initial, sizeof(initial));

    FileUtils::AppendFile(path, ByteArrayView(suffix, sizeof(suffix)), driver);
    const Byte appended[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    RequireBytes(FileUtils::ReadFile(path, driver), appended, sizeof(appended));

    FileUtils::AppendFile(path, ByteArrayView(suffix, sizeof(suffix)), driver);
    const Byte appendedTwice[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x04, 0x05 };
    RequireBytes(FileUtils::ReadFile(path, driver), appendedTwice, sizeof(appendedTwice));

    FileUtils::AppendFile(path, ByteArrayView(), driver);
    RequireBytes(FileUtils::ReadFile(path, driver), appendedTwice, sizeof(appendedTwice));

    FileUtils::WriteFile(path, ByteArrayView(replaced, sizeof(replaced)), driver);
    RequireBytes(FileUtils::ReadFile(path, driver), replaced, sizeof(replaced));

    FileUtils::WriteFile(path, ByteArrayView(), driver);
    REQUIRE(driver.IsFile(path));
    REQUIRE(FileUtils::ReadFile(path, driver).IsEmpty());

    FileUtils::AppendFile(appendedPath, ByteArrayView(suffix, sizeof(suffix)), driver);
    REQUIRE(driver.IsFile(appendedPath));
    RequireBytes(FileUtils::ReadFile(appendedPath, driver), suffix, sizeof(suffix));
}

TEST_CASE("FileUtils appends text fragments with explicit encoding", "[Utility][FileUtils]")
{
    MemoryFileSystemDriver driver;
    const Path autoPath = CK_TEXT("text/auto.txt");
    const Path utf8Path = CK_TEXT("text/utf8.txt");

    FileUtils::WriteFile(autoPath, AsciiStringView("plain"), FileUtils::EncodingOption::Auto, driver);
    const Byte plain[] = { 'p', 'l', 'a', 'i', 'n' };
    RequireBytes(FileUtils::ReadFile(autoPath, driver), plain, sizeof(plain));

    FileUtils::WriteFile(utf8Path, AsciiStringView("first"), FileUtils::EncodingOption::UTF8, driver);
    FileUtils::AppendFile(utf8Path, AsciiStringView(" second"), FileUtils::EncodingOption::UTF8WithoutBOM, driver);
    const Byte utf8[] = { 0xef, 0xbb, 0xbf, 'f', 'i', 'r', 's', 't', ' ', 's', 'e', 'c', 'o', 'n', 'd' };
    RequireBytes(FileUtils::ReadFile(utf8Path, driver), utf8, sizeof(utf8));
}

TEST_CASE("FileUtils writes and appends lines using a non-default encoding", "[Utility][FileUtils]")
{
    using Encoding = Encoders::Ascii;
    using AsciiString = BasicString<Encoding>;

    MemoryFileSystemDriver driver;
    const Path path = CK_TEXT("text/lines.txt");
    Array<AsciiString> lines;
    lines.Add(AsciiString("first"));
    lines.Add(AsciiString::Empty);
    lines.Add(AsciiString("third"));

    Array<AsciiString> appendedLines;
    appendedLines.Add(AsciiString("fourth"));

    FileUtils::WriteFileLines<Encoding>(path, lines, FileUtils::EncodingOption::UTF8WithoutBOM, driver);
    FileUtils::AppendFileLines<Encoding>(path, appendedLines, FileUtils::EncodingOption::UTF8WithoutBOM, driver);

    const Array<AsciiString> allLines = FileUtils::ReadFileLines<Encoding>(path, false, driver);
    REQUIRE(allLines.GetSize() == 4);
    REQUIRE(allLines[0] == AsciiString("first"));
    REQUIRE(allLines[1].IsEmpty());
    REQUIRE(allLines[2] == AsciiString("third"));
    REQUIRE(allLines[3] == AsciiString("fourth"));

    const Array<AsciiString> nonEmptyLines = FileUtils::ReadFileLines<Encoding>(path, true, driver);
    REQUIRE(nonEmptyLines.GetSize() == 3);
    REQUIRE(nonEmptyLines[0] == AsciiString("first"));
    REQUIRE(nonEmptyLines[1] == AsciiString("third"));
    REQUIRE(nonEmptyLines[2] == AsciiString("fourth"));
}
