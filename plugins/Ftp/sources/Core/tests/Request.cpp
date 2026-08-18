#include <catch2/catch_all.hpp>

#include <Ftp/Core/Request.hpp>

TEST_CASE("Formating an Abor request", "[Request]")
{
    Ck::Ftp::AbortRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("ABOR\r\n"));
}

TEST_CASE("Formating an Acct request", "[Request]")
{
    Ck::Ftp::AccountRequest request(CK_UTF8_TEXT("account"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("ACCT account\r\n"));
}

TEST_CASE("Formating an Allo request", "[Request]")
{
    Ck::Ftp::AllocateRequest request(CK_UTF8_TEXT("1024"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("ALLO 1024\r\n"));
}

TEST_CASE("Formating an Appe request", "[Request]")
{
    Ck::Ftp::AppendRequest request(CK_UTF8_TEXT("my/file.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("APPE my/file.txt\r\n"));
}

TEST_CASE("Formating an Auth request", "[Request]")
{
    Ck::Ftp::AuthenticationRequest request(CK_UTF8_TEXT("TLS"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("AUTH TLS\r\n"));
}

TEST_CASE("Formating a Ccc request", "[Request]")
{
    Ck::Ftp::ClearCommandChannelRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("CCC\r\n"));
}

TEST_CASE("Formating a Cdup request", "[Request]")
{
    Ck::Ftp::ChangeToParentDirectoryRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("CDUP\r\n"));
}

TEST_CASE("Formating a Conf request", "[Request]")
{
    Ck::Ftp::ConfidentialityProtectionCommandRequest request(CK_UTF8_TEXT("data"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("CONF data\r\n"));
}

TEST_CASE("Formating a Cwd request", "[Request]")
{
    Ck::Ftp::ChangeDirectoryRequest request(CK_UTF8_TEXT("my/path"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("CWD my/path\r\n"));
}

TEST_CASE("Formating a Dele request", "[Request]")
{
    Ck::Ftp::DeleteRequest request(CK_UTF8_TEXT("my/path/here.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("DELE my/path/here.txt\r\n"));
}

TEST_CASE("Formating an Eprt request", "[Request]")
{
    Ck::Ftp::ExtendedPortRequest request(CK_UTF8_TEXT("|1|127.0.0.1|12345|"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("EPRT |1|127.0.0.1|12345|\r\n"));
}

TEST_CASE("Formating an Epsv request", "[Request]")
{
    Ck::Ftp::ExtendedPassiveRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("EPSV\r\n"));
}

TEST_CASE("Formating a Feat request", "[Request]")
{
    Ck::Ftp::FeatureRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("FEAT\r\n"));
}

TEST_CASE("Formating a Help request", "[Request]")
{
    Ck::Ftp::HelpRequest request(CK_UTF8_TEXT("RETR"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("HELP RETR\r\n"));
}

TEST_CASE("Formating a Lang request", "[Request]")
{
    Ck::Ftp::LanguageRequest request(CK_UTF8_TEXT("en"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("LANG en\r\n"));
}

TEST_CASE("Formating a List request", "[Request]")
{
    Ck::Ftp::ListRequest request(CK_UTF8_TEXT("my/path"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("LIST my/path\r\n"));
}

TEST_CASE("Formating a Mdtm request", "[Request]")
{
    Ck::Ftp::ModificationTimeRequest request(CK_UTF8_TEXT("my/file.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("MDTM my/file.txt\r\n"));
}

TEST_CASE("Formating an Mff request", "[Request]")
{
    Ck::Ftp::ModifyFactRequest request(CK_UTF8_TEXT("modify=20261023120000 my/file.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("MFF modify=20261023120000 my/file.txt\r\n"));
}

TEST_CASE("Formating an Mkd request", "[Request]")
{
    Ck::Ftp::MakeDirectoryRequest request(CK_UTF8_TEXT("my/new-directory"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("MKD my/new-directory\r\n"));
}

TEST_CASE("Formating an Mlst request", "[Request]")
{
    Ck::Ftp::MachineListRequest request(CK_UTF8_TEXT("my/path"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("MLST my/path\r\n"));
}

TEST_CASE("Formating a Mode request", "[Request]")
{
    Ck::Ftp::TransferModeRequest request(CK_UTF8_TEXT("S"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("MODE S\r\n"));
}

TEST_CASE("Formating an Nlst request", "[Request]")
{
    Ck::Ftp::NameListRequest request(CK_UTF8_TEXT("my/path"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("NLST my/path\r\n"));
}

TEST_CASE("Formating a Noop request", "[Request]")
{
    Ck::Ftp::NoOperationRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("NOOP\r\n"));
}

TEST_CASE("Formating an Opts request", "[Request]")
{
    Ck::Ftp::OptionsRequest request(CK_UTF8_TEXT("UTF8 ON"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("OPTS UTF8 ON\r\n"));
}

TEST_CASE("Formating a Pass request", "[Request]")
{
    Ck::Ftp::PasswordRequest request(CK_UTF8_TEXT("secret"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("PASS secret\r\n"));
}

TEST_CASE("Formating a Pasv request", "[Request]")
{
    Ck::Ftp::PassiveRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("PASV\r\n"));
}

TEST_CASE("Formating a Pbsz request", "[Request]")
{
    Ck::Ftp::ProtectionBufferSizeRequest request(CK_UTF8_TEXT("0"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("PBSZ 0\r\n"));
}

TEST_CASE("Formating a Port request", "[Request]")
{
    Ck::Ftp::PortRequest request(CK_UTF8_TEXT("127,0,0,1,48,57"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("PORT 127,0,0,1,48,57\r\n"));
}

TEST_CASE("Formating a Prot request", "[Request]")
{
    Ck::Ftp::DataChannelProtectionRequest request(CK_UTF8_TEXT("P"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("PROT P\r\n"));
}

TEST_CASE("Formating a Pwd request", "[Request]")
{
    Ck::Ftp::PrintWorkingDirectoryRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("PWD\r\n"));
}

TEST_CASE("Formating a Quit request", "[Request]")
{
    Ck::Ftp::QuitRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("QUIT\r\n"));
}

TEST_CASE("Formating a Rein request", "[Request]")
{
    Ck::Ftp::ReinitializeRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("REIN\r\n"));
}

TEST_CASE("Formating a Rest request", "[Request]")
{
    Ck::Ftp::RestartRequest request(CK_UTF8_TEXT("1024"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("REST 1024\r\n"));
}

TEST_CASE("Formating a Retr request", "[Request]")
{
    Ck::Ftp::RetrieveRequest request(CK_UTF8_TEXT("my/file.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("RETR my/file.txt\r\n"));
}

TEST_CASE("Formating an Rmd request", "[Request]")
{
    Ck::Ftp::RemoveDirectoryRequest request(CK_UTF8_TEXT("my/directory"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("RMD my/directory\r\n"));
}

TEST_CASE("Formating an Rnfr request", "[Request]")
{
    Ck::Ftp::RenameFromRequest request(CK_UTF8_TEXT("old-name.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("RNFR old-name.txt\r\n"));
}

TEST_CASE("Formating an Rnto request", "[Request]")
{
    Ck::Ftp::RenameToRequest request(CK_UTF8_TEXT("new-name.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("RNTO new-name.txt\r\n"));
}

TEST_CASE("Formating a Site request", "[Request]")
{
    Ck::Ftp::SiteParametersRequest request(CK_UTF8_TEXT("HELP"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("SITE HELP\r\n"));
}

TEST_CASE("Formating a Size request", "[Request]")
{
    Ck::Ftp::FileSizeRequest request(CK_UTF8_TEXT("my/file.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("SIZE my/file.txt\r\n"));
}

TEST_CASE("Formating an Smnt request", "[Request]")
{
    Ck::Ftp::StructureMountRequest request(CK_UTF8_TEXT("/mnt/ftp"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("SMNT /mnt/ftp\r\n"));
}

TEST_CASE("Formating a Stat request", "[Request]")
{
    Ck::Ftp::StatusRequest request(CK_UTF8_TEXT("my/path"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("STAT my/path\r\n"));
}

TEST_CASE("Formating a Stor request", "[Request]")
{
    Ck::Ftp::StoreRequest request(CK_UTF8_TEXT("my/file.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("STOR my/file.txt\r\n"));
}

TEST_CASE("Formating a Stou request", "[Request]")
{
    Ck::Ftp::StoreUniqueRequest request(CK_UTF8_TEXT("my/file.txt"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("STOU my/file.txt\r\n"));
}

TEST_CASE("Formating a Stru request", "[Request]")
{
    Ck::Ftp::FileStructureRequest request(CK_UTF8_TEXT("F"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("STRU F\r\n"));
}

TEST_CASE("Formating a Syst request", "[Request]")
{
    Ck::Ftp::SystemTypeRequest request;

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("SYST\r\n"));
}

TEST_CASE("Formating a Type request", "[Request]")
{
    Ck::Ftp::RepresentationTypeRequest request(CK_UTF8_TEXT("I"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("TYPE I\r\n"));
}

TEST_CASE("Formating a User request", "[Request]")
{
    Ck::Ftp::UserRequest request(CK_UTF8_TEXT("anonymous"));

    Ck::StringWriter<Ck::Encoders::Utf8> writer;
    request.Serialize(writer);

    REQUIRE(writer.ToString() == CK_UTF8_TEXT("USER anonymous\r\n"));
}
