// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformApplicationMac.hpp"
#include "Futurewalker.Application.PlatformApplicationEvent.hpp"
#include "Futurewalker.Application.Menu.hpp"
#include "Futurewalker.Application.MenuItem.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Action.CommandDispatcher.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Core.Mac.PlatformStringFunctionMac.hpp"

using namespace FW_NS;

@interface PlatformMenuItemObject : NSObject
@property(assign, nonatomic) Optional<CommandId> commandId;
@end

@implementation PlatformMenuItemObject
@end

@interface PlatformApplicationDelegate : NSObject <NSApplicationDelegate>
@property(assign, nonatomic) void* data;
@property(assign, nonatomic) void (*callbackOnFinishLaunching)(void*);
@property(assign, nonatomic) void (*callbackOnBecomeActive)(void*);
@property(assign, nonatomic) void (*callbackOnResignActive)(void*);
- (void)applicationDidFinishLaunching:(NSNotification*)notification;
- (void)applicationDidBecomeActive:(NSNotification*)notification;
- (void)applicationDidResignActive:(NSNotification*)notification;
- (void)action:(id)sender;
- (BOOL)validateMenuItem:(NSMenuItem *)menuItem;
@end

@implementation PlatformApplicationDelegate
- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    _callbackOnFinishLaunching(_data);
}

- (void)applicationDidBecomeActive:(NSNotification*)notification
{
    _callbackOnBecomeActive(_data);
}

- (void)applicationDidResignActive:(NSNotification*)notification
{
    _callbackOnResignActive(_data);
}

- (void)action:(id)sender
{
    if ([sender isKindOfClass:[NSMenuItem class]])
    {
        NSMenuItem* menuItem = (NSMenuItem*)sender;
        if ([menuItem.representedObject isKindOfClass:[PlatformMenuItemObject class]])
        {
            auto menuItemObject = (PlatformMenuItemObject*)menuItem.representedObject;
            if (menuItemObject.commandId)
            {
                CommandDispatcher::Execute(*menuItemObject.commandId);
            }
        }
    }
}

- (BOOL)validateMenuItem:(NSMenuItem*)menuItem
{
    if ([menuItem.representedObject isKindOfClass:[PlatformMenuItemObject class]])
    {
        auto menuItemObject = (PlatformMenuItemObject*)menuItem.representedObject;
        if (menuItemObject.commandId)
        {
            if (CommandDispatcher::IsEnabled(*menuItemObject.commandId))
            {
                return YES;
            }
        }
    }
    return NO;
}
@end

namespace FW_DETAIL_NS
{
namespace
{
auto ConvertMenuToNSMenu(PlatformApplicationDelegate* delegate, NSMenu* parentMenu, Menu const& menu) -> void
{
    for (auto const& item : menu.GetItems())
    {
        auto const title = PlatformStringFunctionMac::ConvertStringToNSString(item.GetTitle());
        auto const subtitle = PlatformStringFunctionMac::ConvertStringToNSString(item.GetSubtitle());

        NSMenuItem* nsMenuItem = [NSMenuItem new];
        if (item.GetType() == MenuItemType::Item)
        {
            auto representedObject = [PlatformMenuItemObject new];
            if (item.GetCommandId() != 0U)
            {
                representedObject.commandId = item.GetCommandId();
            }
            [nsMenuItem setTitle:title];
            [nsMenuItem setSubtitle:subtitle];
            [nsMenuItem setRepresentedObject:representedObject];
            [nsMenuItem setTarget:delegate];
            [nsMenuItem setAction:@selector(action:)];
        }
        else if (item.GetType() == MenuItemType::Separator)
        {
            nsMenuItem = [NSMenuItem separatorItem];
        }
        else if (item.GetType() == MenuItemType::SubMenu)
        {
            auto subMenu = [NSMenu new];
            ConvertMenuToNSMenu(delegate, subMenu, item.GetSubMenu());
            [nsMenuItem setTitle:title];
            [nsMenuItem setSubtitle:subtitle];
            [nsMenuItem setSubmenu:subMenu];
        }
        [parentMenu addItem:nsMenuItem];
    }
}

auto RemoveMenuItemWithRole(Menu& menu, MenuItemRole const role) -> Optional<MenuItem>
{
    auto items = menu.GetItems();
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if (it->GetType() == MenuItemType::SubMenu)
        {
            auto subMenu = it->GetSubMenu();
            auto subItems = subMenu.GetItems();
            for (auto subIt = subItems.begin(); subIt != subItems.end(); ++subIt)
            {
                if (subIt->GetRole() == role)
                {
                    auto result = *subIt;
                    subItems.erase(subIt);
                    if (subItems.empty())
                    {
                        items.erase(it);
                        menu.SetItems(items);
                    }
                    else
                    {
                        subMenu.SetItems(subItems);
                        it->SetSubMenu(subMenu);
                        menu.SetItems(items);
                    }
                    return result;
                }
            }
        }
    }
    return {};
}

auto RemoveAllMenuItemsWithRole(Menu& menu, MenuItemRole const role) -> std::vector<MenuItem>
{
    std::vector<MenuItem> result;
    while (true)
    {
        auto item = RemoveMenuItemWithRole(menu, role);
        if (!item)
        {
            break;
        }
        result.push_back(*item);
    }
    return result;
}
}

