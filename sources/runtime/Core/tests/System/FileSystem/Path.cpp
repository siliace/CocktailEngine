#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/System/FileSystem/Path.hpp>

TEST_CASE("Parse a path", "[Path]")
{
    SECTION("With a relative path")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT("hello/world"));

        REQUIRE_FALSE(path.IsAbsolute());
        REQUIRE(path.GetRoot() == Ck::String::Empty);
        REQUIRE(path.ToString() == CK_TEXT("hello/world"));
        REQUIRE(path.GetParent().ToString() == CK_TEXT("hello"));
        REQUIRE(path.GetParent().GetParent().ToString() == Ck::String::Empty);
    }
    SECTION("With a relative path starting with a '.'")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT("./hello/world"));

        REQUIRE_FALSE(path.IsAbsolute());
        REQUIRE(path.GetRoot() == Ck::String::Empty);
        REQUIRE(path.ToString() == CK_TEXT("./hello/world"));
        REQUIRE(path.GetParent().ToString() == CK_TEXT("./hello"));
        REQUIRE(path.GetParent().GetParent().ToString() == CK_TEXT("."));
    }

    SECTION("With a relative Windows style path")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT(R"(..\..\Siliace\Documents)"));

        REQUIRE_FALSE(path.IsAbsolute());
        REQUIRE(path.GetRoot() == Ck::String::Empty);
        REQUIRE(path.ToString() == CK_TEXT(R"(..\..\Siliace\Documents)"));
        REQUIRE(path.GetParent().ToString() == CK_TEXT(R"(..\..\Siliace)"));
        REQUIRE(path.GetParent().GetParent().ToString() == CK_TEXT(R"(..\..)"));
    }

    SECTION("With an absolute Windows style path")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT(R"(C:\Users\Siliace\Documents)"));

        REQUIRE(path.IsAbsolute());
        REQUIRE(path.GetRoot() == CK_TEXT(R"(C:\)"));
        REQUIRE(path.ToString() == CK_TEXT(R"(C:\Users\Siliace\Documents)"));
        REQUIRE(path.GetParent().ToString() == CK_TEXT(R"(C:\Users\Siliace)"));
        REQUIRE(path.GetParent().GetParent().ToString() == CK_TEXT(R"(C:\Users)"));
    }

    SECTION("With an absolute UNC style path")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT(R"(\\Server\Share\Users\Siliace\Documents)"));

        REQUIRE(path.IsAbsolute());
        REQUIRE(path.GetRoot() == CK_TEXT(R"(\\Server\Share)"));
        REQUIRE(path.ToString() == CK_TEXT(R"(\\Server\Share\Users\Siliace\Documents)"));
        REQUIRE(path.GetParent().ToString() == CK_TEXT(R"(\\Server\Share\Users\Siliace)"));
    }

    SECTION("With an absolute unix style path")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT("/home/siliace/documents"));

        REQUIRE(path.IsAbsolute());
        REQUIRE(path.ToString() == CK_TEXT("/home/siliace/documents"));
        REQUIRE(path.GetParent().ToString() == CK_TEXT("/home/siliace"));
        REQUIRE(path.GetParent().GetParent().ToString() == CK_TEXT("/home"));
    }
}

TEST_CASE("Join two paths", "[Path]")
{
    Ck::Path absolute = Ck::Path::Parse(CK_TEXT("/home/siliace"), Ck::Path::Format::Generic);
    Ck::Path relative = Ck::Path::Parse(CK_TEXT("./documents/c++"), Ck::Path::Format::Generic);

    SECTION("A relative into an absolute")
    {
        REQUIRE(absolute.Join(relative).ToString() == CK_TEXT("/home/siliace/./documents/c++"));
    }

    SECTION("An absolute into an relative")
    {
        REQUIRE(relative.Join(absolute).ToString() == CK_TEXT("/home/siliace"));
    }
}

TEST_CASE("Get a path filename", "[Path]")
{
    SECTION("The extension is part of the filename")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets/wood.png")).GetFilename().ToString() == CK_TEXT("wood.png"));
    }

    SECTION("An extensionless filename is preserved")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets/README")).GetFilename().ToString() == CK_TEXT("README"));
    }

    SECTION("A hidden filename is preserved")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets/.gitignore")).GetFilename().ToString() == CK_TEXT(".gitignore"));
    }

    SECTION("A path ending with a separator has no filename")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets/")).GetFilename().IsEmpty());
    }
}

