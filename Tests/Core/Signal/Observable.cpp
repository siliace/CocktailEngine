#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Signal/Connection.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/Signal/Observable.hpp>

class TestObservable : public Ck::Observable
{
public:

	~TestObservable()
	{
		Ck::Observable::DisconnectAll();
	}

	void Update(int value)
	{
		mValue += value;
	}

	int GetValue() const
	{
		return mValue;
	}

private:

	int mValue = 0;
};

TEST_CASE("Handle free function connection with observable", "[Observable]")
{
	Ck::Signal<> signal;
	Ck::Observable observable;

	int value = 0;
	observable.Connect(signal, [&]() {
		value = 1;
	});

	signal.Emit();
	REQUIRE(value == 1);
}

TEST_CASE("Observable disconnect its connections when destroyed", "[Observable]")
{
	Ck::Signal<> signal;

	int value = 0;
	{
		TestObservable observable;

		observable.Connect(signal, [&]() {
			value += 1;
		});

		observable.Connect(signal, [&]() {
			value += 1;
		});
	}

	signal.Emit();
	REQUIRE(value == 0);
}