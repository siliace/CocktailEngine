#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Application/ServiceContainer.hpp>

class DummyService
{
};

class AliasService
{
};

class DependencyService
{
public:

	explicit DependencyService(DummyService* dummyService) :
		mDummy(dummyService)
	{
	}

	DummyService* GetDummy() const
	{
		return mDummy;
	}

private:

	DummyService* mDummy;
};

TEST_CASE("Service container can resolve bindings", "[ServiceContainer]")
{
	Ck::ServiceContainer serviceContainer;
	serviceContainer.Singleton<DummyService>();

	SECTION("Singleton bindings always return the same instance")
	{
		DummyService* instance1 = serviceContainer.Resolve<DummyService>();
		DummyService* instance2 = serviceContainer.Resolve<DummyService>();

		REQUIRE(instance1 == instance2);
	}
}

TEST_CASE("Service container can resolve bindings from other bindings callback", "[ServiceContainer]")
{
	Ck::ServiceContainer serviceContainer;
	serviceContainer.Singleton<DummyService>();
	serviceContainer.Singleton<DependencyService>([](DummyService* dummyService) {
		return std::make_unique<DependencyService>(dummyService);
	});

	auto dummy = serviceContainer.Resolve<DummyService>();
	auto dependency = serviceContainer.Resolve<DependencyService>();

	REQUIRE(dummy == dependency->GetDummy());
}

TEST_CASE("Service container can resolve aliases", "[ServiceContainer]")
{
	Ck::ServiceContainer serviceContainer;
	serviceContainer.Singleton<DummyService>();
	serviceContainer.Alias<AliasService, DummyService>();

	REQUIRE(serviceContainer.Resolve<DummyService>());
}