#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

class TestClass
{
public:

	static bool StaitcMethodCalled;
	bool MemberMethodCalled = false;
	mutable bool ConstMemberMethodCalled = false;

	static void StaticCall(bool called)
	{
		StaitcMethodCalled = called;
	}

	void MethodCall(bool called)
	{
		MemberMethodCalled = called;
	}

	void ConstMethodCall() const
	{
		ConstMemberMethodCalled = true;
	}
};

bool TestClass::StaitcMethodCalled = false;

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

TEST_CASE("Emit a signal to a member method", "[Signal]")
{
	Ck::Signal<bool> signal;

	TestClass testObject;
	auto connection = signal.Connect(testObject, &TestClass::MethodCall);
	signal.Emit(true);

	REQUIRE(testObject.MemberMethodCalled);
}

TEST_CASE("Emit a signal to a constant member method", "[Signal]")
{
	Ck::Signal<> signal;

	TestClass testObject;
	auto connection = signal.Connect(testObject, &TestClass::ConstMethodCall);
	signal.Emit();

	REQUIRE(testObject.ConstMemberMethodCalled);
}

TEST_CASE("Emit a signal to a static method", "[Signal]")
{
	Ck::Signal<bool> signal;

	auto connection = signal.Connect(&TestClass::StaticCall);
	signal.Emit(true);

	REQUIRE(TestClass::StaitcMethodCalled);
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
