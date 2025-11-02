// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerWinType.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualWinType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"
#include "Futurewalker.Graphics.DisplayListType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Rebuilds visual for specific base layer, skipping child visuals.
///
class PlatformViewLayerVisualUpdaterWin
{
public:
    explicit PlatformViewLayerVisualUpdaterWin(Shared<PlatformDCompositionDeviceWin> const& dcompDevice);
    auto Update(Shared<PlatformViewLayerVisualWin> const& visual, Shared<PlatformViewLayerWin> const& layer) -> void;

private:
    auto PushVisual(PlatformViewLayerId const id) -> Bool;
    auto PopVisual() -> void;
    auto PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Float64 const& opacity) -> void;
    auto PopNode() -> void;
    auto AddFragment(PlatformViewLayerId const id, Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& displayListOffset) -> void;
    auto UpdateCore(Shared<PlatformViewLayerWin> const& layer) -> void;

private:
    auto InternalInsertVisual(PlatformViewLayerId const id, SInt64 const base, SInt64 const target) -> Shared<PlatformViewLayerVisualWin>;
    auto InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisualWin>;
    auto InternalSetCurrentVisual(Shared<PlatformViewLayerVisualWin> const& visual) -> void;
    auto InternalPushVisualNodeIndex() -> void;
    auto InternalPopVisualNodeIndex() -> void;
    auto InternalGetOffset(SInt64 const base, SInt64 const target) const -> Offset<Dp>;
    auto InternalGetClipRect(SInt64 const base, SInt64 const target) const -> Rect<Dp>;
    auto InternalGetOpacity(SInt64 const base, SInt64 const target) const -> Float64;
    auto InternalGetBaseNodeIndex() const -> SInt64;
    auto InternalGetCurrentNodeIndex() const -> SInt64;
    auto InternalRemoveUntilNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisualWin>;

private:
    struct NodeInfo
    {
        PlatformViewLayerId id;
        Offset<Dp> offset;
        Rect<Dp> clipRect;
        Float64 opacity = 1.0;
    };
    Shared<PlatformDCompositionDeviceWin> _dcompDevice;
    std::vector<NodeInfo> _nodeStack;
    std::vector<SInt64> _visualNodeIndexStack;
    Shared<PlatformViewLayerVisualWin> _currentVisual;
};

}
