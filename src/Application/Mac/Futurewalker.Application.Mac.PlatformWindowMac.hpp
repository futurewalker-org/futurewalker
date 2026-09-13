// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformWindowMacType.hpp"
#include "Futurewalker.Application.PlatformWindow.hpp"
#include "Futurewalker.Application.Mac.PlatformWindowContextMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformRootViewLayerMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformInputMethodMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerVisualContextMacType.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"

#import <QuartzCore/QuartzCore.h>

@class PlatformWindowDelegate;
@class PlatformWindowContentView;

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform window class.
///
class PlatformWindowMac final : public PlatformWindow
{
public:
    static auto Make(Shared<PlatformWindowContextMac> const& context, Shared<PlatformViewLayerVisualContextMac> const& visualContext, PlatformWindowOptions const& options, Delegate const& delegate)
      -> Shared<PlatformWindowMac>;

    PlatformWindowMac(
      PassKey<PlatformWindow> key,
      Shared<PlatformWindowContextMac> const& context,
      Shared<PlatformViewLayerVisualContextMac> const& visualContext,
      PlatformWindowOptions const& options,
      Delegate const& delegate);

    ~PlatformWindowMac();

    auto IsVisible() -> Bool override;
    auto SetVisible(Bool const visible) -> void override;
    auto IsActive() -> Bool override;
    auto SetActive() -> void override;
    auto IsFocused() -> Bool override;
    auto SetFocus() -> void override;
    auto GetFrameRect() -> Rect<Vp> override;
    auto SetFrameRect(Rect<Vp> const& rect) -> void override;
    auto CapturePointer(PointerId const id) -> void override;
    auto ReleasePointer(PointerId const id) -> void override;
    auto GetRestoredFrameRect() -> Rect<Vp> override;
    auto SetRestoredFrameRect(Rect<Vp> const& rect) -> void override;
    auto GetAreaBounds(WindowArea const area) -> std::vector<Rect<Dp>> override;
    auto GetAreaInsets(WindowArea const are) -> EdgeInsets override;
    auto GetDisplayScale() -> DisplayScale override;
    auto GetBackingScale() -> BackingScale override;
    auto GetSizeConstraints() -> BoxConstraints override;
    auto SetSizeConstraints(BoxConstraints const& constraints) -> void override;
    auto GetTitle() -> String override;
    auto SetTitle(String const& title) -> void override;
    auto Minimize() -> void override;
    auto Maximize() -> void override;
    auto Restore() -> void override;
    auto IsClosed() -> Bool override;
    auto RequestClose() -> Async<Bool> override;
    auto Close() -> void override;
    auto RequestFrame() -> void override;
    auto GetFrameTime() -> MonotonicTime override;
    auto GetViewLayer() -> Shared<PlatformViewLayer> override;
    auto GetInputMethod() -> Shared<PlatformInputMethod> override;
    auto SetBackgroundColor(RGBColor const& color) -> void override;

    auto GetNativeHandle() -> NSWindow*;

    auto IsOwned() const -> Bool;
    auto IsOwnerOf(PlatformWindowMac const& window) const -> Bool;
    auto GetOwner() -> Shared<PlatformWindowMac>;
    auto GetOwner() const -> Shared<PlatformWindowMac const>;
    auto GetRootOwner() -> Shared<PlatformWindowMac>;
    auto GetOwnedWindows() -> std::vector<Weak<PlatformWindowMac>> const&;
    auto RemoveOwnedWindow(Shared<PlatformWindowMac> const& window) -> void;

    auto OrderFront() -> void;

private:
    auto Initialize() -> void override;
    auto CallbackOnPointer(Event<>& event) -> void;
    auto CallbackOnKey(Event<>& event) -> void;
    auto CallbackOnResize() -> void;
    auto CallbackOnMove() -> void;
    auto CallbackOnScreenChange() -> void;
    auto CallbackOnScreenProfileChange() -> void;
    auto CallbackOnBackingPropertyChange() -> void;
    auto CallbackOnBecomeKey() -> void;
    auto CallbackOnResignKey() -> void;
    auto CallbackOnShouldClose() -> BOOL;
    auto CallbackOnWillClose() -> BOOL;
    auto CallbackOnFrameUpdate(CADisplayLink* displayLink) -> void;
    auto Render() -> void;
    auto Frame(NSTimeInterval targetTimestamp) -> void;
    auto NotifyContextBecomeKey() -> void;
    auto NotifyContextResignKey() -> void;
    auto CanMakeVisible() -> Bool;
    auto DestroyPopupChainOnResignKey() -> void;
    auto CloseOwnedWindowsRecursive() -> void;
    static auto NativeToVpRect(NSRect const& rect) -> Rect<Vp>;
    static auto VpToNativeRect(Rect<Vp> const& rect) -> NSRect;

private:
    Shared<PlatformWindowContextMac> const _context;
    Shared<PlatformViewLayerVisualContextMac> const _visualContext;
    __strong PlatformWindowContentView* _windowContentView;
    __strong PlatformWindowDelegate* _windowDelegate;
    PlatformWindowOptions _options;
    Shared<PlatformRootViewLayerMac> _rootViewLayer;
    BoxConstraints _sizeConstraints;
    String _title;
    MonotonicTime _currentFrameTime;
    std::vector<Weak<PlatformWindowMac>> _ownedWindows;
    Bool _closed = false;
};
}
}
