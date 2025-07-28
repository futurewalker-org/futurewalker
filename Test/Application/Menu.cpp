#include <catch2/catch_all.hpp>

#include <Futurewalker.Application.Menu.hpp>

using namespace Futurewalker;

TEST_CASE("Menu")
{
    SECTION("Constructor")
    {
        SECTION("Default")
        {
            auto const menu = Menu();
            REQUIRE(menu.GetItems().empty());
        }
    }

    SECTION("Properties")
    {
        auto menu = Menu();

        SECTION("Items management")
        {
            REQUIRE(menu.GetItems().empty());

            // Create test items
            auto items = MenuItemArray();
            items.emplace_back(MenuItemType::Item);
            items.emplace_back(MenuItemType::Separator);
            items.emplace_back(MenuItemType::SubMenu);

            items[0].SetTitle(u8"File");
            items[0].SetId(MenuItemId(1u));
            items[2].SetTitle(u8"Edit");
            items[2].SetId(MenuItemId(2u));

            menu.SetItems(items);

            auto const& retrievedItems = menu.GetItems();
            REQUIRE(retrievedItems.size() == 3);
            REQUIRE(retrievedItems[0].GetType() == MenuItemType::Item);
            REQUIRE(retrievedItems[0].GetTitle() == u8"File");
            REQUIRE(retrievedItems[0].GetId() == MenuItemId(1u));
            REQUIRE(retrievedItems[1].GetType() == MenuItemType::Separator);
            REQUIRE(retrievedItems[2].GetType() == MenuItemType::SubMenu);
            REQUIRE(retrievedItems[2].GetTitle() == u8"Edit");
            REQUIRE(retrievedItems[2].GetId() == MenuItemId(2u));
        }

        SECTION("Replace items")
        {
            // Set initial items
            auto initialItems = MenuItemArray();
            initialItems.emplace_back(MenuItemType::Item);
            initialItems[0].SetTitle(u8"Initial Item");
            menu.SetItems(initialItems);

            REQUIRE(menu.GetItems().size() == 1);
            REQUIRE(menu.GetItems()[0].GetTitle() == u8"Initial Item");

            // Replace with new items
            auto newItems = MenuItemArray();
            newItems.emplace_back(MenuItemType::SubMenu);
            newItems.emplace_back(MenuItemType::Item);
            newItems[0].SetTitle(u8"New SubMenu");
            newItems[1].SetTitle(u8"New Item");

            menu.SetItems(newItems);

            auto const& retrievedItems = menu.GetItems();
            REQUIRE(retrievedItems.size() == 2);
            REQUIRE(retrievedItems[0].GetType() == MenuItemType::SubMenu);
            REQUIRE(retrievedItems[0].GetTitle() == u8"New SubMenu");
            REQUIRE(retrievedItems[1].GetType() == MenuItemType::Item);
            REQUIRE(retrievedItems[1].GetTitle() == u8"New Item");
        }

        SECTION("Add individual items")
        {
            REQUIRE(menu.GetItems().empty());

            auto item1 = MenuItem(MenuItemType::Item);
            item1.SetTitle(u8"First Item");
            item1.SetId(MenuItemId(10u));

            menu.AddItem(item1);
            REQUIRE(menu.GetItems().size() == 1);
            REQUIRE(menu.GetItems()[0].GetTitle() == u8"First Item");
            REQUIRE(menu.GetItems()[0].GetId() == MenuItemId(10u));

            auto separator = MenuItem(MenuItemType::Separator);
            menu.AddItem(separator);
            REQUIRE(menu.GetItems().size() == 2);
            REQUIRE(menu.GetItems()[1].GetType() == MenuItemType::Separator);

            auto item2 = MenuItem(MenuItemType::SubMenu);
            item2.SetTitle(u8"Second Item");
            item2.SetSubtitle(u8"SubMenu with subtitle");
            item2.SetId(MenuItemId(20u));

            menu.AddItem(item2);
            REQUIRE(menu.GetItems().size() == 3);
            REQUIRE(menu.GetItems()[2].GetTitle() == u8"Second Item");
            REQUIRE(menu.GetItems()[2].GetSubtitle() == u8"SubMenu with subtitle");
            REQUIRE(menu.GetItems()[2].GetId() == MenuItemId(20u));
        }
    }

    SECTION("Complex menu structures")
    {
        auto menu = Menu();

        SECTION("Menu with submenus")
        {
            // Create a submenu
            auto subMenuItems = MenuItemArray();
            subMenuItems.emplace_back(MenuItemType::Item);
            subMenuItems.emplace_back(MenuItemType::Separator);
            subMenuItems.emplace_back(MenuItemType::Item);

            subMenuItems[0].SetTitle(u8"New");
            subMenuItems[0].SetId(MenuItemId(101u));
            subMenuItems[2].SetTitle(u8"Open");
            subMenuItems[2].SetId(MenuItemId(102u));

            auto fileMenuItem = MenuItem(MenuItemType::SubMenu);
            fileMenuItem.SetTitle(u8"File");
            fileMenuItem.SetId(MenuItemId(100u));
            fileMenuItem.SetSubMenu(subMenuItems);

            auto editMenuItem = MenuItem(MenuItemType::Item);
            editMenuItem.SetTitle(u8"Edit");
            editMenuItem.SetId(MenuItemId(200u));

            menu.AddItem(fileMenuItem);
            menu.AddItem(editMenuItem);

            auto const& items = menu.GetItems();
            REQUIRE(items.size() == 2);

            // Verify File submenu
            REQUIRE(items[0].GetType() == MenuItemType::SubMenu);
            REQUIRE(items[0].GetTitle() == u8"File");
            REQUIRE(items[0].GetId() == MenuItemId(100u));

            auto const& subMenu = items[0].GetSubMenu();
            REQUIRE(subMenu.size() == 3);
            REQUIRE(subMenu[0].GetTitle() == u8"New");
            REQUIRE(subMenu[0].GetId() == MenuItemId(101u));
            REQUIRE(subMenu[1].GetType() == MenuItemType::Separator);
            REQUIRE(subMenu[2].GetTitle() == u8"Open");
            REQUIRE(subMenu[2].GetId() == MenuItemId(102u));

            // Verify Edit item
            REQUIRE(items[1].GetType() == MenuItemType::Item);
            REQUIRE(items[1].GetTitle() == u8"Edit");
            REQUIRE(items[1].GetId() == MenuItemId(200u));
        }

        SECTION("Empty items array")
        {
            auto emptyItems = MenuItemArray();
            menu.SetItems(emptyItems);
            REQUIRE(menu.GetItems().empty());
        }
    }

    SECTION("Copy")
    {
        auto original = Menu();
        
        // Setup original menu
        auto items = MenuItemArray();
        items.emplace_back(MenuItemType::Item);
        items.emplace_back(MenuItemType::SubMenu);
        items[0].SetTitle(u8"Original Item");
        items[0].SetId(MenuItemId(123u));
        items[1].SetTitle(u8"Original SubMenu");
        items[1].SetId(MenuItemId(456u));
        
        original.SetItems(items);

        SECTION("Copy constructor")
        {
            auto copy = original;

            // Verify copy has same values
            auto const& originalItems = original.GetItems();
            auto const& copyItems = copy.GetItems();
            
            REQUIRE(copyItems.size() == originalItems.size());
            REQUIRE(copyItems[0].GetTitle() == originalItems[0].GetTitle());
            REQUIRE(copyItems[0].GetId() == originalItems[0].GetId());
            REQUIRE(copyItems[1].GetTitle() == originalItems[1].GetTitle());
            REQUIRE(copyItems[1].GetId() == originalItems[1].GetId());

            // Verify copy-on-write: modifying copy doesn't affect original
            auto modifiedItem = MenuItem(MenuItemType::Item);
            modifiedItem.SetTitle(u8"Modified Item");
            copy.AddItem(modifiedItem);

            REQUIRE(copy.GetItems().size() == 3);
            REQUIRE(original.GetItems().size() == 2);
            REQUIRE(copy.GetItems()[2].GetTitle() == u8"Modified Item");
        }

        SECTION("Assignment operator")
        {
            auto assigned = Menu();
            assigned = original;

            // Verify assignment has same values
            auto const& originalItems = original.GetItems();
            auto const& assignedItems = assigned.GetItems();
            
            REQUIRE(assignedItems.size() == originalItems.size());
            REQUIRE(assignedItems[0].GetTitle() == originalItems[0].GetTitle());
            REQUIRE(assignedItems[0].GetId() == originalItems[0].GetId());
            REQUIRE(assignedItems[1].GetTitle() == originalItems[1].GetTitle());
            REQUIRE(assignedItems[1].GetId() == originalItems[1].GetId());

            // Verify copy-on-write: modifying assigned doesn't affect original
            auto newItems = MenuItemArray();
            newItems.emplace_back(MenuItemType::Separator);
            assigned.SetItems(newItems);

            REQUIRE(assigned.GetItems().size() == 1);
            REQUIRE(assigned.GetItems()[0].GetType() == MenuItemType::Separator);
            REQUIRE(original.GetItems().size() == 2);
        }
    }

    SECTION("Move")
    {
        auto original = Menu();
        
        // Setup original menu
        auto items = MenuItemArray();
        items.emplace_back(MenuItemType::Item);
        items[0].SetTitle(u8"Original Item");
        items[0].SetId(MenuItemId(789u));
        original.SetItems(items);

        SECTION("Move constructor")
        {
            auto moved = std::move(original);

            // Verify moved object has same values
            auto const& movedItems = moved.GetItems();
            REQUIRE(movedItems.size() == 1);
            REQUIRE(movedItems[0].GetTitle() == u8"Original Item");
            REQUIRE(movedItems[0].GetId() == MenuItemId(789u));

            REQUIRE(original.GetItems().empty()); // Default state after move
        }

        SECTION("Move assignment operator")
        {
            auto assigned = Menu();
            assigned = std::move(original);

            // Verify assigned object has same values
            auto const& assignedItems = assigned.GetItems();
            REQUIRE(assignedItems.size() == 1);
            REQUIRE(assignedItems[0].GetTitle() == u8"Original Item");
            REQUIRE(assignedItems[0].GetId() == MenuItemId(789u));

            REQUIRE(original.GetItems().empty()); // Default state after move
        }
    }
}
