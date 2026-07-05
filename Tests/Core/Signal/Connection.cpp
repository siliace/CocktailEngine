#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Signal/Connection.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

TEST_CASE("Disconnect manually a signal", "[Connection]")
{
	int value = 0;
	Ck::Signal<> signal;
	auto connection = signal.Connect([&]() {
		value = 1;
	});

	signal.Emit();

	REQUIRE(value == 1);
	REQUIRE(connection.IsConnected());

	connection.Disconnect();
	signal.Emit();

	REQUIRE(value == 1);
	REQUIRE_FALSE(connection.IsConnected());
}

TEST_CASE("Connect a signal while emitting it", "[Connection]")
{
    int value = 0;
    Ck::Signal<> signal;
    Ck::Connection connection, connection2;
    connection = signal.Connect([&]() {
        value = 1;
        connection2 = signal.Connect([&]() {
            value = 2;
        });
    });

    signal.Emit();
    REQUIRE(value == 1);
    REQUIRE(connection.IsConnected());
    REQUIRE(connection2.IsConnected());
}

TEST_CASE("Disconnect a signal while emitting it", "[Connection]")
{
    int value = 0;
    Ck::Signal<> signal;
    Ck::Connection connection;
    connection = signal.Connect([&]() {
        value = 1;
        connection.Disconnect();
    });

    signal.Emit();

    REQUIRE(value == 1);
    REQUIRE_FALSE(connection.IsConnected());

    signal.Emit();

    REQUIRE(value == 1);
    REQUIRE_FALSE(connection.IsConnected());
}

TEST_CASE("Disconnect a signal when connection leave its scope", "[Connection]")
{
	int value = 0;
	Ck::Signal<> signal;
	{
		auto connection = signal.Connect([&]() {
			value = 1;
		});
	}

	signal.Emit();

	REQUIRE(value == 0);
}

TEST_CASE("Disable a connection", "[Connection]")
{
	int value = 0;
	Ck::Signal<> signal;
	auto connection = signal.Connect([&]() {
		value = 1;
	});

	signal.Emit();

	REQUIRE(value == 1);
	REQUIRE_FALSE(connection.IsDisabled());

	connection.Disable();
	signal.Emit();

	REQUIRE(value == 1);
	REQUIRE(connection.IsDisabled());
}