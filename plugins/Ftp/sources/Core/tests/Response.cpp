#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/IO/Output/Writer/StringWriter.hpp>

#include <Ftp/Core/Response.hpp>

TEST_CASE("Serializing a single-line response", "[Response][Serialize]")
{
    Ck::Ftp::Response response{ 200, CK_UTF8_TEXT("Command okay") };

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    response.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("200 Command okay\r\n"));
}

TEST_CASE("Serializing a multi-line response", "[Response][Serialize]")
{
    Ck::Ftp::Response response{ 211, CK_UTF8_TEXT("Features:\r\n UTF8\r\nEnd") };

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    response.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("211-Features:\r\n UTF8\r\n211 End\r\n"));
}

TEST_CASE("Parsing a single-line response", "[Response][Parse]")
{
    Ck::Ftp::Response response = Ck::Ftp::Response::Parse(CK_UTF8_TEXT("220 Service ready\r\n"));

    REQUIRE(response.Code == 220);
    REQUIRE(response.IsPositiveCompletion());
}

TEST_CASE("Response status classification", "[Response]")
{
    REQUIRE(Ck::Ftp::Response{ 150, {} }.IsPreliminary());
    REQUIRE(Ck::Ftp::Response{ 226, {} }.IsPositiveCompletion());
    REQUIRE(Ck::Ftp::Response{ 331, {} }.IsIntermediate());
    REQUIRE(Ck::Ftp::Response{ 530, {} }.IsNegativeCompletion());
}
