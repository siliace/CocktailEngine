#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Signal/Connection.hpp>
#include <CocktailEngine/Core/Signal/Signal.hpp>

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

TEST_CASE("Disconnect all slots while emitting a signal", "[Connection]")
{
    Ck::Signal<> signal;
    int firstCalls = 0;
    int laterCalls = 0;

    auto first = signal.Connect([&]() {
        ++firstCalls;
        signal.Disconnect();
    });
    auto later = signal.Connect([&]() {
        ++laterCalls;
    });

    signal.Emit();

    REQUIRE(firstCalls == 1);
    REQUIRE(laterCalls == 0);
    REQUIRE_FALSE(first.IsConnected());
    REQUIRE_FALSE(later.IsConnected());
    REQUIRE_FALSE(signal.IsBound());

    signal.Emit();
    REQUIRE(firstCalls == 1);
    REQUIRE(laterCalls == 0);
}

TEST_CASE("Disconnect a group while emitting a signal", "[Connection]")
{
    Ck::Signal<> signal;
    int groupCalls = 0;

    auto disconnecting = signal.Connect([&]() {
        signal.Disconnect(7);
    });
    auto firstInGroup = signal.Connect([&]() {
        ++groupCalls;
    }, 7);
    auto secondInGroup = signal.Connect([&]() {
        ++groupCalls;
    }, 7);

    signal.Emit();

    REQUIRE(groupCalls == 0);
    REQUIRE(disconnecting.IsConnected());
    REQUIRE_FALSE(firstInGroup.IsConnected());
    REQUIRE_FALSE(secondInGroup.IsConnected());
    REQUIRE(signal.IsBound());
}

TEST_CASE("Move-assigning a connection disconnects its previous slot", "[Connection]")
{
    Ck::Signal<> signal;
    int firstCalls = 0;
    int secondCalls = 0;

    auto first = signal.Connect([&]() {
        ++firstCalls;
    });
    auto second = signal.Connect([&]() {
        ++secondCalls;
    });

    first = Ck::Move(second);
    signal.Emit();

    REQUIRE(firstCalls == 0);
    REQUIRE(secondCalls == 1);
    REQUIRE(first.IsConnected());
    REQUIRE_FALSE(second.IsConnected());
}

TEST_CASE("Connections made during emission are deferred until the next emission", "[Connection]")
{
    Ck::Signal<> signal;
    Ck::Connection deferredConnection;
    int initialCalls = 0;
    int deferredCalls = 0;
    bool connected = false;

    auto initialConnection = signal.Connect([&]() {
        ++initialCalls;
        if (!connected)
        {
            connected = true;
            deferredConnection = signal.Connect([&]() {
                ++deferredCalls;
            });
        }
    });

    signal.Emit();
    REQUIRE(initialCalls == 1);
    REQUIRE(deferredCalls == 0);

    signal.Emit();
    REQUIRE(initialCalls == 2);
    REQUIRE(deferredCalls == 1);
}
