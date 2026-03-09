// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerVisualPropertyUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Graphics.Path.hpp"
#include "Futurewalker.Graphics.DisplayListType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.Optional.hpp"

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
    auto PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Optional<Graphics::Path> const& clipPath, Float64 const& opacity) -> void;
    auto PopNode() -> void;
    auto FindNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisual>;
    auto PushVisual(PlatformViewLayerId const id) -> void;
    auto PopVisual() -> void;
    auto UpdateFragment(Shared<PlatformViewLayer> const& layer) -> void;
    auto UpdateCore(Shared<PlatformViewLayer> const& layer) -> void;

private:
    auto InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisual>;
    auto InternalSetCurrentVisual(Shared<PlatformViewLayerVisual> const& visual) -> void;
    auto InternalSetCurrentVisualProperties(Shared<PlatformViewLayerVisual> const& visual, SInt64 const target) -> void;
    auto InternalPushBaseVisual(Shared<PlatformViewLayerVisual> const& visual) -> void;
    auto InternalPopBaseVisual() -> void;
    auto InternalGetNodeState(SInt64 const target, Offset<Dp>& offset, Rect<Dp>& clipRect, std::vector<Graphics::Path>& clipPaths, Float64& opacity) const -> void;
    auto InternalGetBaseNodeIndex() const -> SInt64;
    auto InternalGetBaseVisual() const -> Shared<PlatformViewLayerVisual>;
    auto InternalGetCurrentNodeIndex() const -> SInt64;
    auto InternalReplaceNextPushNodeFragment(Offset<Dp> const& offset, Rect<Dp> const& clipRect, Optional<Graphics::Path> const& clipPath, Float64 const& opacity) -> void;
    auto InternalReplaceNextPopNodeFragment() -> void;
    auto InternalReplaceNextDisplayListFragment(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& displayListOffset) -> void;

private:
    struct NodeInfo
    {
        PlatformViewLayerId id;
        Offset<Dp> offset;
        Rect<Dp> clipRect;
        Optional<Graphics::Path> clipPath;
        Float64 opacity = 1.0;
    };
    std::vector<NodeInfo> _nodeStack;
    std::vector<SInt64> _baseVisualNodeIndexStack;
    std::vector<Shared<PlatformViewLayerVisual>> _baseVisualStack;
    Shared<PlatformViewLayerVisual> _currentVisual;
    SInt32 _currentFragmentIndex = 0;
};
}
}
