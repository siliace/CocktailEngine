#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/LinkedList.hpp>
#include <Cocktail/Core/Memory/Allocator/SizedLinearAllocator.hpp>

TEMPLATE_TEST_CASE("Create an empty linked list", "[LinkedList]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::LinkedList<int, TestType> list;

    REQUIRE(list.IsEmpty());
    REQUIRE(list.GetSize() == 0);
}

TEMPLATE_TEST_CASE("Copy a linked list", "[LinkedList]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::LinkedList<int, TestType> list1;
    list1.AddBack(1);
    list1.AddBack(2);
    list1.AddBack(3);

    SECTION ("By constructor")
    {
        Ck::LinkedList<int, TestType> list2(list1);
    
        REQUIRE_FALSE(list2.IsEmpty());
        REQUIRE(list2.GetSize() == 3);

        typename Ck::LinkedList<int, TestType>::ConstIterator lhs = list1.GetIterator();
        typename Ck::LinkedList<int, TestType>::ConstIterator rhs = list2.GetIterator();
        while (lhs.IsValid() || rhs.IsValid())
        {
            REQUIRE(lhs.IsValid());
            REQUIRE(rhs.IsValid());
            REQUIRE(lhs.GetValue() == rhs.GetValue());
            
            lhs.Advance();
            rhs.Advance();
        }
    }
    
    SECTION("By assignation")
    {
        Ck::LinkedList<int, TestType> list2 = list1;
    
        REQUIRE_FALSE(list2.IsEmpty());
        REQUIRE(list2.GetSize() == 3);

        typename Ck::LinkedList<int, TestType>::ConstIterator lhs = list1.GetIterator();
        typename Ck::LinkedList<int, TestType>::ConstIterator rhs = list2.GetIterator();
        while (lhs.IsValid() || rhs.IsValid())
        {
            REQUIRE(lhs.IsValid());
            REQUIRE(rhs.IsValid());
            REQUIRE(lhs.GetValue() == rhs.GetValue());
            
            lhs.Advance();
            rhs.Advance();
        }
    }
}

TEMPLATE_TEST_CASE("Move a linked list", "[LinkedList]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::LinkedList<int, TestType> list1;
    list1.AddBack(1);
    list1.AddBack(2);
    list1.AddBack(3);

    SECTION ("By constructor")
    {
        Ck::LinkedList<int, TestType> list2(std::move(list1));
    
        REQUIRE(list1.IsEmpty());
        REQUIRE_FALSE(list2.IsEmpty());
        
        REQUIRE(list1.GetSize() == 0);
        REQUIRE(list2.GetSize() == 3);

        typename Ck::LinkedList<int, TestType>::ConstIterator it = list2.GetIterator();
        REQUIRE(it.GetValue() == 1); it.Advance();
        REQUIRE(it.GetValue() == 2); it.Advance();
        REQUIRE(it.GetValue() == 3);
    }
    
    SECTION("By assignation")
    {
        Ck::LinkedList<int, TestType> list2 = std::move(list1);
    
        REQUIRE(list1.IsEmpty());
        REQUIRE_FALSE(list2.IsEmpty());
        
        REQUIRE(list1.GetSize() == 0);
        REQUIRE(list2.GetSize() == 3);

        typename Ck::LinkedList<int, TestType>::ConstIterator it = list2.GetIterator();
        REQUIRE(it.GetValue() == 1); it.Advance();
        REQUIRE(it.GetValue() == 2); it.Advance();
        REQUIRE(it.GetValue() == 3);
    }
}

TEMPLATE_TEST_CASE("Create a linked list with repeated values", "[LinkedList]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::LinkedList<int, TestType> list(5, 10);

    REQUIRE_FALSE(list.IsEmpty());

    unsigned int count = 0;

    typename Ck::LinkedList<int, TestType>::Iterator it(list);
    while (it.IsValid())
    {
        REQUIRE(it.GetValue() == 10);
        ++count;

        it.Advance();
    }

    REQUIRE(count == 5);
}

