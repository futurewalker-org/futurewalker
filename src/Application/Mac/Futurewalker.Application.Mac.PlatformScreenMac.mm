// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformScreenMac.hpp"

#include "Futurewalker.Core.Mac.PlatformStringFunctionMac.hpp"

namespace FW_DETAIL_NS
{
PlatformScreenMac::PlatformScreenMac(NSScreen* screen)
  : _screen {screen}
{
}

auto PlatformScreenMac::GetInfo() const -> Optional<ScreenInfo>
{
    return _info;
}

auto PlatformScreenMac::Refresh(CGFloat const mainScreenOrigin) -> void
{
    @autoreleasepool
    {
        auto screenInfo = ScreenInfo();
        {
            auto frame = [_screen frame];
            frame.origin.y = mainScreenOrigin - (frame.origin.y + frame.size.height);
            screenInfo.bounds = Rect<Vp>(Point<Vp>(frame.origin.x, frame.origin.y), Size<Vp>(frame.size.width, frame.size.height));
        }
        {
            auto frame = [_screen visibleFrame];
            frame.origin.y = mainScreenOrigin - (frame.origin.y + frame.size.height);
            frame.origin.x -= static_cast<CGFloat>(screenInfo.bounds.GetLeft());
            frame.origin.y -= static_cast<CGFloat>(screenInfo.bounds.GetTop());
            screenInfo.safeArea = Rect<Vp>(Point<Vp>(frame.origin.x, frame.origin.y), Size<Vp>(frame.size.width, frame.size.height));
        }
        {
            auto const insets = [_screen safeAreaInsets];
            auto rect = screenInfo.bounds;
            rect.SetLeft(rect.GetLeft() + insets.left);
            rect.SetTop(rect.GetTop() + insets.top);
            rect.SetRight(rect.GetRight() - insets.right);
            rect.SetBottom(rect.GetBottom() - insets.bottom);
            screenInfo.safeArea = rect;
        }
        screenInfo.backingScale = [_screen backingScaleFactor];
        screenInfo.displayScale = 1.0;
        screenInfo.displayName = PlatformStringFunctionMac::ConvertNSStringToString([_screen localizedName]);
        _info = screenInfo;
    }
}

auto PlatformScreenMac::Invalidate() -> void
{
    _info = {};
}

auto PlatformScreenMac::GetNativeHandle() const -> NSScreen*
{
    return _screen;
}
}
