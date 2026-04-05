// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformRootViewLayerMacType.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerVisualContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerMac.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualRendererType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS Implementation of PlatformViewLayer.
///
class PlatformRootViewLayerMac final : public PlatformViewLayerMac
{
public:
    static auto Make(Shared<PlatformViewLayerVisualContextMac> const& visualContext, NSWindow* window) -> Shared<PlatformRootViewLayerMac>;

    PlatformRootViewLayerMac(PassKey<PlatformViewLayer>, Shared<PlatformViewLayerVisualContextMac> const& visualContext, NSWindow* window);

    auto GetControl() -> Shared<PlatformViewLayerControl> override;

    auto Render() -> void;

protected:
    auto Initialize() -> void override;

private:
    auto RootGetDisplayScale() const -> DisplayScale override;
    auto RootGetBackingScale() const -> BackingScale override;
    auto RootOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootSizeChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootClipPathChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootClipModeChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootOpacityChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootRenderFlagsChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootDisplayListChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootDisplayListOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootChildAdded(Shared<PlatformViewLayer> const& child) -> void override;
    auto RootChildRemoved(Shared<PlatformViewLayer> const& parent) -> void override;

private:
    __strong NSWindow* _window = nil;
    __strong NSView* _view = nil;
    __strong NSMutableArray<NSView*>* _expectedViews = nil;
    Shared<PlatformViewLayerVisualContextMac> _visualContext;
    Shared<PlatformViewLayerVisualRenderer> _renderer;
};
}
}
