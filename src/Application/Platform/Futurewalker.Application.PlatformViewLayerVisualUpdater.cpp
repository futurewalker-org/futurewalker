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
    _baseVisualNodeIndexStack.clear();
    _baseVisualStack.clear();
    _currentVisual = visual;
    UpdateCore(layer);
}

auto PlatformViewLayerVisualUpdater::PushVisual(PlatformViewLayerId const id, DisplayScale const displayScale, BackingScale const backingScale) -> void
{
    FW_DEBUG_ASSERT(!_nodeStack.empty());

    if (auto const nextVisual = InternalRemoveUntilNextVisual(id))
    {
        nextVisual->ClearFragments();
        InternalSetCurrentVisualProperties(nextVisual, InternalGetCurrentNodeIndex(), displayScale, backingScale);
        InternalSetCurrentVisual(nextVisual);
        InternalPushBaseVisual(nextVisual);
        return;
    }

    if (auto const currentVisual = InternalGetCurrentVisual())
    {
        for (auto i = InternalGetCurrentNodeIndex(); i > InternalGetBaseNodeIndex(); --i)
        {
            currentVisual->AddPopNodeFragment(_nodeStack[static_cast<size_t>(i)].id);
        }
    }
    auto const visual = InternalInsertVisual(id);
    InternalSetCurrentVisualProperties(visual, InternalGetCurrentNodeIndex(), displayScale, backingScale);
    InternalSetCurrentVisual(visual);
    InternalPushBaseVisual(visual);
}

auto PlatformViewLayerVisualUpdater::PopVisual() -> void
{
    InternalPopBaseVisual();
    InternalSetCurrentVisual(nullptr);
}

auto PlatformViewLayerVisualUpdater::PushNode(PlatformViewLayerId const& id, Vector<Dp> const& offset, Rect<Dp> const& clipRect, Optional<Graphics::Path> const& clipPath, Float64 const& opacity) -> void
{
    _nodeStack.push_back({
        .id = id,
        .offset = offset,
        .clipRect = clipRect,
        .clipPath = clipPath,
        .opacity = opacity,
    });

    if (auto const visual = InternalGetCurrentVisual())
    {
        visual->AddPushNodeFragment(
          id,
          {
              .offset = offset,
              .clipRect = clipRect,
              .clipPath = clipPath,
              .opacity = opacity,
          });
    }
}

auto PlatformViewLayerVisualUpdater::PopNode(PlatformViewLayerId const& id) -> void
{
    FW_DEBUG_ASSERT(!_nodeStack.empty());
    _nodeStack.pop_back();

    if (auto const visual = InternalGetCurrentVisual())
    {
        visual->AddPopNodeFragment(id);
    }
}

auto PlatformViewLayerVisualUpdater::AddFragment(PlatformViewLayerId const id, DisplayScale const displayScale, BackingScale const backingScale, Shared<Graphics::DisplayList> const& displayList, Vector<Dp> const& displayListOffset) -> void
{
    auto visual = InternalGetCurrentVisual();
    if (!visual)
    {
        // When one of child nodes has its own visual, rest of siblings needs another visual for them.
        visual = InternalInsertVisual(id);

        for (auto i = InternalGetBaseNodeIndex() + 1; i <= InternalGetCurrentNodeIndex(); ++i)
        {
            auto const& node = _nodeStack[static_cast<size_t>(i)];
            visual->AddPushNodeFragment(
              node.id,
              {
                  .offset = node.offset,
                  .clipRect = node.clipRect,
                  .clipPath = node.clipPath,
                  .opacity = node.opacity,
              });
        }
        InternalSetCurrentVisualProperties(visual, InternalGetCurrentNodeIndex(), displayScale, backingScale);
        InternalSetCurrentVisual(visual);
        InternalPushBaseVisual(visual);
    }

    visual->AddDisplayListFragment(
      id,
      {
          .displayList = displayList,
          .displayListOffset = displayListOffset,
      });
}

