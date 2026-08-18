#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Application/ServiceContainer.hpp>

class InterfaceDummyService
{
public:

    virtual ~InterfaceDummyService() = default;
};

class DummyService : public InterfaceDummyService
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

class TestServiceContainer : public Ck::ServiceContainer
{
public:

    using ServiceContainer::Clear;
};

class InterfaceLifetimeService
{
public:

    virtual ~InterfaceLifetimeService() = default;
};

struct DestructionOrder
{
    int Next = 0;
    int Provider = 0;
    int Consumer = 0;
};

class LifetimeService : public InterfaceLifetimeService
{
public:

    explicit LifetimeService(DestructionOrder* destructionOrder) :
        mDestructionOrder(destructionOrder)
    {
    }

    ~LifetimeService() override
    {
        mDestructionOrder->Provider = ++mDestructionOrder->Next;
    }

private:

    DestructionOrder* mDestructionOrder;
};

class LifetimeConsumer
{
public:

    LifetimeConsumer(InterfaceLifetimeService* service, DestructionOrder* destructionOrder) :
        mService(service),
        mDestructionOrder(destructionOrder)
    {
    }

    ~LifetimeConsumer()
    {
        mDestructionOrder->Consumer = ++mDestructionOrder->Next;
    }

    InterfaceLifetimeService* GetService() const
    {
        return mService;
    }

private:

    InterfaceLifetimeService* mService;
    DestructionOrder* mDestructionOrder;
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
        return Ck::MakeUnique<DependencyService>(dummyService);
    });

    auto dummy = serviceContainer.Resolve<DummyService>();
    auto dependency = serviceContainer.Resolve<DependencyService>();

    REQUIRE(dummy == dependency->GetDummy());
}

TEST_CASE("Service container can resolve aliases", "[ServiceContainer]")
{
    Ck::ServiceContainer serviceContainer;
    serviceContainer.Singleton<DummyService>();
    serviceContainer.Alias<InterfaceDummyService, DummyService>();

    REQUIRE(serviceContainer.Resolve<InterfaceDummyService>());
}

TEST_CASE("Service container keeps an aliased dependency alive through teardown", "[ServiceContainer]")
{
    TestServiceContainer serviceContainer;
    DestructionOrder destructionOrder;

    serviceContainer.Alias<InterfaceLifetimeService, LifetimeService>();
    serviceContainer.Singleton<LifetimeConsumer>([&destructionOrder](InterfaceLifetimeService* service) {
        return Ck::MakeUnique<LifetimeConsumer>(service, &destructionOrder);
    });
    serviceContainer.Singleton<LifetimeService>([&destructionOrder]() {
        return Ck::MakeUnique<LifetimeService>(&destructionOrder);
    });

    LifetimeConsumer* consumer = serviceContainer.Resolve<LifetimeConsumer>();

    REQUIRE(consumer->GetService() == serviceContainer.Resolve<InterfaceLifetimeService>());

    serviceContainer.Clear();

    REQUIRE(destructionOrder.Consumer == 1);
    REQUIRE(destructionOrder.Provider == 2);
}

TEST_CASE("Service container safely replaces a binding waiting for a dependency", "[ServiceContainer]")
{
    TestServiceContainer serviceContainer;
    DestructionOrder destructionOrder;

    serviceContainer.Alias<InterfaceLifetimeService, LifetimeService>();
    serviceContainer.Singleton<LifetimeConsumer>([&destructionOrder](InterfaceLifetimeService* service) {
        return Ck::MakeUnique<LifetimeConsumer>(service, &destructionOrder);
    });
    serviceContainer.Singleton<LifetimeConsumer>([&destructionOrder](InterfaceLifetimeService* service) {
        return Ck::MakeUnique<LifetimeConsumer>(service, &destructionOrder);
    });
    serviceContainer.Singleton<LifetimeService>([&destructionOrder]() {
        return Ck::MakeUnique<LifetimeService>(&destructionOrder);
    });

    LifetimeConsumer* consumer = serviceContainer.Resolve<LifetimeConsumer>();

    REQUIRE(consumer->GetService() == serviceContainer.Resolve<InterfaceLifetimeService>());

    serviceContainer.Clear();

    REQUIRE(destructionOrder.Consumer == 1);
    REQUIRE(destructionOrder.Provider == 2);
}

TEST_CASE("Service container tracks dependencies when aliases are registered late", "[ServiceContainer]")
{
    TestServiceContainer serviceContainer;
    DestructionOrder destructionOrder;

    serviceContainer.Singleton<LifetimeConsumer>([&destructionOrder](InterfaceLifetimeService* service) {
        return Ck::MakeUnique<LifetimeConsumer>(service, &destructionOrder);
    });
    serviceContainer.Singleton<LifetimeService>([&destructionOrder]() {
        return Ck::MakeUnique<LifetimeService>(&destructionOrder);
    });
    serviceContainer.Alias<InterfaceLifetimeService, LifetimeService>();

    LifetimeConsumer* consumer = serviceContainer.Resolve<LifetimeConsumer>();

    REQUIRE(consumer->GetService() == serviceContainer.Resolve<InterfaceLifetimeService>());

    serviceContainer.Clear();

    REQUIRE(destructionOrder.Consumer == 1);
    REQUIRE(destructionOrder.Provider == 2);
}