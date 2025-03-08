#include <catch2/catch_all.hpp>

#include <Cocktail/Core/System/Network/Address/IpAddressV4.hpp>

TEST_CASE("Convert an ipv4 address to a string", "[IpAddressV4]")
{
    REQUIRE(Ck::IpAddressV4::Localhost.ToString() == "127.0.0.1");
}