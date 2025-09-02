#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Plane.hpp>

TEMPLATE_TEST_CASE("Create a Plane from a point and a normal", "[Plane]", float, double)
{
    Ck::Vector3<TestType> point(0, 3, 0);
    Ck::Plane<TestType> plane(point, Ck::Vector3<TestType>::Up());

    SECTION("Its normal is normalized")
    {
        REQUIRE(plane.GetNormal().GetLength() == 1);
    }

    SECTION("Compute distance to the origin")
    {
        REQUIRE(plane.DistanceTo(Ck::Vector3<TestType>::Zero()) == -3);
    }
}

TEMPLATE_TEST_CASE("Create a Plane from three points", "[Plane]", float, double)
{
    Ck::Plane<TestType> plane(
        Ck::Vector3<TestType>(1, 3, 0),
        Ck::Vector3<TestType>(0, 3, 1),
        Ck::Vector3<TestType>(1, 3, 3)
    );

    SECTION("Its distance should be 3")
    {
        REQUIRE(plane.GetDistance() == -3);
    }

    SECTION("Its normal should be up up")
    {
        REQUIRE(plane.GetNormal() == Ck::Vector3<TestType>::Up());
    }
}

TEMPLATE_TEST_CASE("Compute distance between a plane and a point", "[Plane]", float, double)
{
    SECTION("Horizontal plane")
    {
        Ck::Plane<TestType> plane(-3, Ck::Vector3<TestType>::Up());
        
        SECTION("Distance to a point in front of the plane")
        {
            REQUIRE(plane.DistanceTo(Ck::Vector3<TestType>(1, 5, 3)) == 2);
        }
        
        SECTION("Distance to a point in behind of the plane")
        {
            REQUIRE(plane.DistanceTo(Ck::Vector3<TestType>(1, -5, 3)) == -8);
        }
        
        SECTION("Distance to a point in the plane")
        {
            REQUIRE(plane.DistanceTo(Ck::Vector3<TestType>(1, 3, 3)) == 0);
        }
    }
}