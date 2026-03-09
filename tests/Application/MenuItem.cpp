#include <catch2/catch_all.hpp>

#include <Futurewalker.Application.MenuItem.hpp>

using namespace Futurewalker;

TEST_CASE("MenuItem")
{
    SECTION("Constructor")
    {
        SECTION("Default")
        {
            auto const item = MenuItem(MenuItemType::Item);
            REQUIRE(item.GetType() == MenuItemType::Item);
            REQUIRE(item.GetId() == 0u);
            REQUIRE(item.GetTitle().IsEmpty());
            REQUIRE(item.GetSubtitle().IsEmpty());
            REQUIRE(item.GetSubMenu().GetItems().empty());

            auto const separator = MenuItem(MenuItemType::Separator);
            REQUIRE(separator.GetType() == MenuItemType::Separator);

            auto const subMenu = MenuItem(MenuItemType::SubMenu);
            REQUIRE(subMenu.GetType() == MenuItemType::SubMenu);
        }
    }

    SECTION("Properties")
    {
        auto item = MenuItem(MenuItemType::Item);

        REQUIRE(item.GetType() == MenuItemType::Item);

        item.SetType(MenuItemType::Separator);
        REQUIRE(item.GetType() == MenuItemType::Separator);
        item.SetType(MenuItemType::SubMenu);
        REQUIRE(item.GetType() == MenuItemType::SubMenu);

        auto const testId = MenuItemId(42u);
        item.SetId(testId);
        REQUIRE(item.GetId() == testId);
        auto const anotherId = MenuItemId(1337u);
        item.SetId(anotherId);
        REQUIRE(item.GetId() == anotherId);

        auto const testTitle = String(u8"Test Menu Item");
        item.SetTitle(testTitle);
        REQUIRE(item.GetTitle() == testTitle);
        auto const anotherTitle = String(u8"Another Title");
        item.SetTitle(anotherTitle);
        REQUIRE(item.GetTitle() == anotherTitle);

        auto const testSubtitle = String(u8"Test Subtitle");
        item.SetSubtitle(testSubtitle);
        REQUIRE(item.GetSubtitle() == testSubtitle);

        auto const anotherSubtitle = String(u8"Another Subtitle");
        item.SetSubtitle(anotherSubtitle);
        REQUIRE(item.GetSubtitle() == anotherSubtitle);


    }

    SECTION("SubMenu")
    {
        auto parentItem = MenuItem(MenuItemType::SubMenu);

        SECTION("Default submenu is empty")
        {
            REQUIRE(parentItem.GetSubMenu().GetItems().empty());
        }

        SECTION("Set and get submenu")
        {
            auto subMenuItems = MenuItemArray();
            subMenuItems.emplace_back(MenuItemType::Item);
            subMenuItems.emplace_back(MenuItemType::Separator);
            subMenuItems.emplace_back(MenuItemType::Item);
            
            subMenuItems[0].SetTitle(u8"First Item");
            subMenuItems[2].SetTitle(u8"Second Item");

            auto subMenu = Menu();
            subMenu.SetItems(subMenuItems);
            parentItem.SetSubMenu(subMenu);

            auto const& retrievedSubMenu = parentItem.GetSubMenu();
            REQUIRE(retrievedSubMenu.GetItems().size() == 3);
            REQUIRE(retrievedSubMenu.GetItems()[0].GetType() == MenuItemType::Item);
            REQUIRE(retrievedSubMenu.GetItems()[0].GetTitle() == u8"First Item");
            REQUIRE(retrievedSubMenu.GetItems()[1].GetType() == MenuItemType::Separator);
            REQUIRE(retrievedSubMenu.GetItems()[2].GetType() == MenuItemType::Item);
            REQUIRE(retrievedSubMenu.GetItems()[2].GetTitle() == u8"Second Item");
        }

        SECTION("Nested submenus")
        {
            auto nestedSubMenuItems = MenuItemArray();
            nestedSubMenuItems.emplace_back(MenuItemType::Item);
            nestedSubMenuItems[0].SetTitle(u8"Nested Item");

            auto nestedSubMenu = Menu();
            nestedSubMenu.SetItems(nestedSubMenuItems);

            auto nestedParent = MenuItem(MenuItemType::SubMenu);
            nestedParent.SetTitle(u8"Nested Parent");
            nestedParent.SetSubMenu(nestedSubMenu);

            auto topLevelSubMenuItems = MenuItemArray();
            topLevelSubMenuItems.push_back(nestedParent);

            auto topLevelSubMenu = Menu();
            topLevelSubMenu.SetItems(topLevelSubMenuItems);
            parentItem.SetSubMenu(topLevelSubMenu);

            auto const& retrievedSubMenu = parentItem.GetSubMenu();
            REQUIRE(retrievedSubMenu.GetItems().size() == 1);
            REQUIRE(retrievedSubMenu.GetItems()[0].GetType() == MenuItemType::SubMenu);
            REQUIRE(retrievedSubMenu.GetItems()[0].GetTitle() == u8"Nested Parent");
            REQUIRE(retrievedSubMenu.GetItems()[0].GetSubMenu().GetItems().size() == 1);
            REQUIRE(retrievedSubMenu.GetItems()[0].GetSubMenu().GetItems()[0].GetTitle() == u8"Nested Item");
        }
    }

    SECTION("Copy")
    {
        auto original = MenuItem(MenuItemType::Item);
        original.SetId(123u);
        original.SetTitle(u8"Original Title");
        original.SetSubtitle(u8"Original Subtitle");
        
        SECTION("Copy constructor")
        {
            auto copy = original;

            // Verify copy has same values
            REQUIRE(copy.GetType() == original.GetType());
            REQUIRE(copy.GetId() == original.GetId());
            REQUIRE(copy.GetTitle() == original.GetTitle());
            REQUIRE(copy.GetSubtitle() == original.GetSubtitle());
            
            // Verify copy-on-write: modifying copy doesn't affect original
            copy.SetTitle(u8"Modified Title");
            REQUIRE(copy.GetTitle() == u8"Modified Title");
            REQUIRE(original.GetTitle() == u8"Original Title");
        }

        SECTION("Assignment operator")
        {
            auto assigned = MenuItem(MenuItemType::Separator);
            assigned = original;
            
            // Verify assignment has same values
            REQUIRE(assigned.GetType() == original.GetType());
            REQUIRE(assigned.GetId() == original.GetId());
            REQUIRE(assigned.GetTitle() == original.GetTitle());
            REQUIRE(assigned.GetSubtitle() == original.GetSubtitle());
            
            // Verify copy-on-write: modifying assigned doesn't affect original
            assigned.SetSubtitle(u8"Modified Subtitle");
            REQUIRE(assigned.GetSubtitle() == u8"Modified Subtitle");
            REQUIRE(original.GetSubtitle() == u8"Original Subtitle");
        }
    }

    SECTION("Move")
    {
        auto original = MenuItem(MenuItemType::Item);
        original.SetId(456u);
        original.SetTitle(u8"Original Title");
        original.SetSubtitle(u8"Original Subtitle");

        SECTION("Move constructor")
        {
            auto moved = std::move(original);

            // Verify moved object has same values
            REQUIRE(moved.GetType() == MenuItemType::Item);
            REQUIRE(moved.GetId() == 456u);
            REQUIRE(moved.GetTitle() == u8"Original Title");
            REQUIRE(moved.GetSubtitle() == u8"Original Subtitle");

            REQUIRE(original.GetId() == 0u); // Default id after move
        }

        SECTION("Move assignment operator")
        {
            auto assigned = MenuItem(MenuItemType::Separator);
            assigned = std::move(original);

            // Verify assigned object has same values
            REQUIRE(assigned.GetType() == MenuItemType::Item);
            REQUIRE(assigned.GetId() == 456u);
            REQUIRE(assigned.GetTitle() == u8"Original Title");
            REQUIRE(assigned.GetSubtitle() == u8"Original Subtitle");

            // Verify original is in a valid but unspecified state
            REQUIRE(original.GetId() == 0u); // Default id after move
        }
    }
}