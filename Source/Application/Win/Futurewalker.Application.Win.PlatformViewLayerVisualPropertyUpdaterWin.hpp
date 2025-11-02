// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerWinType.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualWinType.hpp"

#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Recursively update properties like offset, clip rect, opacity of visuals and fragments.
///
/// @note This is O(n) walk of entire layer tree so it becomes slow on large ones.
///
class PlatformViewLayerVisualPropertyUpdaterWin
{
public:
    auto Update(Shared<PlatformViewLayerVisualWin> const& visual, Shared<PlatformViewLayerWin> const& layer) -> void;

private:
    auto PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Float64 const& opacity) -> void;
    auto PopNode() -> void;
    auto FindNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisualWin>;
    auto PushVisual(PlatformViewLayerId const id) -> void;
    auto PopVisual() -> void;
    auto GetBaseNodeIndex() const -> SInt64;
    auto GetCurrentNodeIndex() const -> SInt64;
    auto UpdateFragment(Shared<PlatformViewLayerWin> const& layer) -> void;
    auto UpdateCore(Shared<PlatformViewLayerWin> const& layer) -> void;

private:
    auto InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisualWin>;
    auto InternalSetCurrentVisual(Shared<PlatformViewLayerVisualWin> const& visual) -> void;
    auto InternalPushVisualNodeIndex() -> void;
    auto InternalPopVisualNodeIndex() -> void;
    auto InternalResetRelativePropertiesFromBase() -> void;
    auto InternalGetCurrentOffset() const -> Offset<Dp>;
    auto InternalGetCurrentClipRect() const -> Rect<Dp>;
    auto InternalGetCurrentOpacity() const -> Float64;
    auto InternalGetCurrentOffsetFromBase() const -> Offset<Dp>;
    auto InternalGetCurrentClipRectFromBase() const -> Rect<Dp>;
    auto InternalGetCurrentOpacityFromBase() const -> Float64;

private:
    struct NodeInfo
    {
        PlatformViewLayerId id;
        Offset<Dp> offset;
        Rect<Dp> clipRect;
        Float64 opacity = 1.0;
        Offset<Dp> currentOffset = Offset<Dp>();
        Rect<Dp> currentClipRect = Rect<Dp>::Infinite();
        Float64 currentOpacity = 1.0;
        Offset<Dp> currentOffsetFromBase = Offset<Dp>();
        Rect<Dp> currentClipRectFromBase = Rect<Dp>::Infinite();
        Float64 currentOpacityFromBase = 1.0;
    };
    std::vector<NodeInfo> _nodeStack;
    std::vector<SInt64> _visualNodeIndexStack;
    Shared<PlatformViewLayerVisualWin> _currentVisual;
};
}