auto PlatformApplicationMac::Make(Delegate const& delegate, Shared<PlatformApplicationContextMac> context) -> Shared<PlatformApplicationMac>
{
    return PlatformApplication::MakeDerived<PlatformApplicationMac>(delegate, context);
}

PlatformApplicationMac::PlatformApplicationMac(PassKey<PlatformApplication>, Delegate const& delegate, Shared<PlatformApplicationContextMac> context)
  : PlatformApplication(delegate)
  , _context {context}
{
}

auto PlatformApplicationMac::Run() -> Async<void>
{
    @autoreleasepool
    {
        if ([NSApp isRunning])
        {
            FW_DEBUG_LOG_ERROR("Nested Application::Run() is not allowed");
            FW_DEBUG_ASSERT(false);
            co_return;
        }
        FW_DEBUG_LOG_INFO("PlatformApplicationMac::Run");
        [NSApp setDelegate:_delegate];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular]; // TODO: Can this be removed?
        [NSApp run];
        [NSApp setDelegate:nil];
    }
}

auto PlatformApplicationMac::Exit() -> void
{
    @autoreleasepool
    {
        [NSApp stop:nil];
    }
}

auto PlatformApplicationMac::IsRunning() -> Bool
{
    @autoreleasepool
    {
        return [NSApp isRunning];
    }
}

auto PlatformApplicationMac::IsActive() -> Bool
{
    @autoreleasepool
    {
        return [NSApp isActive];
    }
}

auto PlatformApplicationMac::IsForeground() -> Bool
{
    return true;
}

auto PlatformApplicationMac::SetMainMenu(Menu const& menu) -> void
{
    @autoreleasepool
    {
        auto modifiedMenu = menu;

        auto appMenu = [NSMenu new];

        if (auto aboutItem = RemoveMenuItemWithRole(modifiedMenu, MenuItemRole::About))
        {
            auto appName = NSRunningApplication.currentApplication.localizedName;
            auto title = [NSString stringWithFormat:@"About %@", appName];

            auto representedObject = [PlatformMenuItemObject new];
            if (aboutItem->GetCommandId() != 0U)
            {
                representedObject.commandId = aboutItem->GetCommandId();
            }
            NSMenuItem* appMenuItem = [NSMenuItem new];
            [appMenuItem setTitle:title];
            [appMenuItem setRepresentedObject:representedObject];
            [appMenuItem setAction:@selector(action:)];
            [appMenuItem setTarget:_delegate];
            [appMenu addItem:appMenuItem];
        }

        auto appSpecificItems = RemoveAllMenuItemsWithRole(modifiedMenu, MenuItemRole::AppSpecific);
        for (auto const& item : appSpecificItems)
        {
            auto title = PlatformStringFunctionMac::ConvertStringToNSString(item.GetTitle());
            NSMenuItem* appMenuItem = [NSMenuItem new];
            [appMenuItem setTitle:title];
            auto representedObject = [PlatformMenuItemObject new];
            if (item.GetCommandId() != 0U)
            {
                representedObject.commandId = item.GetCommandId();
            }
            [appMenuItem setRepresentedObject:representedObject];
            [appMenuItem setAction:@selector(action:)];
            [appMenuItem setTarget:_delegate];
            [appMenu addItem:appMenuItem];
        }

        [appMenu addItem:[NSMenuItem separatorItem]];

        if (auto preferencesItem = RemoveMenuItemWithRole(modifiedMenu, MenuItemRole::Preferences))
        {
            auto title = PlatformStringFunctionMac::ConvertStringToNSString(preferencesItem->GetTitle());

            auto representedObject = [PlatformMenuItemObject new];
            if (preferencesItem->GetCommandId() != 0U)
            {
                representedObject.commandId = preferencesItem->GetCommandId();
            }
            NSMenuItem* appMenuItem = [NSMenuItem new];
            [appMenuItem setTitle:title];
            [appMenuItem setRepresentedObject:representedObject];
            [appMenuItem setAction:@selector(action:)];
            [appMenuItem setTarget:_delegate];
            [appMenu addItem:appMenuItem];
        }

        [appMenu addItem:[NSMenuItem separatorItem]];

        if (auto servicesItem = RemoveMenuItemWithRole(modifiedMenu, MenuItemRole::Services))
        {
            auto title = PlatformStringFunctionMac::ConvertStringToNSString(servicesItem->GetTitle());

            NSMenu* servicesMenu = [NSMenu new];
            [NSApp setServicesMenu:servicesMenu];

            NSMenuItem* appMenuItem = [NSMenuItem new];
            [appMenuItem setTitle:title];
            [appMenuItem setSubmenu:servicesMenu];
            [appMenu addItem:appMenuItem];
        }

        [appMenu addItem:[NSMenuItem separatorItem]];

        {
            NSMenuItem* hideItem = [NSMenuItem new];
            [hideItem setTitle:[NSString stringWithFormat:@"Hide %@", NSRunningApplication.currentApplication.localizedName]];
            [hideItem setAction:@selector(hide:)];
            [hideItem setTarget:NSApp];
            [appMenu addItem:hideItem];
        }
        {
            NSMenuItem* hideOthersItem = [NSMenuItem new];
            [hideOthersItem setTitle:@"Hide Others"];
            [hideOthersItem setAction:@selector(hideOtherApplications:)];
            [hideOthersItem setTarget:NSApp];
            [appMenu addItem:hideOthersItem];
        }
        {
            NSMenuItem* showAllItem = [NSMenuItem new];
            [showAllItem setTitle:@"Show All"];
            [showAllItem setAction:@selector(unhideAllApplications:)];
            [showAllItem setTarget:NSApp];
            [appMenu addItem:showAllItem];
        }

        [appMenu addItem:[NSMenuItem separatorItem]];

        if (auto quitItem = RemoveMenuItemWithRole(modifiedMenu, MenuItemRole::Quit))
        {
            auto representedObject = [PlatformMenuItemObject new];
            if (quitItem->GetCommandId() != 0U)
            {
                representedObject.commandId = quitItem->GetCommandId();
            }
            NSMenuItem* appMenuItem = [NSMenuItem new];
            [appMenuItem setTitle:[NSString stringWithFormat:@"Quit %@", NSRunningApplication.currentApplication.localizedName]];
            [appMenuItem setRepresentedObject:representedObject];
            [appMenuItem setAction:@selector(action:)];
            [appMenuItem setTarget:_delegate];
            [appMenu addItem:appMenuItem];
        }

        auto nsMenu = [NSMenu new];

        // Add App Menu
        auto appMenuItem = [NSMenuItem new];
        [appMenuItem setTitle:@"App"];
        [appMenuItem setSubmenu:appMenu];
        [nsMenu addItem:appMenuItem];

        // Add rest of the menu
        ConvertMenuToNSMenu(_delegate, nsMenu, modifiedMenu);

        [NSApp setMainMenu:nsMenu];
    }
}

