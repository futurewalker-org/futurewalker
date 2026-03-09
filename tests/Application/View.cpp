#include <catch2/catch_all.hpp>

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.ViewLayerManager.hpp>

#include <Futurewalker.Base.PlatformDebug.hpp>

#include <Futurewalker.Test.TestPlatformDebug.hpp>
#include <Futurewalker.Test.TestPlatformViewLayerContext.hpp>
#include <Futurewalker.Test.TestView.hpp>

using namespace Futurewalker;

TEST_CASE("View")
{
    Locator::Register<UniqueIdentifierManager>();
    Locator::Register<ViewLayerManager>();
    Locator::Register<TestPlatformDebug>();
    Locator::Register<TestPlatformViewLayerContext>();

    auto const uniqueIdManager = Locator::Resolve<UniqueIdentifierManager>();
    auto const viewLayerManager = Locator::Resolve<ViewLayerManager>();

    SECTION("Init")
    {
        auto view = TestView::Make();
        REQUIRE(view);
        REQUIRE(view->IsVisible());
        REQUIRE(view->IsVisibleFromRoot());
        REQUIRE(view->IsEnabled());
        REQUIRE(view->IsEnabledFromRoot());
        REQUIRE(!view->IsActive());
        REQUIRE(!view->IsAttached());
        REQUIRE(!view->IsFocused());
        REQUIRE(view->GetDisplayScale() == 1.0);
        REQUIRE(view->GetBackingScale() == 1.0);
        REQUIRE(view->GetLayoutDirection() == LayoutDirection::LeftToRight);
        REQUIRE(view->GetRawLayoutDirection() == ViewLayoutDirection::Inherited);
        REQUIRE(view->GetChildCount() == 0);
        REQUIRE(view->GetChildren().empty());
    }
}