auto PlatformViewLayerVisualUpdater::UpdateCore(Shared<PlatformViewLayer> const& layer) -> void
{
    auto const id = layer->GetId();
    auto const offset = layer->GetOffset();
    auto const opacity = layer->GetOpacity();
    auto const clipRect = layer->GetClipMode() == ViewClipMode::Bounds ? Rect<Dp>({}, layer->GetSize()) : Rect<Dp>::Infinite();
    auto const clipPath = layer->GetClipPath();
    PushNode(id, offset, clipRect, clipPath, opacity);

    auto const displayScale = layer->GetDisplayScale();
    auto const backingScale = layer->GetBackingScale();
    auto const needsSurface = layer->ShouldRasterize();
    if (needsSurface)
    {
        PushVisual(id, displayScale, backingScale);
    }

    auto const displayList = layer->GetDisplayList();
    auto const displayListOffset = layer->GetDisplayListOffset();
    AddFragment(id, displayScale, backingScale, displayList, displayListOffset);

    for (auto const& child : layer->GetChildren())
    {
        UpdateCore(child);
    }

    if (needsSurface)
    {
        PopVisual();
    }
    PopNode(id);
}

auto PlatformViewLayerVisualUpdater::InternalInsertVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisual>
{
    auto const visual = _context->CreateVisual();
    visual->SetBaseLayerId(id);

    if (auto const currentVisual = InternalGetBaseVisual())
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

auto PlatformViewLayerVisualUpdater::InternalSetCurrentVisualProperties(Shared<PlatformViewLayerVisual> const& visual, SInt64 const target, DisplayScale const displayScale, BackingScale const backingScale) -> void
{
    if (visual)
    {
        auto offset = Vector<Dp>();
        auto clipRect = Rect<Dp>();
        auto clipPaths = std::vector<Graphics::Path>();
        auto opacity = Float64(1.0);
        InternalGetNodeState(target, offset, clipRect, clipPaths, opacity);
        visual->SetOffset(offset);
        visual->SetClipRect(clipRect);
        visual->SetClipPaths(clipPaths);
        visual->SetOpacity(opacity);
        visual->SetDisplayScale(displayScale);
        visual->SetBackingScale(backingScale);
    }
}

auto PlatformViewLayerVisualUpdater::InternalPushBaseVisual(Shared<PlatformViewLayerVisual> const& visual) -> void
{
    _baseVisualNodeIndexStack.push_back(std::ssize(_nodeStack) - 1);
    _baseVisualStack.push_back(visual);
}

auto PlatformViewLayerVisualUpdater::InternalPopBaseVisual() -> void
{
    _baseVisualNodeIndexStack.pop_back();
    _baseVisualStack.pop_back();
}

auto PlatformViewLayerVisualUpdater::InternalGetNodeState(SInt64 const target, Vector<Dp>& offset, Rect<Dp>& clipRect, std::vector<Graphics::Path>& clipPaths, Float64& opacity) const -> void
{
    auto totalOffset = Vector<Dp>();
    auto accumulatedOpacity = Float64(1.0);
    for (auto i = SInt64(0); i <= target; ++i)
    {
        auto const& node = _nodeStack[static_cast<size_t>(i)];
        totalOffset += node.offset;
        accumulatedOpacity *= node.opacity;
    }

    auto currentOffset = -totalOffset;
    auto currentClipRect = Rect<Dp>::Infinite();
    auto currentClipPaths = std::vector<Graphics::Path>();
    for (auto i = SInt64(0); i <= target; ++i)
    {
        auto const& node = _nodeStack[static_cast<size_t>(i)];
        currentOffset += node.offset;
        currentClipRect = Rect<Dp>::Intersect(Rect<Dp>::Offset(currentClipRect, currentOffset), node.clipRect);

        if (node.clipPath)
        {
            currentClipPaths.push_back(*node.clipPath);
            currentClipPaths.back().Translate(currentOffset);
        }
    }
    offset = currentOffset;
    clipRect = currentClipRect;
    clipPaths = currentClipPaths;
    opacity = accumulatedOpacity;
}

auto PlatformViewLayerVisualUpdater::InternalGetBaseNodeIndex() const -> SInt64
{
    if (!_baseVisualNodeIndexStack.empty())
    {
        return _baseVisualNodeIndexStack.back();
    }
    return 0;
}

auto PlatformViewLayerVisualUpdater::InternalGetBaseVisual() const -> Shared<PlatformViewLayerVisual>
{
    if (!_baseVisualStack.empty())
    {
        return _baseVisualStack.back();
    }
    return {};
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