TEMPLATE_TEST_CASE("Add elements to the linked list", "[LinkedList]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::LinkedList<int, TestType> list;

    list.AddBack(2);
    list.AddBack(3);
    list.AddBack(5);

    SECTION("To the head")
    {
        list.AddFront(1);

        REQUIRE(list.GetIterator().GetValue() == 1);
        REQUIRE(list.GetSize() == 4);
    }

    SECTION("To the tail")
    {
        list.AddBack(8);

        REQUIRE(list.GetLastIterator().GetValue() == 8);
        REQUIRE(list.GetSize() == 4);
    }

    SECTION("From an iterator a the head of the list")
    {
        list.Insert(list.GetIterator(), 1);

        REQUIRE(list.GetIterator().GetValue() == 1);
        REQUIRE(list.GetSize() == 4);
    }

    SECTION("From an iterator in the middle of the list")
    {
        auto it = list.GetIterator().Next();
        list.Insert(it, 42);

        REQUIRE(list.GetSize() == 4);
        REQUIRE(list.GetIterator().GetValue() == 2);
        REQUIRE(list.GetIterator().Next().GetValue() == 42);
        REQUIRE(list.GetLastIterator().Previous().GetValue() == 3);
        REQUIRE(list.GetLastIterator().GetValue() == 5);
    }

    SECTION("From an iterator a the tail of the list")
    {
        list.Insert(list.GetLastIterator().Next(), 8);

        REQUIRE(list.GetLastIterator().GetValue() == 8);
        REQUIRE(list.GetLastIterator().Previous().GetValue() == 5);
        REQUIRE(list.GetSize() == 4);
    }
}

TEMPLATE_TEST_CASE("Check if list contains an element", "[LinkedList]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::LinkedList<int, TestType> list;

    list.AddBack(2);
    list.AddBack(3);
    list.AddBack(5);

    SECTION("With an contained element")
    {
        REQUIRE(list.FindFirst(3).IsValid());
    }

    SECTION("With an not contained element")
    {
        REQUIRE_FALSE(list.FindFirst(42).IsValid());
    }
}

TEMPLATE_TEST_CASE("Remove an element from the linked list", "[LinkedList]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::LinkedList<int, TestType> list;

    list.AddBack(1);
    list.AddBack(2);
    list.AddBack(3);
    list.AddBack(5);
    list.AddBack(8);

    SECTION("With a valid iterator")
    {
        typename Ck::LinkedList<int, TestType>::Iterator it = list.FindFirst(3);
        REQUIRE(it.IsValid());

        it = list.Remove(it);

        REQUIRE(list.GetSize() == 4);
        REQUIRE(it.GetValue() == 5);
    }

    SECTION("With a value that does not exists in the list")
    {
        typename Ck::LinkedList<int, TestType>::Iterator it = list.FindFirst(42);
        REQUIRE_FALSE(it.IsValid());
    }
}

TEMPLATE_TEST_CASE("Clear a linked list", "[LinkedList]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::LinkedList<int, TestType> list;

    list.AddBack(1);
    list.AddBack(2);
    list.AddBack(3);
    list.AddBack(5);
    list.AddBack(8);

    list.Clear();

    REQUIRE(list.GetSize() == 0);
    REQUIRE_FALSE(list.GetIterator().IsValid());
}

TEMPLATE_TEST_CASE("Iterate over elements of a linked list", "[LinkedList]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::LinkedList<int, TestType> list;

    list.AddBack(1);
    list.AddBack(2);
    list.AddBack(3);
    list.AddBack(4);
    list.AddBack(5);

    SECTION("In normal order")
    {
        int previous = 0;
        for (typename Ck::LinkedList<int, TestType>::Iterator it = list.GetIterator(); it.IsValid(); it = it.Next())
        {
            REQUIRE(previous + 1 == *it);

            previous = *it;
        }
    }

    SECTION("In revered order")
    {
        int previous = 6;
        for (typename Ck::LinkedList<int, TestType>::Iterator it = list.GetLastIterator(); it.IsValid(); it = it.Previous())
        {
            REQUIRE(previous - 1 == *it);

            previous = *it;
        }
    }
}
