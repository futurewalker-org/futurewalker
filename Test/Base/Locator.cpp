#include <catch2/catch_all.hpp>

#include <Futurewalker.Base.Locator.hpp>

using namespace Futurewalker;

class TestDependency
{
};

class TestSingleton
{
public:
    explicit TestSingleton(Shared<TestDependency> dep)
      : _dep {dep}
    {
    }

    auto GetDependency()
    {
        return _dep;
    }

private:
    Shared<TestDependency> _dep;
};

template <>
struct Locator::Resolver<TestDependency>
{
    using Interface = TestDependency;
    static auto Resolve() -> Shared<Interface>
    {
        return Shared<TestDependency>::Make();
    }
};

template <>
struct Locator::Resolver<TestSingleton>
{
    using Interface = TestSingleton;
    static auto Resolve() -> Shared<Interface>
    {
        auto dep = Locator::Resolve<TestDependency>();
        return Shared<TestSingleton>::Make(dep);
    }
};

TEST_CASE("Locator")
{
    REQUIRE_THROWS(Locator::Resolve<TestSingleton>());
    Locator::Register<TestDependency>();
    REQUIRE_THROWS(Locator::Resolve<TestSingleton>());
    Locator::Register<TestSingleton>();
    const auto singleton = Locator::Resolve<TestSingleton>();
    REQUIRE(singleton);
    REQUIRE(singleton == Locator::GetInstance<TestSingleton>());
    REQUIRE(singleton->GetDependency() == Locator::GetInstance<TestDependency>());
}
