// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformWindow.hpp"
#include "Futurewalker.Application.Mac.PlatformWindowContextMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformRootViewLayerMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformInputMethodMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerVisualContextMacType.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"

#import <AppKit/AppKit.h>
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
    auto Close() -> Async<Bool> override;
    auto Destroy() -> void override;
    auto Render() -> void override;
    auto RequestFrame() -> void override;
    auto GetFrameTime() -> MonotonicTime override;
    auto GetViewLayer() -> Shared<PlatformViewLayer> override;
    auto GetInputMethod() -> Shared<PlatformInputMethod> override;
    auto SetBackgroundColor(RGBColor const& color) -> void override;

    auto GetNativeHandle() -> NSWindow*;

private:
    auto Initialize() -> void override;
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
};
}
}
