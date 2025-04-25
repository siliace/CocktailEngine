#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Plane.hpp>

TEST_CASE("Create a Plane from a point and a normal", "[Plane]")
{
    Ck::Vector3<float> point(0.f, 3.f, 0.f);
    Ck::Plane<float> plane(point, Ck::Vector3<float>::Up());

    SECTION("Its normal is normalized")
    {
        REQUIRE(plane.GetNormal().GetLength() == 1.f);
    }

    SECTION("Compute distance to the origin")
    {
        REQUIRE(plane.DistanceTo(Ck::Vector3<float>::Zero()) == -3.f);
    }
}

TEST_CASE("Create a Plane from three points", "[Plane]")
{
    Ck::Plane<float> plane(
        Ck::Vector3<float>(1.f, 3.f, 0.f),
        Ck::Vector3<float>(0.f, 3.f, 1.f),
        Ck::Vector3<float>(1.f, 3.f, 3.f)
    );

    SECTION("Its distance should be 3")
    {
        REQUIRE(plane.GetDistance() == -3.f);
    }

    SECTION("Its normal should be up up")
    {
        REQUIRE(plane.GetNormal() == Ck::Vector3<float>::Up());
    }
}

TEST_CASE("Compute distance between a plane and a point", "[Plane]")
{
    SECTION("Horizontal plane")
    {
        Ck::Plane<float> plane(-3.f, Ck::Vector3<float>::Up());
        
        SECTION("Distance to a point in front of the plane")
        {
            REQUIRE(plane.DistanceTo(Ck::Vector3<float>(1.f, 5.f, 3.f)) == 2.f);
        }
        
        SECTION("Distance to a point in behind of the plane")
        {
            REQUIRE(plane.DistanceTo(Ck::Vector3<float>(1.f, -5.f, 3.f)) == -8.f);
        }
        
        SECTION("Distance to a point in the plane")
        {
            REQUIRE(plane.DistanceTo(Ck::Vector3<float>(1.f, 3.f, 3.f)) == 0.f);
        }
    }
}