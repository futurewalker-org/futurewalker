// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerVisualPropertyUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Recursively update properties like offset, clip rect, opacity of visuals and fragments.
///
/// @note This is O(n) walk of entire layer tree so it becomes slow on large ones.
///
class PlatformViewLayerVisualPropertyUpdater
{
public:
    auto Update(Shared<PlatformViewLayerVisual> const& visual, Shared<PlatformViewLayer> const& layer) -> void;

private:
    auto PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Float64 const& opacity) -> void;
    auto PopNode() -> void;
    auto FindNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisual>;
    auto PushVisual(PlatformViewLayerId const id) -> void;
    auto PopVisual() -> void;
    auto GetBaseNodeIndex() const -> SInt64;
    auto GetCurrentNodeIndex() const -> SInt64;
    auto UpdateFragment(Shared<PlatformViewLayer> const& layer) -> void;
    auto UpdateCore(Shared<PlatformViewLayer> const& layer) -> void;

private:
    auto InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisual>;
    auto InternalSetCurrentVisual(Shared<PlatformViewLayerVisual> const& visual) -> void;
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
    Shared<PlatformViewLayerVisual> _currentVisual;
};
}
}
