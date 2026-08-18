#include <catch2/catch_all.hpp>

#include <initializer_list>

#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemDriver.hpp>
#include <CocktailEngine/Core/Utility/Finder.hpp>

namespace
{
    Ck::Path AppendPath(const Ck::Path& parent, const Ck::TextChar* child)
    {
        Ck::Path path = parent;
        path.Join(child);

        return path;
    }

    const Ck::Path FixtureRoot(CK_TEXT("finder-fixture"), Ck::Path::Format::Generic);
    const Ck::Path Source = AppendPath(FixtureRoot, CK_TEXT("source"));

    Ck::Path InSource(const Ck::TextChar* suffix)
    {
        return AppendPath(Source, suffix);
    }

    void PopulateFinderTree(Ck::MemoryFileSystemDriver& driver)
    {
        driver.CreateDirectory(FixtureRoot);
        driver.CreateDirectory(Source);
        driver.CreateDirectory(InSource(CK_TEXT("images")));
        driver.CreateDirectory(InSource(CK_TEXT("images/nested")));
        driver.CreateDirectory(InSource(CK_TEXT("skip")));
        driver.CreateDirectory(InSource(CK_TEXT("skip/child")));
        driver.CreateDirectory(InSource(CK_TEXT("skip-sibling")));

        driver.CreateFile(InSource(CK_TEXT("root.txt")));
        driver.CreateFile(InSource(CK_TEXT("root.md")));
        driver.CreateFile(InSource(CK_TEXT("CAPS.TXT")));
        driver.CreateFile(InSource(CK_TEXT("images/icon.png")));
        driver.CreateFile(InSource(CK_TEXT("images/notes.txt")));
        driver.CreateFile(InSource(CK_TEXT("images/nested/leaf.txt")));
        driver.CreateFile(InSource(CK_TEXT("images/nested/guide.md")));
        driver.CreateFile(InSource(CK_TEXT("skip/private.txt")));
        driver.CreateFile(InSource(CK_TEXT("skip/child/deep.txt")));
        driver.CreateFile(InSource(CK_TEXT("skip-sibling/sibling.txt")));
    }

    void RequirePaths(const Ck::Array<Ck::Path>& actual, std::initializer_list<Ck::Path> expected)
    {
        REQUIRE(actual.GetSize() == expected.size());

        for (const Ck::Path& path : expected)
            REQUIRE(actual.Contains(path));
    }
}

TEST_CASE("Find direct entries from a non-root source", "[Finder]")
{
    Ck::MemoryFileSystemDriver driver;
    PopulateFinderTree(driver);

    const Ck::Array<Ck::Path> paths = Ck::Finder().From(Source).Get(driver);

    RequirePaths(paths, {
        InSource(CK_TEXT("root.txt")),
        InSource(CK_TEXT("root.md")),
        InSource(CK_TEXT("CAPS.TXT")),
        InSource(CK_TEXT("images")),
        InSource(CK_TEXT("skip")),
        InSource(CK_TEXT("skip-sibling")),
    });
}

TEST_CASE("Filter finder files by extension", "[Finder]")
{
    Ck::MemoryFileSystemDriver driver;
    PopulateFinderTree(driver);

    SECTION("An extension without a leading dot is normalized")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .WithExtension(CK_TEXT("txt"))
            .IgnoreDirectories()
            .Recursively(true)
            .Get(driver);

        RequirePaths(paths, {
            InSource(CK_TEXT("root.txt")),
            InSource(CK_TEXT("images/notes.txt")),
            InSource(CK_TEXT("images/nested/leaf.txt")),
            InSource(CK_TEXT("skip/private.txt")),
            InSource(CK_TEXT("skip/child/deep.txt")),
            InSource(CK_TEXT("skip-sibling/sibling.txt")),
        });
    }

    SECTION("Extensions are compared exactly")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .WithExtension(CK_TEXT(".TXT"))
            .IgnoreDirectories()
            .Get(driver);

        RequirePaths(paths, { InSource(CK_TEXT("CAPS.TXT")) });
    }
}

TEST_CASE("Filter finder entries with predicates", "[Finder]")
{
    Ck::MemoryFileSystemDriver driver;
    PopulateFinderTree(driver);

    SECTION("Where predicates are combined and receive qualified paths")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .Where([](const Ck::Path& path) {
                return path.StartsWith(InSource(CK_TEXT("images")));
            })
            .Where([](const Ck::Path& path) {
                return path.GetExtension() == CK_TEXT(".txt");
            })
            .IgnoreDirectories()
            .Recursively(true)
            .Get(driver);

        RequirePaths(paths, {
            InSource(CK_TEXT("images/notes.txt")),
            InSource(CK_TEXT("images/nested/leaf.txt")),
        });
    }

    SECTION("A rejected directory remains traversable")
    {
        const Ck::Path images = InSource(CK_TEXT("images"));
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .Where([images](const Ck::Path& path) {
                return !(path == images);
            })
            .Recursively(true, 2)
            .Get(driver);

        REQUIRE(paths.GetSize() == 14);
        REQUIRE_FALSE(paths.Contains(images));
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/notes.txt"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/nested"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/nested/leaf.txt"))));
    }
}

