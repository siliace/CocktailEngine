#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/PropertyTree/PropertyTree.hpp>

TEST_CASE("Insert a property tree node in another one", "[PropertyTreeElement]")
{
    Ck::Properties::ElementType element;

    element.Insert(CK_TEXT("name"), Ck::Properties::ValueType(CK_TEXT("John")));
    element.Insert(CK_TEXT("job"), Ck::Properties::ValueType(CK_TEXT("firemane")));
    element.Insert(CK_TEXT("age"), Ck::Properties::ValueType(42));

    REQUIRE(element.GetChildCount() == 3);

    SECTION("Insert in front")
    {
        element.FrontInsert(CK_TEXT("city"), Ck::Properties::ValueType(CK_TEXT("Rouen")));

        Ck::Properties::NodeType* firstChild = element.GetFirstChild();
        REQUIRE(firstChild->GetParent() == &element);
        REQUIRE(firstChild->GetName() == CK_TEXT("city"));
        REQUIRE(firstChild->GetType() == Ck::Properties::NodeType::Type::Value);
    }

    SECTION("Before another one")
    {
        Ck::Properties::NodeType* lastChild = element.GetLastChild();

        element.InsertBefore(lastChild, CK_TEXT("city"), Ck::Properties::ValueType(CK_TEXT("Rouen")));

        REQUIRE(lastChild->GetPreviousSibling()->GetParent() == &element);
        REQUIRE(lastChild->GetPreviousSibling()->GetName() == CK_TEXT("city"));
        REQUIRE(lastChild->GetPreviousSibling()->GetType() == Ck::Properties::NodeType::Type::Value);
    }

    SECTION("Insert after another one")
    {
        Ck::Properties::NodeType* firstChild = element.GetFirstChild();

        element.InsertAfter(firstChild, CK_TEXT("city"), Ck::Properties::ValueType(CK_TEXT("Rouen")));

        REQUIRE(firstChild->GetNextSibling()->GetParent() == &element);
        REQUIRE(firstChild->GetNextSibling()->GetName() == CK_TEXT("city"));
        REQUIRE(firstChild->GetNextSibling()->GetType() == Ck::Properties::NodeType::Type::Value);
    }

    SECTION("Insert in back")
    {
        element.Insert(CK_TEXT("city"), Ck::Properties::ValueType(CK_TEXT("Rouen")));

        Ck::Properties::NodeType* lastChild = element.GetLastChild();
        REQUIRE(lastChild->GetParent() == &element);
        REQUIRE(lastChild->GetName() == CK_TEXT("city"));
        REQUIRE(lastChild->GetType() == Ck::Properties::NodeType::Type::Value);
    }
}

TEST_CASE("Remove a child node from a property tree element", "[PropertyTreeElement]")
{
    Ck::Properties::ElementType element;
    element.Insert(CK_TEXT("name"), Ck::Properties::ValueType(CK_TEXT("John")));
    element.Insert(CK_TEXT("job"), Ck::Properties::ValueType(CK_TEXT("firemane")));
    element.Insert(CK_TEXT("age"), Ck::Properties::ValueType(42));
    element.Insert(CK_TEXT("city"), Ck::Properties::ValueType(CK_TEXT("Rouen")));
    element.Insert(CK_TEXT("zip"), Ck::Properties::ValueType(76000));

    SECTION("Remove this first child")
    {
        element.Remove(CK_TEXT("name"));

        REQUIRE(element.GetFirstChild()->GetName() == CK_TEXT("job"));
    }

    SECTION("Remove the last child")
    {
        element.Remove(CK_TEXT("zip"));

        REQUIRE(element.GetLastChild()->GetName() == CK_TEXT("city"));
    }
}