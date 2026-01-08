// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayerVisualUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualContext.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
PlatformViewLayerVisualUpdater::PlatformViewLayerVisualUpdater(Shared<PlatformViewLayerVisualContext> context)
  : _context {context}
{
    if (!_context)
    {
        FW_DEBUG_LOG_ERROR("PlatformViewLayerVisualUpdater: context is null");
        throw Exception(ErrorCode::InvalidArgument);
    }
}

auto PlatformViewLayerVisualUpdater::Update(Shared<PlatformViewLayerVisual> const& visual, Shared<PlatformViewLayer> const& layer) -> void
{
    _nodeStack.clear();
    _visualNodeIndexStack.clear();
    _currentVisual = visual;
    UpdateCore(layer);
}

auto PlatformViewLayerVisualUpdater::PushVisual(PlatformViewLayerId const id) -> Bool
{
    if (auto const nextVisual = InternalRemoveUntilNextVisual(id))
    {
        if (_visualNodeIndexStack.empty())
        {
            nextVisual->ClearFragments();
            InternalSetCurrentVisual(nextVisual);
            InternalPushVisualNodeIndex();
            InternalResetRelativePropertiesFromBase();
            return true;
        }
        InternalSetCurrentVisual(nullptr);
        return false;
    }
    FW_DEBUG_ASSERT(!_nodeStack.empty());
    InternalSetCurrentVisual(InternalInsertVisual(id, InternalGetCurrentNodeIndex()));
    InternalPushVisualNodeIndex();
    InternalResetRelativePropertiesFromBase();
    return true;
}

auto PlatformViewLayerVisualUpdater::PopVisual() -> void
{
    InternalPopVisualNodeIndex();
    InternalSetCurrentVisual(nullptr);
}

auto PlatformViewLayerVisualUpdater::PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Float64 const& opacity) -> void
{
    Offset<Dp> currentOffset = Offset<Dp>();
    Rect<Dp> currentClipRect = Rect<Dp>::Infinite();
    Float64 currentOpacity = 1.0;
    Offset<Dp> currentOffsetFromBase = Offset<Dp>();
    Rect<Dp> currentClipRectFromBase = Rect<Dp>::Infinite();
    Float64 currentOpacityFromBase = 1.0;

    if (_nodeStack.empty())
    {
        currentOffset = offset;
        currentClipRect = clipRect;
        currentOpacity = opacity;
    }
    else
    {
        auto& back = _nodeStack.back();
        currentOffset = back.currentOffset + offset;
        currentClipRect = Rect<Dp>::Intersect(Rect<Dp>::Offset(back.currentClipRect, -offset), clipRect);
        currentOpacity = back.currentOpacity * opacity;
        currentOffsetFromBase = back.currentOffsetFromBase + offset;
        currentClipRectFromBase = Rect<Dp>::Intersect(Rect<Dp>::Offset(back.currentClipRectFromBase, -offset), clipRect);
        currentOpacityFromBase = back.currentOpacityFromBase * opacity;
    }

    _nodeStack.push_back({
        .id = id,
        .offset = offset,
        .clipRect = clipRect,
        .opacity = opacity,
        .currentOffset = currentOffset,
        .currentClipRect = currentClipRect,
        .currentOpacity = currentOpacity,
        .currentOffsetFromBase = currentOffsetFromBase,
        .currentClipRectFromBase = currentClipRectFromBase,
        .currentOpacityFromBase = currentOpacityFromBase,
    });
}

auto PlatformViewLayerVisualUpdater::PopNode() -> void
{
    FW_DEBUG_ASSERT(!_nodeStack.empty());
    _nodeStack.pop_back();
}

auto PlatformViewLayerVisualUpdater::AddFragment(PlatformViewLayerId const id, Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& displayListOffset) -> void
{
    auto visual = InternalGetCurrentVisual();
    if (!visual)
    {
        // When one of child nodes has its own visual, rest of siblings needs another visual for them.
        visual = InternalInsertVisual(id, InternalGetBaseNodeIndex());
        InternalSetCurrentVisual(visual);
        InternalPushVisualNodeIndex();
    }

    auto const baseIndex = InternalGetBaseNodeIndex();
    visual->AddFragment(
      id,
      {
          .offset = InternalGetCurrentOffsetFromBase(),
          .clipRect = InternalGetCurrentClipRectFromBase(),
          .opacity = InternalGetCurrentOpacityFromBase(),
          .displayList = displayList,
          .displayListOffset = displayListOffset,
      });
}

auto PlatformViewLayerVisualUpdater::UpdateCore(Shared<PlatformViewLayer> const& layer) -> void
{
    auto const id = layer->GetId();
    auto const offset = layer->GetOffset();
    auto const opacity = layer->GetOpacity();
    auto const clipMode = layer->GetClipMode() == ViewClipMode::Bounds ? Rect<Dp>({}, layer->GetSize()) : Rect<Dp>::Infinite();
    PushNode(id, offset, clipMode, opacity);

    auto const needsSurface = layer->ShouldRasterize();
    if (needsSurface)
    {
        if (!PushVisual(id))
        {
            PopNode();
            return;
        }
    }

    auto const displayList = layer->GetDisplayList();
    auto const displayListOffset = layer->GetDisplayListOffset();
    AddFragment(id, displayList, displayListOffset);

    for (auto const& child : layer->GetChildren())
    {
        UpdateCore(child);
    }

    PopNode();

    if (needsSurface)
    {
        PopVisual();
    }
}

