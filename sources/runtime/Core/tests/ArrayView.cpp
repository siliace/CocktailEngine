#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/ArrayView.hpp>
#include <CocktailEngine/Core/Exception.hpp>

#include <type_traits>

TEST_CASE("Construct an ArrayView", "[ArrayView]")
{
    SECTION("empty")
    {
        Ck::ArrayView<int> view;

        REQUIRE(view.IsEmpty());
        REQUIRE(view.GetSize() == 0);
        REQUIRE(view.GetData() == nullptr);
    }

    SECTION("from a pointer and an element count")
    {
        int values[] = { 2, 4, 6 };
        Ck::ArrayView<int> view(values, 3);

        REQUIRE_FALSE(view.IsEmpty());
        REQUIRE(view.GetSize() == 3);
        REQUIRE(view.GetData() == values);
        REQUIRE(view[0] == 2);
        REQUIRE(view[1] == 4);
        REQUIRE(view[2] == 6);
    }

    SECTION("from a C array")
    {
        int values[] = { 1, 3, 5, 7 };
        Ck::ArrayView<int> view(values);

        REQUIRE(view.GetSize() == 4);
        REQUIRE(view.GetData() == values);
    }

    SECTION("from an Array")
    {
        Ck::Array<int> values = { 8, 9, 10 };
        Ck::ArrayView<int> view(values);

        REQUIRE(view.GetSize() == values.GetSize());
        REQUIRE(view.GetData() == values.GetData());
    }

    SECTION("without copying the viewed elements")
    {
        int values[] = { 1, 2, 3 };
        Ck::ArrayView<int> view(values);

        values[1] = 42;

        REQUIRE(view[1] == 42);
        STATIC_REQUIRE(std::is_same_v<decltype(view[0]), const int&>);
        STATIC_REQUIRE(std::is_same_v<decltype(view.GetData()), const int*>);
    }
}

TEST_CASE("Access elements through an ArrayView", "[ArrayView]")
{
    int values[] = { 10, 20, 30 };
    Ck::ArrayView<int> view(values);

    SECTION("at a valid index")
    {
        REQUIRE(view.At(1) == 20);

        Ck::Optional<const int&> value = view.TryAt(1);
        REQUIRE_FALSE(value.IsEmpty());
        REQUIRE(value.Get() == 20);
    }

    SECTION("at an invalid index")
    {
        REQUIRE_THROWS_AS(view.At(3), Ck::ContainerOutOfRange);
        REQUIRE(view.TryAt(3).IsEmpty());
    }

    SECTION("at the first element")
    {
        REQUIRE(view.First() == 10);
        REQUIRE(view.TryFirst().Get() == 10);
    }

    SECTION("at the last element")
    {
        REQUIRE(view.Last() == 30);
        REQUIRE(view.TryLast().Get() == 30);
    }

    SECTION("in an empty view")
    {
        Ck::ArrayView<int> empty;

        REQUIRE_THROWS_AS(empty.At(0), Ck::ContainerOutOfRange);
        REQUIRE(empty.TryAt(0).IsEmpty());

        REQUIRE_THROWS_AS(empty.First(), Ck::ContainerEmpty);
        REQUIRE(empty.TryFirst().IsEmpty());

        REQUIRE_THROWS_AS(empty.Last(), Ck::ContainerEmpty);
        REQUIRE(empty.TryLast().IsEmpty());
    }
}

TEST_CASE("Slice an ArrayView", "[ArrayView]")
{
    int values[] = { 1, 2, 3, 4, 5 };
    Ck::ArrayView<int> view(values);

    SECTION("from an offset to the end")
    {
        Ck::ArrayView<int> slice = view.Slice(2);

        REQUIRE(slice.GetData() == values + 2);
        REQUIRE(slice.GetSize() == 3);
        REQUIRE(slice[0] == 3);
        REQUIRE(slice[2] == 5);
    }

    SECTION("from an offset with an element count")
    {
        Ck::ArrayView<int> slice = view.Slice(1, 2);

        REQUIRE(slice.GetData() == values + 1);
        REQUIRE(slice.GetSize() == 2);
        REQUIRE(slice[0] == 2);
        REQUIRE(slice[1] == 3);
    }

    SECTION("with a count greater than the remaining size")
    {
        Ck::ArrayView<int> slice = view.Slice(3, 20);

        REQUIRE(slice.GetSize() == 2);
        REQUIRE(slice[0] == 4);
        REQUIRE(slice[1] == 5);
    }

    SECTION("at the end")
    {
        Ck::ArrayView<int> slice = view.Slice(view.GetSize());

        REQUIRE(slice.IsEmpty());
        REQUIRE(slice.GetData() == values + view.GetSize());
    }

    SECTION("past the end")
    {
        REQUIRE_THROWS_AS(view.Slice(view.GetSize() + 1), Ck::ContainerOutOfRange);
        REQUIRE_THROWS_AS(view.Slice(view.GetSize() + 1, 1), Ck::ContainerOutOfRange);
    }
}

TEST_CASE("Find elements in an ArrayView", "[ArrayView]")
{
    int values[] = { 4, 7, 4, 9 };
    Ck::ArrayView<int> view(values);

    REQUIRE(view.FindIndex(4).Get() == 0);
    REQUIRE(view.FindIndex(4, 1).Get() == 2);
    REQUIRE(view.FindIndex(4, 3).IsEmpty());
    REQUIRE(view.FindIndex(12).IsEmpty());

    REQUIRE(view.FindLastIndex(4).Get() == 2);
    REQUIRE(view.FindLastIndex(7).Get() == 1);
    REQUIRE(view.FindLastIndex(12).IsEmpty());

    REQUIRE(view.Contains(7));
    REQUIRE_FALSE(view.Contains(12));

    Ck::ArrayView<int> empty;
    REQUIRE(empty.FindIndex(4).IsEmpty());
    REQUIRE(empty.FindLastIndex(4).IsEmpty());
    REQUIRE_FALSE(empty.Contains(4));
}

