#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Application/ServiceContainer.hpp>

class DummyService : public Ck::Extends<DummyService, Ck::Object>
{
};

class AliasService : public Ck::Extends<AliasService, Ck::Object>
{
};

class DependencyService : public Ck::Extends<DependencyService, Ck::Object>
{
public:

	explicit DependencyService(const Ck::Ref<DummyService>& dummyService) :
		mDummy(dummyService)
	{
	}

	const Ck::Ref<DummyService>& GetDummy() const
	{
		return mDummy;
	}

private:

	Ck::Ref<DummyService> mDummy;
};

TEST_CASE("Service container can resolve bindings", "[ServiceContainer]")
{
	Ck::Ref<Ck::ServiceContainer> serviceContainer = Ck::ServiceContainer::New();
	serviceContainer->Singleton<DummyService>();
	serviceContainer->Factory<DependencyService>([](const Ck::Ref<DummyService>& dummyService) {
		return DependencyService::New(dummyService);
	});

	SECTION("Singleton bindings always return the same instance")
	{
		Ck::Ref<DummyService> instance1 = serviceContainer->Resolve<DummyService>();
		Ck::Ref<DummyService> instance2 = serviceContainer->Resolve<DummyService>();

		REQUIRE(instance1 == instance2);
	}

	SECTION("Factory bindings never return the same instance")
	{
		Ck::Ref<DependencyService> instance1 = serviceContainer->Resolve<DependencyService>();
		Ck::Ref<DependencyService> instance2 = serviceContainer->Resolve<DependencyService>();

		REQUIRE(instance1 != instance2);
	}
}

TEST_CASE("Service container can resolve bindings from other bindings callback", "[ServiceContainer]")
{
	Ck::Ref<Ck::ServiceContainer> serviceContainer = Ck::ServiceContainer::New();
	serviceContainer->Singleton<DummyService>();
	serviceContainer->Factory<DependencyService>([](const Ck::Ref<DummyService>& dummyService) {
		return DependencyService::New(dummyService);
	});

	auto dummy = serviceContainer->Resolve<DummyService>();
	auto dependency = serviceContainer->Resolve<DependencyService>();

	REQUIRE(dummy == dependency->GetDummy());
}

TEST_CASE("Service container can resolve aliases", "[ServiceContainer]")
{
	Ck::Ref<Ck::ServiceContainer> serviceContainer = Ck::ServiceContainer::New();
	serviceContainer->Singleton<DummyService>();
	serviceContainer->Alias<AliasService, DummyService>();

	REQUIRE(serviceContainer->Resolve<DummyService>());
}