TEST_CASE("Remove a path extension", "[Path]")
{
    SECTION("Creates a copy without its last extension")
    {
        const Ck::Path path = Ck::Path::Parse(CK_TEXT("assets/archive.tar.gz"));

        REQUIRE(path.RemoveExtension().ToString() == CK_TEXT("assets/archive.tar"));
        REQUIRE(path.ToString() == CK_TEXT("assets/archive.tar.gz"));
    }

    SECTION("Removes the extension in place after the path has been serialized")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT("assets/wood.png"));

        REQUIRE(path.ToString() == CK_TEXT("assets/wood.png"));
        REQUIRE(&path.RemoveExtensionInPlace() == &path);
        REQUIRE(path.ToString() == CK_TEXT("assets/wood"));
    }

    SECTION("Leaves an extensionless filename unchanged")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT("assets/README"));

        REQUIRE(path.RemoveExtensionInPlace().ToString() == CK_TEXT("assets/README"));
    }

    SECTION("Only removes the final filename extension")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT("assets.v1/wood.png"));

        REQUIRE(path.RemoveExtensionInPlace().ToString() == CK_TEXT("assets.v1/wood"));
    }

    SECTION("Leaves a path without a filename unchanged")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT("assets.v1/"));

        REQUIRE(path.RemoveExtensionInPlace().ToString() == CK_TEXT("assets.v1/"));
    }
}

TEST_CASE("Remove all path extensions", "[Path]")
{
    SECTION("Creates a copy without all extensions")
    {
        const Ck::Path path = Ck::Path::Parse(CK_TEXT("assets/archive.tar.gz"));

        REQUIRE(path.RemoveAllExtensions().ToString() == CK_TEXT("assets/archive"));
        REQUIRE(path.ToString() == CK_TEXT("assets/archive.tar.gz"));
    }

    SECTION("Removes all extensions in place after the path has been serialized")
    {
        Ck::Path path = Ck::Path::Parse(CK_TEXT("assets/archive.tar.gz"));

        REQUIRE(path.ToString() == CK_TEXT("assets/archive.tar.gz"));
        path.RemoveAllExtensionsInPlace();
        REQUIRE(path.ToString() == CK_TEXT("assets/archive"));
    }

    SECTION("Removes a single extension")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets/wood.png")).RemoveAllExtensions().ToString() == CK_TEXT("assets/wood"));
    }

    SECTION("Leaves an extensionless filename unchanged")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets/README")).RemoveAllExtensions().ToString() == CK_TEXT("assets/README"));
    }
}

TEST_CASE("Get a path stem", "[Path]")
{
    SECTION("Removes only the last filename extension")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets/archive.tar.gz")).GetStem().ToString() == CK_TEXT("archive.tar"));
    }

    SECTION("Preserves an extensionless filename")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets/README")).GetStem().ToString() == CK_TEXT("README"));
    }

    SECTION("Ignores extensions in parent directories")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets.v1/wood.png")).GetStem().ToString() == CK_TEXT("wood"));
    }

    SECTION("Returns an empty path when there is no filename")
    {
        REQUIRE(Ck::Path::Parse(CK_TEXT("assets/")).GetStem().IsEmpty());
    }
}

TEST_CASE("Check whether a path starts with another path", "[Path]")
{
    const Ck::Path path = Ck::Path::Parse(CK_TEXT("assets/textures/wood.png"));

    SECTION("With matching leading elements")
    {
        REQUIRE(path.StartsWith(Ck::Path::Parse(CK_TEXT("assets"))));
        REQUIRE(path.StartsWith(Ck::Path::Parse(CK_TEXT("assets/textures"))));
        REQUIRE(path.StartsWith(path));
    }

    SECTION("With non-matching or longer prefixes")
    {
        REQUIRE_FALSE(path.StartsWith(Ck::Path::Parse(CK_TEXT("assets/models"))));
        REQUIRE_FALSE(path.StartsWith(Ck::Path::Parse(CK_TEXT("assets/textures/wood.png/preview"))));
    }

    SECTION("With a different root")
    {
        REQUIRE_FALSE(Ck::Path::Parse(CK_TEXT("/assets/textures/wood.png")).StartsWith(Ck::Path::Parse(CK_TEXT("assets"))));
    }
}

TEST_CASE("Check whether a path ends with another path", "[Path]")
{
    const Ck::Path path = Ck::Path::Parse(CK_TEXT("assets/textures/wood.png"));

    SECTION("With matching trailing elements")
    {
        REQUIRE(path.EndsWith(Ck::Path::Parse(CK_TEXT("wood.png"))));
        REQUIRE(path.EndsWith(Ck::Path::Parse(CK_TEXT("textures/wood.png"))));
        REQUIRE(path.EndsWith(path));
    }

    SECTION("With non-matching or longer suffixes")
    {
        REQUIRE_FALSE(path.EndsWith(Ck::Path::Parse(CK_TEXT("assets/wood.png"))));
        REQUIRE_FALSE(path.EndsWith(Ck::Path::Parse(CK_TEXT("assets/textures/wood.png/preview"))));
    }

    SECTION("With a different root")
    {
        REQUIRE_FALSE(Ck::Path::Parse(CK_TEXT("/assets/textures/wood.png")).EndsWith(Ck::Path::Parse(CK_TEXT("wood.png"))));
    }
}
