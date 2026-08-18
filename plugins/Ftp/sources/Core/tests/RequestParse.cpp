#include <catch2/catch_all.hpp>

#include <Ftp/Core/Request.hpp>

TEST_CASE("Parsing a request without argument", "[Request][Parse]")
{
    Ck::Optional<Ck::Ftp::GenericRequest> request = Ck::Ftp::Request::Parse(CK_UTF8_TEXT("NOOP"));

    REQUIRE(!request.IsEmpty());
    REQUIRE(request.Get().GetCode() == Ck::Ftp::Request::Code::Noop);
    REQUIRE(request.Get().GetArgument().IsEmpty());
}

TEST_CASE("Parsing a request with an argument", "[Request][Parse]")
{
    Ck::Optional<Ck::Ftp::GenericRequest> request = Ck::Ftp::Request::Parse(CK_UTF8_TEXT("USER anonymous"));

    REQUIRE(!request.IsEmpty());
    REQUIRE(request.Get().GetCode() == Ck::Ftp::Request::Code::User);
    REQUIRE(request.Get().GetArgument() == CK_UTF8_TEXT("anonymous"));
}

TEST_CASE("Parsing a request with an argument containing spaces", "[Request][Parse]")
{
    Ck::Optional<Ck::Ftp::GenericRequest> request = Ck::Ftp::Request::Parse(CK_UTF8_TEXT("OPTS UTF8 ON"));

    REQUIRE(!request.IsEmpty());
    REQUIRE(request.Get().GetCode() == Ck::Ftp::Request::Code::Opts);
    REQUIRE(request.Get().GetArgument() == CK_UTF8_TEXT("UTF8 ON"));
}

TEST_CASE("Parsing a request is case-insensitive on the command", "[Request][Parse]")
{
    Ck::Optional<Ck::Ftp::GenericRequest> request = Ck::Ftp::Request::Parse(CK_UTF8_TEXT("user bob"));

    REQUIRE(!request.IsEmpty());
    REQUIRE(request.Get().GetCode() == Ck::Ftp::Request::Code::User);
    REQUIRE(request.Get().GetArgument() == CK_UTF8_TEXT("bob"));
}

TEST_CASE("Parsing an unknown request yields no request", "[Request][Parse]")
{
    Ck::Optional<Ck::Ftp::GenericRequest> request = Ck::Ftp::Request::Parse(CK_UTF8_TEXT("XYZZY something"));

    REQUIRE(request.IsEmpty());
}

TEST_CASE("A parsed request serializes back to its command line", "[Request][Parse]")
{
    Ck::Optional<Ck::Ftp::GenericRequest> request = Ck::Ftp::Request::Parse(CK_UTF8_TEXT("RETR my/file.txt"));
    REQUIRE(!request.IsEmpty());

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Get().Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("RETR my/file.txt\r\n"));
}
