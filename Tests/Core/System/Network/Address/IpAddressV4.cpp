#include <catch2/catch_all.hpp>

#include <Cocktail/Core/System/Network/Address/IpAddressV4.hpp>

static constexpr Ck::Uint32 LocalhostPacked = Ck::Pack(127, 0, 0, 1);

TEST_CASE("Create an ipv4 address from a packed Uint32", "[IpAddressV4]")
{
	Ck::IpAddressV4 address(LocalhostPacked);

	REQUIRE(address.GetByte(0) == 127);
	REQUIRE(address.GetByte(1) == 0);
	REQUIRE(address.GetByte(2) == 0);
	REQUIRE(address.GetByte(3) == 1);
}

TEST_CASE("Create an ipv4 address from four bytes", "[IpAddressV4]")
{
	Ck::IpAddressV4 address(127, 0, 0, 1);

	REQUIRE(address.GetByte(0) == 127);
	REQUIRE(address.GetByte(1) == 0);
	REQUIRE(address.GetByte(2) == 0);
	REQUIRE(address.GetByte(3) == 1);
}

TEST_CASE("Convert an ipv4 address to a string", "[IpAddressV4]")
{
    REQUIRE(Ck::IpAddressV4::Localhost.ToString() == CK_TEXT("127.0.0.1"));
}

TEST_CASE("Convert an ipv4 address to a packed Uint32", "[IpAddressV4]")
{
	REQUIRE(Ck::IpAddressV4::Localhost.ToInteger() == LocalhostPacked);
}

TEST_CASE("Protocol of an ipv4 address is always IpV4", "[IpAddressV4]")
{
	REQUIRE(Ck::IpAddressV4(127, 0, 0, 1).GetProtocol() == Ck::IpAddress::Protocol::Ipv4);
}