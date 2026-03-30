// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformScreenContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformScreenMac.hpp"
#include "Futurewalker.Application.Mac.PlatformWindowMac.hpp"

namespace FW_DETAIL_NS
{
 namespace
{
auto GetScreenDisplayId(NSScreen* screen)
{
    @autoreleasepool
    {
        auto const desc = [screen deviceDescription];
        auto const number = (NSNumber*)desc[@"NSScreenNumber"];
        return static_cast<CGDirectDisplayID>([number unsignedIntValue]);
    }
}
}
auto PlatformScreenContextMac::GetScreens() -> PlatformScreenArray
{
    return PlatformScreenArray(_screens.begin(), _screens.end());
}

auto PlatformScreenContextMac::GetPrimaryScreen() -> Shared<PlatformScreen>
{
    if (!_screens.empty())
    {
        return _screens[0];
    }
    return {};
}

auto PlatformScreenContextMac::GetScreenFromWindow(Shared<PlatformWindow> window) -> Shared<PlatformScreen>
{
    @autoreleasepool
    {
        if (auto windowMac = window.TryAs<PlatformWindowMac>())
        {
            auto const nativeHandle = windowMac->GetNativeHandle();
            auto const screen = [nativeHandle screen];
            auto const displayId = GetScreenDisplayId(screen);
            return FindScreenById(displayId);
        }
        return {};
    }
}

auto PlatformScreenContextMac::GetScreenFromRect(Rect<Vp> const& rect) -> Shared<PlatformScreen>
{
    @autoreleasepool
    {
        auto const targetRect =
          NSRectFromCGRect(CGRectMake(static_cast<CGFloat>(rect.GetLeft()), static_cast<CGFloat>(rect.GetBottom()), static_cast<CGFloat>(rect.GetWidth()), static_cast<CGFloat>(rect.GetHeight())));

        auto closestScreen = Shared<PlatformScreenMac>();

        auto maxIntersectionArea = CGFloat(0);
        for (auto const& screen : _screens)
        {
            auto const nativeScreen = screen->GetNativeHandle();
            auto const screenRect = nativeScreen.frame;
            auto const intersection = NSIntersectionRect(screenRect, targetRect);
            auto const intersectionArea = intersection.size.width * intersection.size.height;
            if (intersectionArea > maxIntersectionArea)
            {
                maxIntersectionArea = intersectionArea;
                closestScreen = screen;
            }
        }

        if (closestScreen)
        {
            return closestScreen;
        }

        auto minDistance = CGFloat(MAXFLOAT);
        for (auto const& screeen : _screens)
        {
            auto const nativeScreen = screeen->GetNativeHandle();
            auto const screenRect = nativeScreen.frame;
            auto const centerX = targetRect.origin.x + (targetRect.size.width / 2);
            auto const centerY = targetRect.origin.y + (targetRect.size.height / 2);
            auto const screenCenterX = screenRect.origin.x + (screenRect.size.width / 2);
            auto const screenCenterY = screenRect.origin.y + (screenRect.size.height / 2);
            auto const deltaX = centerX - screenCenterX;
            auto const deltaY = centerY - screenCenterY;
            auto const distance = deltaX * deltaX + deltaY * deltaY;
            if (distance < minDistance)
            {
                minDistance = distance;
                closestScreen = screeen;
            }
        }

        if (closestScreen)
        {
            return closestScreen;
        }
        return nullptr;
    }
}

auto PlatformScreenContextMac::Refresh() -> void
{
    @autoreleasepool
    {
        auto newScreens = std::vector<Shared<PlatformScreenMac>>();
        auto const nsScreens = [NSScreen screens];

        auto mainScrenOrigin = CGFloat();
        if (nsScreens.count > 0)
        {
            auto const mainNsScreen = nsScreens[0];
            mainScrenOrigin = mainNsScreen.frame.origin.y + mainNsScreen.frame.size.height;
        }

        for (NSScreen* nsScreen in nsScreens)
        {
            auto const displayId = GetScreenDisplayId(nsScreen);
            if (auto const found = FindScreenById(displayId))
            {
                newScreens.push_back(found);
                found->Refresh(mainScrenOrigin);
            }
            else
            {
                newScreens.push_back(Shared<PlatformScreenMac>::Make(nsScreen));
                newScreens.back()->Refresh(mainScrenOrigin);
            }
        }

        for (auto const& existingScreen : _screens)
        {
            if (std::find(newScreens.begin(), newScreens.end(), existingScreen) == newScreens.end())
            {
                existingScreen->Invalidate();
            }
        }
        _screens = newScreens;
    }
}

auto PlatformScreenContextMac::FindScreenById(CGDirectDisplayID const displayId) -> Shared<PlatformScreenMac>
{
    for (auto const& screen : _screens)
    {
        auto const nsScreen = screen->GetNativeHandle();
        auto const nsScreenDisplayId = GetScreenDisplayId(nsScreen);
        if (nsScreenDisplayId == displayId)
        {
            return screen;
        }
    }
    return nullptr;
}

auto Locator::Resolver<PlatformScreenContextMac>::Resolve() -> Shared<PlatformScreenContextMac>
{
    return Shared<PlatformScreenContextMac>::Make();
}
}
