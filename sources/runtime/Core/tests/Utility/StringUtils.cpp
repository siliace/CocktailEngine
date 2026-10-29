#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/Utility/StringUtils.hpp>

namespace
{
    using TextStringUtils = Ck::StringUtils<Ck::TextChar, Ck::String::SizeType>;
}

TEST_CASE("Find counted occurrences from the start", "[StringUtils]")
{
    SECTION("Character in a bounded buffer")
    {
        const Ck::TextChar haystack[] = {CK_CHAR('a'), CK_CHAR('B'), CK_CHAR('A'), CK_CHAR('b'), CK_CHAR('a')};

        auto secondCaseSensitive = TextStringUtils::FindFirstOf(haystack, 5, CK_CHAR('a'), 1);
        REQUIRE_FALSE(secondCaseSensitive.IsEmpty());
        REQUIRE(secondCaseSensitive.Get() == 4);

        auto secondCaseInsensitive = TextStringUtils::FindFirstOf(haystack, 5, CK_CHAR('a'), 1, 0, false);
        REQUIRE_FALSE(secondCaseInsensitive.IsEmpty());
        REQUIRE(secondCaseInsensitive.Get() == 2);

        auto fromOffset = TextStringUtils::FindFirstOf(haystack, 5, CK_CHAR('a'), 0, 1);
        REQUIRE_FALSE(fromOffset.IsEmpty());
        REQUIRE(fromOffset.Get() == 4);

        REQUIRE(TextStringUtils::FindFirstOf(haystack, 5, CK_CHAR('a'), 2).IsEmpty());
        REQUIRE(TextStringUtils::FindFirstOf(haystack, 5, CK_CHAR('z'), 0).IsEmpty());
    }

    SECTION("Substring in a null-terminated string")
    {
        auto first = TextStringUtils::FindFirstOf(CK_TEXT("banana"), CK_TEXT("ana"), 0);
        REQUIRE_FALSE(first.IsEmpty());
        REQUIRE(first.Get() == 1);

        auto second = TextStringUtils::FindFirstOf(CK_TEXT("banana"), CK_TEXT("ana"), 1);
        REQUIRE_FALSE(second.IsEmpty());
        REQUIRE(second.Get() == 3);

        auto fromOffset = TextStringUtils::FindFirstOf(CK_TEXT("banana"), CK_TEXT("ana"), 0, 2);
        REQUIRE_FALSE(fromOffset.IsEmpty());
        REQUIRE(fromOffset.Get() == 3);

        auto caseInsensitive = TextStringUtils::FindFirstOf(CK_TEXT("bAnAnA"), CK_TEXT("ana"), 0, 0, false);
        REQUIRE_FALSE(caseInsensitive.IsEmpty());
        REQUIRE(caseInsensitive.Get() == 1);

        REQUIRE(TextStringUtils::FindFirstOf(CK_TEXT("bAnAnA"), CK_TEXT("ana"), 0).IsEmpty());
        REQUIRE(TextStringUtils::FindFirstOf(CK_TEXT("banana"), CK_TEXT(""), 0).IsEmpty());
    }

    SECTION("Substring in a bounded buffer")
    {
        const Ck::TextChar haystack[] = {CK_CHAR('x'), CK_CHAR('a'), CK_CHAR('b'), CK_CHAR('x'), CK_CHAR('a'), CK_CHAR('b')};
        const Ck::TextChar needle[] = {CK_CHAR('a'), CK_CHAR('b')};

        auto second = TextStringUtils::FindFirstOf(haystack, 6, needle, 2, 1);
        REQUIRE_FALSE(second.IsEmpty());
        REQUIRE(second.Get() == 4);

        auto fromOffset = TextStringUtils::FindFirstOf(haystack, 6, needle, 2, 0, 2);
        REQUIRE_FALSE(fromOffset.IsEmpty());
        REQUIRE(fromOffset.Get() == 4);

        REQUIRE(TextStringUtils::FindFirstOf(haystack, 5, needle, 2, 1).IsEmpty());
        REQUIRE(TextStringUtils::FindFirstOf(haystack, 6, needle, 0, 0).IsEmpty());
    }
}

TEST_CASE("Find counted occurrences from the end", "[StringUtils]")
{
    SECTION("Character in a bounded buffer")
    {
        const Ck::TextChar haystack[] = {CK_CHAR('a'), CK_CHAR('B'), CK_CHAR('A'), CK_CHAR('b'), CK_CHAR('a')};

        auto secondCaseSensitive = TextStringUtils::FindLastOf(haystack, 5, CK_CHAR('a'), 1);
        REQUIRE_FALSE(secondCaseSensitive.IsEmpty());
        REQUIRE(secondCaseSensitive.Get() == 0);

        auto secondCaseInsensitive = TextStringUtils::FindLastOf(haystack, 5, CK_CHAR('a'), 1, false);
        REQUIRE_FALSE(secondCaseInsensitive.IsEmpty());
        REQUIRE(secondCaseInsensitive.Get() == 2);

        REQUIRE(TextStringUtils::FindLastOf(haystack, 5, CK_CHAR('a'), 3).IsEmpty());
        REQUIRE(TextStringUtils::FindLastOf(haystack, 5, CK_CHAR('z'), 0).IsEmpty());
    }

    SECTION("Substring in a null-terminated string")
    {
        auto last = TextStringUtils::FindLastOf(CK_TEXT("banana"), CK_TEXT("ana"), 0);
        REQUIRE_FALSE(last.IsEmpty());
        REQUIRE(last.Get() == 3);

        auto previous = TextStringUtils::FindLastOf(CK_TEXT("banana"), CK_TEXT("ana"), 1);
        REQUIRE_FALSE(previous.IsEmpty());
        REQUIRE(previous.Get() == 1);

        auto caseInsensitive = TextStringUtils::FindLastOf(CK_TEXT("bAnAnA"), CK_TEXT("ana"), 0, false);
        REQUIRE_FALSE(caseInsensitive.IsEmpty());
        REQUIRE(caseInsensitive.Get() == 3);

        REQUIRE(TextStringUtils::FindLastOf(CK_TEXT("bAnAnA"), CK_TEXT("ana"), 0).IsEmpty());
        REQUIRE(TextStringUtils::FindLastOf(CK_TEXT("banana"), CK_TEXT(""), 0).IsEmpty());
    }

    SECTION("Substring in a bounded buffer")
    {
        const Ck::TextChar haystack[] = {CK_CHAR('x'), CK_CHAR('a'), CK_CHAR('b'), CK_CHAR('x'), CK_CHAR('a'), CK_CHAR('b')};
        const Ck::TextChar needle[] = {CK_CHAR('a'), CK_CHAR('b')};

        auto previous = TextStringUtils::FindLastOf(haystack, 6, needle, 2, 1);
        REQUIRE_FALSE(previous.IsEmpty());
        REQUIRE(previous.Get() == 1);

        auto onlyInRange = TextStringUtils::FindLastOf(haystack, 5, needle, 2, 0);
        REQUIRE_FALSE(onlyInRange.IsEmpty());
        REQUIRE(onlyInRange.Get() == 1);

        REQUIRE(TextStringUtils::FindLastOf(haystack, 5, needle, 2, 1).IsEmpty());
        REQUIRE(TextStringUtils::FindLastOf(haystack, 6, needle, 0, 0).IsEmpty());
    }
}
