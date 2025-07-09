#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

TEST_CASE("Emit a signal to a free function", "[Signal]")
{
	Ck::Signal<int> signal;

	int value = 0;
	auto connection = signal.Connect([&](int integer) {
		value = integer;
	});

	signal.Emit(1);
	REQUIRE(value == 1);
}

TEST_CASE("Emit a signal to free functions with group ids", "[Signal]")
{
	Ck::Signal<int> signal;

	Ck::Array<int> values;
	auto connection1 = signal.Connect([&](int integer) {
		values.Add(5 + integer);
	}, 5);

	auto connection2 = signal.Connect([&](int integer) {
		values.Add(3 + integer);
	}, 3);

	auto connection3 = signal.Connect([&](int integer) {
		values.Add(7 + integer);
	}, 7);

	signal.Emit(1);
	REQUIRE(values[0] == 3 + 1);
	REQUIRE(values[1] == 5 + 1);
	REQUIRE(values[2] == 7 + 1);
}
