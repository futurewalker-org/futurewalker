// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerVisualUpdaterType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualContextType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.Path.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp" 

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.Optional.hpp"

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
    auto PushVisual(PlatformViewLayerId const id) -> void;
    auto PopVisual() -> void;
    auto PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Optional<Graphics::Path> const& clipPath, Float64 const& opacity) -> void;
    auto PopNode(PlatformViewLayerId const& id) -> void;
    auto AddFragment(PlatformViewLayerId const id, Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& displayListOffset) -> void;
    auto UpdateCore(Shared<PlatformViewLayer> const& layer) -> void;

private:
    auto InternalInsertVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisual>;
    auto InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisual>;
    auto InternalSetCurrentVisual(Shared<PlatformViewLayerVisual> const& visual) -> void;
    auto InternalSetCurrentVisualProperties(Shared<PlatformViewLayerVisual> const& visual, SInt64 const target) -> void;
    auto InternalPushBaseVisual(Shared<PlatformViewLayerVisual> const& visual) -> void;
    auto InternalPopBaseVisual() -> void;
    auto InternalGetNodeState(SInt64 const target, Offset<Dp>& offset, Rect<Dp>& clipRect, std::vector<Graphics::Path>& clipPaths, Float64& opacity) const -> void;
    auto InternalGetBaseNodeIndex() const -> SInt64;
    auto InternalGetBaseVisual() const -> Shared<PlatformViewLayerVisual>;
    auto InternalGetCurrentNodeIndex() const -> SInt64;
    auto InternalRemoveUntilNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisual>;

private:
    struct NodeInfo
    {
        PlatformViewLayerId id;
        Offset<Dp> offset;
        Rect<Dp> clipRect;
        Optional<Graphics::Path> clipPath; 
        Float64 opacity = 1.0;
    };
    Shared<PlatformViewLayerVisualContext> _context;
    std::vector<NodeInfo> _nodeStack;
    std::vector<SInt64> _baseVisualNodeIndexStack;
    std::vector<Shared<PlatformViewLayerVisual>> _baseVisualStack;
    Shared<PlatformViewLayerVisual> _currentVisual;
};
}
}