auto PlatformViewLayerVisualUpdater::InternalInsertVisual(PlatformViewLayerId const id, SInt64 const target) -> Shared<PlatformViewLayerVisual>
{
    auto const visual = _context->CreateVisual();
    visual->SetBaseLayerId(id);
    visual->SetClipRect(InternalGetClipRect(target));
    visual->SetOffset(InternalGetOffset(target));
    visual->SetOpacity(InternalGetOpacity(target));
    if (auto const currentVisual = InternalGetCurrentVisual())
    {
        currentVisual->AddChild(visual);
        return visual;
    }
    FW_DEBUG_ASSERT(false);
    return {};
}

auto PlatformViewLayerVisualUpdater::InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisual>
{
    return _currentVisual;
}

auto PlatformViewLayerVisualUpdater::InternalSetCurrentVisual(Shared<PlatformViewLayerVisual> const& visual) -> void
{
    _currentVisual = visual;
}

auto PlatformViewLayerVisualUpdater::InternalPushVisualNodeIndex() -> void
{
    _visualNodeIndexStack.push_back(std::ssize(_nodeStack) - 1);
}

auto PlatformViewLayerVisualUpdater::InternalPopVisualNodeIndex() -> void
{
    _visualNodeIndexStack.pop_back();
}

auto PlatformViewLayerVisualUpdater::InternalResetRelativePropertiesFromBase() -> void
{
    if (!_nodeStack.empty())
    {
        auto& back = _nodeStack.back();
        back.currentOffsetFromBase = Offset<Dp>();
        back.currentClipRectFromBase = Rect<Dp>::Infinite();
        back.currentOpacityFromBase = 1.0;
    }
}

auto PlatformViewLayerVisualUpdater::InternalGetOffset(SInt64 const target) const -> Offset<Dp>
{
    if (0 <= target && target < std::ssize(_nodeStack))
    {
        _nodeStack[static_cast<size_t>(target)].currentOffset;
    }
    return Offset<Dp>();
}

auto PlatformViewLayerVisualUpdater::InternalGetClipRect(SInt64 const target) const -> Rect<Dp>
{
    if (0 <= target && target < std::ssize(_nodeStack))
    {
        _nodeStack[static_cast<size_t>(target)].currentClipRect;
    }
    return Rect<Dp>::Infinite();
}

auto PlatformViewLayerVisualUpdater::InternalGetOpacity(SInt64 const target) const -> Float64
{
    if (0 <= target && target < std::ssize(_nodeStack))
    {
        _nodeStack[static_cast<size_t>(target)].currentOpacity;
    }
    return 1.0;
}

auto PlatformViewLayerVisualUpdater::InternalGetCurrentOffset() const -> Offset<Dp>
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentOffset;
    }
    return Offset<Dp>();
}

auto PlatformViewLayerVisualUpdater::InternalGetCurrentClipRect() const -> Rect<Dp>
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentClipRect;
    }
    return Rect<Dp>::Infinite();
}

auto PlatformViewLayerVisualUpdater::InternalGetCurrentOpacity() const -> Float64
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentOpacity;
    }
    return 1.0;
}

auto PlatformViewLayerVisualUpdater::InternalGetCurrentOffsetFromBase() const -> Offset<Dp>
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentOffsetFromBase;
    }
    return Offset<Dp>();
}

auto PlatformViewLayerVisualUpdater::InternalGetCurrentClipRectFromBase() const -> Rect<Dp>
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentClipRectFromBase;
    }
    return Rect<Dp>::Infinite();
}

auto PlatformViewLayerVisualUpdater::InternalGetCurrentOpacityFromBase() const -> Float64
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentOpacityFromBase;
    }
    return 1.0;
}

auto PlatformViewLayerVisualUpdater::InternalGetBaseNodeIndex() const -> SInt64
{
    if (!_visualNodeIndexStack.empty())
    {
        return _visualNodeIndexStack.back();
    }
    return 0;
}

auto PlatformViewLayerVisualUpdater::InternalGetCurrentNodeIndex() const -> SInt64
{
    return static_cast<SInt64>(std::ssize(_nodeStack)) - 1;
}

auto PlatformViewLayerVisualUpdater::InternalRemoveUntilNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisual>
{
    if (auto const visual = InternalGetCurrentVisual())
    {
        if (visual->GetBaseLayerId() == id)
        {
            return visual;
        }

        auto const children = visual->GetChildren();
        for (auto const& child : children)
        {
            if (child->GetBaseLayerId() == id)
            {
                return child;
            }
            else
            {
                visual->RemoveChild(child);
            }
        }
    }
    return {};
}
}