auto PlatformApplicationMac::Initialize() -> void
{
    @autoreleasepool
    {
        _delegate = [PlatformApplicationDelegate new];

        [_delegate setData:this];
        [_delegate setCallbackOnFinishLaunching:[](void* data) -> void {
            @autoreleasepool
            {
                if (!NSRunningApplication.currentApplication.active)
                {
                    // Activate app and bring all windows to frontmost.
                    [NSApplication.sharedApplication activate];
                    [NSRunningApplication.currentApplication activateWithOptions:NSApplicationActivateAllWindows];
                }
            }
            auto const _this = static_cast<PlatformApplicationMac*>(data);
            auto const e = Event<PlatformApplicationEvent::Started>();
            _this->SendApplicationEventDetached(e);
        }];
        [_delegate setCallbackOnBecomeActive:[](void* data) -> void {
            auto const _this = static_cast<PlatformApplicationMac*>(data);
            auto const e = Event<PlatformApplicationEvent::ActiveChanged>();
            _this->SendApplicationEventDetached(e);
        }];
        [_delegate setCallbackOnResignActive:[](void* data) -> void {
            auto const _this = static_cast<PlatformApplicationMac*>(data);
            auto const e = Event<PlatformApplicationEvent::ActiveChanged>();
            _this->SendApplicationEventDetached(e);
        }];
    }

    class ThisThreadScheduler final : public ThisThread::Scheduler
    {
        Weak<PlatformApplicationMac> _app;

    public:
        ThisThreadScheduler(Weak<PlatformApplicationMac> app)
          : _app {app}
        {
        }

        auto Schedule() -> AsyncTask<void> override
        {
            if (auto const app = _app.Lock())
            {
                co_return co_await app->Schedule();
            }
            throw Exception(ErrorCode::InvalidOperation);
        }

        auto ScheduleAfter(std::chrono::nanoseconds const delay) -> AsyncTask<void> override
        {
            if (auto const app = _app.Lock())
            {
                co_return co_await app->ScheduleAfter(delay);
            }
            throw Exception(ErrorCode::InvalidOperation);
        }
    };
    _thisThreadScheduler = Shared<ThisThreadScheduler>::Make(GetSelf());
    ThisThread::SetScheduler(_thisThreadScheduler);
}

auto PlatformApplicationMac::Schedule() -> AsyncTask<void>
{
    co_await _context->Schedule();
}

auto PlatformApplicationMac::ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>
{
    co_await _context->ScheduleAfter(delay);
}
}