TEST_CASE("Check ArrayView prefixes and suffixes", "[ArrayView]")
{
    int values[] = { 1, 2, 3, 4 };
    int prefixValues[] = { 1, 2 };
    int suffixValues[] = { 3, 4 };
    int otherValues[] = { 2, 3 };
    int longerValues[] = { 1, 2, 3, 4, 5 };

    Ck::ArrayView<int> view(values);
    Ck::ArrayView<int> prefix(prefixValues);
    Ck::ArrayView<int> suffix(suffixValues);
    Ck::ArrayView<int> other(otherValues);
    Ck::ArrayView<int> longer(longerValues);
    Ck::ArrayView<int> empty;

    REQUIRE(view.StartsWith(prefix));
    REQUIRE_FALSE(view.StartsWith(other));
    REQUIRE_FALSE(view.StartsWith(longer));
    REQUIRE(view.StartsWith(empty));

    REQUIRE(view.EndsWith(suffix));
    REQUIRE_FALSE(view.EndsWith(other));
    REQUIRE_FALSE(view.EndsWith(longer));
    REQUIRE(view.EndsWith(empty));
}

TEST_CASE("Compare ArrayViews", "[ArrayView]")
{
    int lhsValues[] = { 1, 2, 3 };
    int equalValues[] = { 1, 2, 3 };
    int differentValues[] = { 1, 2, 4 };
    int shorterValues[] = { 1, 2 };

    Ck::ArrayView<int> lhs(lhsValues);
    Ck::ArrayView<int> same(lhsValues);
    Ck::ArrayView<int> equal(equalValues);
    Ck::ArrayView<int> different(differentValues);
    Ck::ArrayView<int> shorter(shorterValues);

    REQUIRE(lhs == same);
    REQUIRE(lhs == equal);
    REQUIRE_FALSE(lhs != equal);
    REQUIRE(lhs != different);
    REQUIRE(lhs != shorter);
    REQUIRE(Ck::ArrayView<int>() == Ck::ArrayView<int>());
}

TEST_CASE("Iterate over an ArrayView", "[ArrayView]")
{
    int values[] = { 2, 4, 6, 8 };
    Ck::ArrayView<int> view(values);

    using Iterator = Ck::ArrayView<int>::Iterator;
    using ConstIterator = Ck::ArrayView<int>::ConstIterator;

    STATIC_REQUIRE(std::is_same_v<Iterator, ConstIterator>);
    STATIC_REQUIRE(std::is_same_v<typename std::iterator_traits<ConstIterator>::reference, const int&>);
    STATIC_REQUIRE(std::is_same_v<typename std::iterator_traits<ConstIterator>::pointer, const int*>);

    SECTION("with the Array-style iterator API")
    {
        ConstIterator first = view.GetIterator();
        ConstIterator last = view.GetLastIterator();
        ConstIterator end = view.GetEndIterator();

        REQUIRE(first.IsValid());
        REQUIRE(first.GetValue() == 2);
        REQUIRE(*first == 2);
        REQUIRE(first.operator->() == values);
        REQUIRE(last.IsValid());
        REQUIRE(last.GetValue() == 8);
        REQUIRE_FALSE(end.IsValid());

        REQUIRE(first.Next().GetValue() == 4);
        REQUIRE(last.Previous().GetValue() == 6);
        REQUIRE(last.Next() == end);
        REQUIRE(end.Previous() == last);
    }

    SECTION("with random-access operations")
    {
        ConstIterator iterator = view.GetIterator();

        REQUIRE((iterator + 2).GetValue() == 6);
        REQUIRE((2 + iterator).GetValue() == 6);

        iterator += 3;
        REQUIRE(iterator.GetValue() == 8);

        iterator -= 2;
        REQUIRE(iterator.GetValue() == 4);
        REQUIRE((iterator - 1).GetValue() == 2);

        REQUIRE(view.GetEndIterator() - view.GetIterator() == 4);
        REQUIRE(view.GetIterator() < view.GetLastIterator());
        REQUIRE(view.GetIterator() <= view.GetIterator());
        REQUIRE(view.GetLastIterator() > view.GetIterator());
        REQUIRE(view.GetLastIterator() >= view.GetLastIterator());
    }

    SECTION("with increment and decrement operators")
    {
        ConstIterator iterator = view.GetIterator();

        REQUIRE(*(iterator++) == 2);
        REQUIRE(*iterator == 4);
        REQUIRE(*(++iterator) == 6);
        REQUIRE(*(iterator--) == 6);
        REQUIRE(*iterator == 4);
        REQUIRE(*(--iterator) == 2);
    }

    SECTION("with free begin and end functions")
    {
        REQUIRE(Ck::begin(view) == view.GetIterator());
        REQUIRE(Ck::end(view) == view.GetEndIterator());

        int sum = 0;
        for (int value : view)
            sum += value;

        REQUIRE(sum == 20);
    }

    SECTION("over an empty view")
    {
        Ck::ArrayView<int> empty;

        REQUIRE(empty.GetIterator() == empty.GetEndIterator());
        REQUIRE_FALSE(empty.GetIterator().IsValid());
        REQUIRE_FALSE(empty.GetLastIterator().IsValid());
        REQUIRE(Ck::begin(empty) == Ck::end(empty));
    }
}
