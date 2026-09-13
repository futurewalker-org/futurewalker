// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformWindowContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformWindowMac.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerVisualContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformInputMethodContextMac.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include <algorithm>

namespace FW_DETAIL_NS
{
namespace
{
auto OwnsKeyWindow(Shared<PlatformWindowMac> const& window) -> Bool
{
    if (window)
    {
        if (auto nativeWindow = window->GetNativeHandle())
        {
            // isKeyWindow can return false during window activation.
            if (nativeWindow.isKeyWindow || NSApp.keyWindow == nativeWindow)
            {
                return true;
            }
        }

        for (auto const& weakOwned : window->GetOwnedWindows())
        {
            if (auto const owned = weakOwned.Lock())
            {
                if (OwnsKeyWindow(owned))
                {
                    return true;
                }
            }
        }
    }
    return False;
}
}

auto PlatformWindowContextMac::Make(Shared<PlatformInputMethodContext> inputMethodContext) -> Shared<PlatformWindowContextMac>
{
    auto context = Shared<PlatformWindowContextMac>::Make(PassKey<PlatformWindowContextMac>(), inputMethodContext);
    context->_self = context;
    return context;
}

PlatformWindowContextMac::PlatformWindowContextMac(PassKey<PlatformWindowContextMac>, Shared<PlatformInputMethodContext> inputMethodContext)
  : _inputMethodContext(inputMethodContext)
{
    _visualContext = PlatformViewLayerVisualContextMac::Make();
}

auto PlatformWindowContextMac::GetSelf() -> Shared<PlatformWindowContextMac>
{
    return _self.Lock();
}

auto PlatformWindowContextMac::MakePlatformWindow(PlatformWindowOptions const& options, PlatformWindow::Delegate const& delegate) -> Shared<PlatformWindow>
{
    auto window = PlatformWindowMac::Make(GetSelf(), _visualContext, options, delegate);
    if (!window->IsOwned())
    {
        _toplevelWindows.push_back(window);
    }
    return window;
}

auto PlatformWindowContextMac::SetApplicationActive(Bool const active) -> void
{
    if (_applicationActive != active)
    {
        FW_DEBUG_LOG_INFO("PlatformWindowContextMac::SetApplicationActive: {}", active ? "true" : "false");
        _applicationActive = active;
        UpdateWindowLevel();
    }
}

auto PlatformWindowContextMac::SetWindowActive(Shared<PlatformWindowMac> const& window, Bool const active) -> void
{
    if (window && active)
    {
        auto owner = window;
        while (owner)
        {
            owner->OrderFront();
            owner = owner->GetOwner();
        }
    }
    UpdateWindowLevel();
}

auto PlatformWindowContextMac::UpdateWindowLevel() -> void
{
    {
        auto const it = std::remove_if(_toplevelWindows.begin(), _toplevelWindows.end(), [](auto const& window) { return window.IsExpired(); });
        _toplevelWindows.erase(it, _toplevelWindows.end());
    }

    {
        auto const it = std::find_if(_toplevelWindows.begin(), _toplevelWindows.end(), [&](auto const& toplevel) {
            if (auto w = toplevel.Lock())
            {
                return OwnsKeyWindow(w);
            }
            return False;
        });

        if (it != _toplevelWindows.end())
        {
            std::rotate(it, it + 1, _toplevelWindows.end());
        }
    }

    auto baseWindowLevel = NSNormalWindowLevel;
    for (auto const& weakWindow : _toplevelWindows)
    {
        if (auto window = weakWindow.Lock())
        {
            if (_applicationActive && OwnsKeyWindow(window))
            {
                AssignWindowLevelRecursive(window, baseWindowLevel);
            }
            else
            {
                CollapseWindowLevelRecursive(window, baseWindowLevel++, 0);
            }
        }
    }
}

auto PlatformWindowContextMac::AssignWindowLevelRecursive(Shared<PlatformWindowMac> const& window, NSWindowLevel const baseWindowLevel) -> NSWindowLevel
{
    if (auto const nativeWindow = window->GetNativeHandle())
    {
        FW_DEBUG_LOG_INFO("Assigning window({}) level {} -> {}", nativeWindow.windowNumber, nativeWindow.level, baseWindowLevel);
        if (nativeWindow.level != baseWindowLevel)
        {
            nativeWindow.level = baseWindowLevel;
        }

        if (nativeWindow.isVisible)
        {
            [nativeWindow orderFront:nil];
        }
    }

    auto nextWindowLevel = baseWindowLevel + 1;
    for (auto const& weakOwned : window->GetOwnedWindows())
    {
        if (auto const owned = weakOwned.Lock())
        {
            nextWindowLevel = AssignWindowLevelRecursive(owned, nextWindowLevel);
        }
    }
    return nextWindowLevel;
}

auto PlatformWindowContextMac::CollapseWindowLevelRecursive(Shared<PlatformWindowMac> const& window, NSWindowLevel const targetWindowLevel, NSInteger const ownerWindowNumber) -> void
{
    auto const nativeWindow = window->GetNativeHandle();

    if (nativeWindow)
    {
        if (nativeWindow.level != targetWindowLevel)
        {
            FW_DEBUG_LOG_INFO("Collapsing window({}) level {} -> {}", nativeWindow.windowNumber, nativeWindow.level, targetWindowLevel);
            nativeWindow.level = targetWindowLevel;
        }

        if (ownerWindowNumber != 0)
        {
            [nativeWindow orderWindow:NSWindowAbove relativeTo:ownerWindowNumber];
        }
    }

    for (auto const& weakOwned : window->GetOwnedWindows())
    {
        if (auto const owned = weakOwned.Lock())
        {
            CollapseWindowLevelRecursive(owned, targetWindowLevel, nativeWindow ? nativeWindow.windowNumber : 0);
        }
    }
}

auto Locator::Resolver<PlatformWindowContextMac>::Resolve() -> Shared<PlatformWindowContextMac>
{
    auto inputMethodContext = Locator::Resolve<PlatformInputMethodContext>().As<PlatformInputMethodContextMac>();
    return PlatformWindowContextMac::Make(inputMethodContext);
}
}