TEST_CASE("Exclude finder entries", "[Finder]")
{
    Ck::MemoryFileSystemDriver driver;
    PopulateFinderTree(driver);

    SECTION("Exclude hides one exact file")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .Exclude(InSource(CK_TEXT("root.md")))
            .Get(driver);

        RequirePaths(paths, {
            InSource(CK_TEXT("root.txt")),
            InSource(CK_TEXT("CAPS.TXT")),
            InSource(CK_TEXT("images")),
            InSource(CK_TEXT("skip")),
            InSource(CK_TEXT("skip-sibling")),
        });
    }

    SECTION("Exclude hides a directory without pruning its descendants")
    {
        const Ck::Path images = InSource(CK_TEXT("images"));
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .Exclude(images)
            .Recursively(true, 2)
            .Get(driver);

        REQUIRE(paths.GetSize() == 14);
        REQUIRE_FALSE(paths.Contains(images));
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/icon.png"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/nested"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/nested/guide.md"))));
    }

    SECTION("ExcludeDirectory prunes a subtree without matching a sibling prefix")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .ExcludeDirectory(InSource(CK_TEXT("skip")))
            .Recursively(true, 2)
            .Get(driver);

        REQUIRE(paths.GetSize() == 11);
        REQUIRE_FALSE(paths.Contains(InSource(CK_TEXT("skip"))));
        REQUIRE_FALSE(paths.Contains(InSource(CK_TEXT("skip/private.txt"))));
        REQUIRE_FALSE(paths.Contains(InSource(CK_TEXT("skip/child"))));
        REQUIRE_FALSE(paths.Contains(InSource(CK_TEXT("skip/child/deep.txt"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("skip-sibling"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("skip-sibling/sibling.txt"))));
    }

    SECTION("ExcludeDirectory skips an excluded source")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(InSource(CK_TEXT("skip")))
            .ExcludeDirectory(InSource(CK_TEXT("skip")))
            .Recursively(true)
            .Get(driver);

        REQUIRE(paths.IsEmpty());
    }
}

TEST_CASE("Control finder entry visibility", "[Finder]")
{
    Ck::MemoryFileSystemDriver driver;
    PopulateFinderTree(driver);

    SECTION("IgnoreFiles keeps directories")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .IgnoreFiles()
            .Recursively(true, 2)
            .Get(driver);

        RequirePaths(paths, {
            InSource(CK_TEXT("images")),
            InSource(CK_TEXT("images/nested")),
            InSource(CK_TEXT("skip")),
            InSource(CK_TEXT("skip/child")),
            InSource(CK_TEXT("skip-sibling")),
        });
    }

    SECTION("IgnoreDirectories keeps files and continues to traverse")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .IgnoreDirectories()
            .Recursively(true, 2)
            .Get(driver);

        RequirePaths(paths, {
            InSource(CK_TEXT("root.txt")),
            InSource(CK_TEXT("root.md")),
            InSource(CK_TEXT("CAPS.TXT")),
            InSource(CK_TEXT("images/icon.png")),
            InSource(CK_TEXT("images/notes.txt")),
            InSource(CK_TEXT("images/nested/leaf.txt")),
            InSource(CK_TEXT("images/nested/guide.md")),
            InSource(CK_TEXT("skip/private.txt")),
            InSource(CK_TEXT("skip/child/deep.txt")),
            InSource(CK_TEXT("skip-sibling/sibling.txt")),
        });
    }

    SECTION("Ignore flags can be disabled again")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder()
            .From(Source)
            .IgnoreFiles()
            .IgnoreFiles(false)
            .IgnoreDirectories()
            .IgnoreDirectories(false)
            .Get(driver);

        RequirePaths(paths, {
            InSource(CK_TEXT("root.txt")),
            InSource(CK_TEXT("root.md")),
            InSource(CK_TEXT("CAPS.TXT")),
            InSource(CK_TEXT("images")),
            InSource(CK_TEXT("skip")),
            InSource(CK_TEXT("skip-sibling")),
        });
    }
}

TEST_CASE("Respect finder recursion depth", "[Finder]")
{
    Ck::MemoryFileSystemDriver driver;
    PopulateFinderTree(driver);

    const std::initializer_list<Ck::Path> directEntries = {
        InSource(CK_TEXT("root.txt")),
        InSource(CK_TEXT("root.md")),
        InSource(CK_TEXT("CAPS.TXT")),
        InSource(CK_TEXT("images")),
        InSource(CK_TEXT("skip")),
        InSource(CK_TEXT("skip-sibling")),
    };

    SECTION("The default search is not recursive")
    {
        RequirePaths(Ck::Finder().From(Source).Get(driver), directEntries);
    }

    SECTION("Disabling recursion ignores the requested depth")
    {
        RequirePaths(Ck::Finder().From(Source).Recursively(false, 2).Get(driver), directEntries);
    }

    SECTION("A recursion depth of zero keeps the search direct")
    {
        RequirePaths(Ck::Finder().From(Source).Recursively(true, 0).Get(driver), directEntries);
    }

    SECTION("A recursion depth of one opens direct child directories")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder().From(Source).Recursively(true, 1).Get(driver);

        REQUIRE(paths.GetSize() == 12);
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/icon.png"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/nested"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("skip/private.txt"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("skip/child"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("skip-sibling/sibling.txt"))));
        REQUIRE_FALSE(paths.Contains(InSource(CK_TEXT("images/nested/leaf.txt"))));
        REQUIRE_FALSE(paths.Contains(InSource(CK_TEXT("skip/child/deep.txt"))));
    }

    SECTION("A recursion depth of two reaches the complete fixture")
    {
        const Ck::Array<Ck::Path> paths = Ck::Finder().From(Source).Recursively(true, 2).Get(driver);

        REQUIRE(paths.GetSize() == 15);
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/nested/leaf.txt"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("images/nested/guide.md"))));
        REQUIRE(paths.Contains(InSource(CK_TEXT("skip/child/deep.txt"))));
    }
}
