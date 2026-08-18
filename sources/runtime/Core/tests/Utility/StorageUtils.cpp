#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemDriver.hpp>
#include <CocktailEngine/Core/System/FileSystem/StorageService.hpp>
#include <CocktailEngine/Core/Utility/StorageUtils.hpp>

using namespace Ck;

namespace
{
    void RequireBytes(const ByteArray& actual, const Byte* expected, std::size_t expectedSize)
    {
        REQUIRE(actual.GetSize() == expectedSize);

        for (std::size_t index = 0; index < expectedSize; ++index)
            REQUIRE(actual.GetData()[index] == expected[index]);
    }
}

TEST_CASE("StorageUtils delegates writes and appends to the URI driver", "[Utility][StorageUtils]")
{
    MemoryFileSystemDriver driver;
    StorageService storage(CK_TEXT("default"));
    storage.MountExternal(CK_TEXT("memory"), &driver);

    const URI bytesUri = { CK_TEXT("memory"), CK_TEXT("cache/data.bin") };
    const Byte initial[] = { 0x01, 0x02 };
    const Byte suffix[] = { 0x03, 0x04 };

    StorageUtils::WriteFile(bytesUri, ByteArrayView(initial, sizeof(initial)), &storage);
    StorageUtils::AppendFile(bytesUri, ByteArrayView(suffix, sizeof(suffix)), &storage);

    REQUIRE(driver.IsFile(bytesUri.GetPath()));
    const Byte appended[] = { 0x01, 0x02, 0x03, 0x04 };
    RequireBytes(StorageUtils::ReadFile(bytesUri, &storage), appended, sizeof(appended));

    const URI textUri = { CK_TEXT("memory"), CK_TEXT("cache/text.txt") };
    StorageUtils::WriteFile(textUri, AsciiStringView("first"), FileUtils::EncodingOption::UTF8, &storage);
    StorageUtils::AppendFile(textUri, AsciiStringView(" second"), FileUtils::EncodingOption::UTF8WithoutBOM, &storage);

    const Byte utf8[] = { 0xef, 0xbb, 0xbf, 'f', 'i', 'r', 's', 't', ' ', 's', 'e', 'c', 'o', 'n', 'd' };
    RequireBytes(StorageUtils::ReadFile(textUri, &storage), utf8, sizeof(utf8));

    using Encoding = Encoders::Ascii;
    using AsciiString = BasicString<Encoding>;

    const URI linesUri = { CK_TEXT("memory"), CK_TEXT("cache/lines.txt") };
    Array<AsciiString> initialLines;
    initialLines.Add(AsciiString("first"));
    Array<AsciiString> appendedLines;
    appendedLines.Add(AsciiString("second"));

    StorageUtils::WriteFileLines<Encoding>(linesUri, initialLines, FileUtils::EncodingOption::UTF8WithoutBOM, &storage);
    StorageUtils::AppendFileLines<Encoding>(linesUri, appendedLines, FileUtils::EncodingOption::UTF8WithoutBOM, &storage);

    const Array<AsciiString> lines = StorageUtils::ReadFileLines<Encoding>(linesUri, false, &storage);
    REQUIRE(lines.GetSize() == 2);
    REQUIRE(lines[0] == AsciiString("first"));
    REQUIRE(lines[1] == AsciiString("second"));
}
