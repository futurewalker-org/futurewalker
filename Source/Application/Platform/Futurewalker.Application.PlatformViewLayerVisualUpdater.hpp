// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerVisualUpdaterType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualContextType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Graphics.DisplayListType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp" 

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerVisualUpdater : NonCopyable
{
public:
    PlatformViewLayerVisualUpdater(Shared<PlatformViewLayerVisualContext> context);

    auto Update(Shared<PlatformViewLayerVisual> const& visual, Shared<PlatformViewLayer> const& layer) -> void;

private:
    auto PushVisual(PlatformViewLayerId const id) -> Bool;
    auto PopVisual() -> void;
    auto PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Float64 const& opacity) -> void;
    auto PopNode() -> void;
    auto AddFragment(PlatformViewLayerId const id, Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& displayListOffset) -> void;
    auto UpdateCore(Shared<PlatformViewLayer> const& layer) -> void;

private:
    auto InternalInsertVisual(PlatformViewLayerId const id, SInt64 const base, SInt64 const target) -> Shared<PlatformViewLayerVisual>;
    auto InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisual>;
    auto InternalSetCurrentVisual(Shared<PlatformViewLayerVisual> const& visual) -> void;
    auto InternalPushVisualNodeIndex() -> void;
    auto InternalPopVisualNodeIndex() -> void;
    auto InternalGetOffset(SInt64 const base, SInt64 const target) const -> Offset<Dp>;
    auto InternalGetClipRect(SInt64 const base, SInt64 const target) const -> Rect<Dp>;
    auto InternalGetOpacity(SInt64 const base, SInt64 const target) const -> Float64;
    auto InternalGetBaseNodeIndex() const -> SInt64;
    auto InternalGetCurrentNodeIndex() const -> SInt64;
    auto InternalRemoveUntilNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisual>;

private:
    struct NodeInfo
    {
        PlatformViewLayerId id;
        Offset<Dp> offset;
        Rect<Dp> clipRect;
        Float64 opacity = 1.0;
    };
    Shared<PlatformViewLayerVisualContext> _context;
    std::vector<NodeInfo> _nodeStack;
    std::vector<SInt64> _visualNodeIndexStack;
    Shared<PlatformViewLayerVisual> _currentVisual;
};
}
}
