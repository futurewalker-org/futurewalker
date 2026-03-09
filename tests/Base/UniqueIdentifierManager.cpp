#include <catch2/catch_all.hpp>

#include <Futurewalker.Base.UniqueIdentifierManager.hpp>
#include <Futurewalker.Base.UniqueIdentifier.hpp>

using namespace Futurewalker;

TEST_CASE("UniqueIdentifierManager")
{
    REQUIRE_THROWS(UniqueIdentifierManager::Generate<Id>());
    auto manager = Locator::ResolveWithDefault<UniqueIdentifierManager>();
    REQUIRE(manager);
    REQUIRE(UniqueIdentifierManager::Generate<Id>() != Id(0U));

    UniqueId id1;
    UniqueId id2;
    REQUIRE(Id(id1) != Id(id2));
}
