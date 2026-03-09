// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayerVisualPropertyUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto PlatformViewLayerVisualPropertyUpdater::PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Optional<Graphics::Path> const& clipPath, Float64 const& opacity) -> void
{
    _nodeStack.push_back({
        .id = id,
        .offset = offset,
        .clipRect = clipRect,
        .clipPath = clipPath,
        .opacity = opacity,
    });

    // Ignore initial PushNode() before first visual is pushed.
    if (InternalGetBaseVisual())
    {
        InternalReplaceNextPushNodeFragment(offset, clipRect, clipPath, opacity);
    }
}

auto PlatformViewLayerVisualPropertyUpdater::PopNode() -> void
{
    FW_DEBUG_ASSERT(!_nodeStack.empty());
    _nodeStack.pop_back();
    InternalReplaceNextPopNodeFragment();
}

auto PlatformViewLayerVisualPropertyUpdater::FindNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisual>
{
    if (auto const visual = InternalGetBaseVisual())
    {
        if (visual->GetBaseLayerId() == id)
        {
            return visual;
        }

        for (auto const& child : visual->GetChildren())
        {
            if (child->GetBaseLayerId() == id)
            {
                return child;
            }
        }
    }
    return InternalGetCurrentVisual();
}

auto PlatformViewLayerVisualPropertyUpdater::PushVisual(PlatformViewLayerId const id) -> void
{
    if (auto const nextVisual = FindNextVisual(id))
    {
        InternalSetCurrentVisual(nextVisual);
        InternalSetCurrentVisualProperties(nextVisual, InternalGetCurrentNodeIndex());
        InternalPushBaseVisual(nextVisual);
    }
    else
    {
        FW_DEBUG_ASSERT(false);
    }
}

auto PlatformViewLayerVisualPropertyUpdater::PopVisual() -> void
{
    InternalPopBaseVisual();
    InternalSetCurrentVisual(nullptr);
}

auto PlatformViewLayerVisualPropertyUpdater::UpdateFragment(Shared<PlatformViewLayer> const& layer) -> void
{
    if (!layer)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    auto visual = InternalGetCurrentVisual();
    if (!visual)
    {
        auto const id = layer->GetId();
        visual = FindNextVisual(id);

        for (auto i = InternalGetBaseNodeIndex() + 1; i <= InternalGetCurrentNodeIndex(); ++i)
        {
            auto const& node = _nodeStack[static_cast<size_t>(i)];
            visual->ReplacePushNodeFragment(
              _currentFragmentIndex++,
              {
                  .offset = node.offset,
                  .clipRect = node.clipRect,
                  .clipPath = node.clipPath,
                  .opacity = node.opacity,
              });
        }
        InternalSetCurrentVisualProperties(visual, InternalGetCurrentNodeIndex());
        InternalSetCurrentVisual(visual);
        InternalPushBaseVisual(visual);
    }
    InternalReplaceNextDisplayListFragment(layer->GetDisplayList(), layer->GetDisplayListOffset());
}

auto PlatformViewLayerVisualPropertyUpdater::UpdateCore(Shared<PlatformViewLayer> const& layer) -> void
{
    auto const layerId = layer->GetId();
    auto const layerOffset = layer->GetOffset();
    auto const layerOpacity = layer->GetOpacity();
    auto const layerClipRect = layer->GetClipMode() == ViewClipMode::Bounds ? Rect<Dp>({}, layer->GetSize()) : Rect<Dp>::Infinite();
    auto const layerClipPath = layer->GetClipPath();
    PushNode(layerId, layerOffset, layerClipRect, layerClipPath, layerOpacity);

    auto const needsSurface = layer->ShouldRasterize();
    if (needsSurface)
    {
        PushVisual(layerId);
    }

    UpdateFragment(layer);

    for (auto const& child : layer->GetChildren())
    {
        UpdateCore(child.As<PlatformViewLayer>());
    }

    if (needsSurface)
    {
        PopVisual();
    }
    PopNode();
}

auto PlatformViewLayerVisualPropertyUpdater::Update(Shared<PlatformViewLayerVisual> const& visual, Shared<PlatformViewLayer> const& layer) -> void
{
    _nodeStack.clear();
    _baseVisualNodeIndexStack.clear();
    _baseVisualStack.clear();
    _currentVisual = visual;
    _currentFragmentIndex = 0;
    UpdateCore(layer);
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisual>
{
    return _currentVisual;
}

auto PlatformViewLayerVisualPropertyUpdater::InternalSetCurrentVisual(Shared<PlatformViewLayerVisual> const& visual) -> void
{
    _currentVisual = visual;
}

auto PlatformViewLayerVisualPropertyUpdater::InternalSetCurrentVisualProperties(Shared<PlatformViewLayerVisual> const& visual, SInt64 const target) -> void
{
    if (visual)
    {
        auto offset = Offset<Dp>();
        auto clipRect = Rect<Dp>();
        auto clipPaths = std::vector<Graphics::Path>();
        auto opacity = Float64(1.0);
        InternalGetNodeState(target, offset, clipRect, clipPaths, opacity);
        visual->SetOffset(offset);
        visual->SetClipRect(clipRect);
        visual->SetClipPaths(clipPaths);
        visual->SetOpacity(opacity);
    }
}

auto PlatformViewLayerVisualPropertyUpdater::InternalPushBaseVisual(Shared<PlatformViewLayerVisual> const& visual) -> void
{
    _baseVisualNodeIndexStack.push_back(std::ssize(_nodeStack) - 1);
    _baseVisualStack.push_back(visual);
}

auto PlatformViewLayerVisualPropertyUpdater::InternalPopBaseVisual() -> void
{
    _baseVisualNodeIndexStack.pop_back();
    _baseVisualStack.pop_back();
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetNodeState(SInt64 const target, Offset<Dp>& offset, Rect<Dp>& clipRect, std::vector<Graphics::Path>& clipPaths, Float64& opacity) const -> void
{
    auto totalOffset = Offset<Dp>();
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

auto PlatformViewLayerVisualPropertyUpdater::InternalGetBaseNodeIndex() const -> SInt64
{
    if (!_baseVisualNodeIndexStack.empty())
    {
        return _baseVisualNodeIndexStack.back();
    }
    return 0;
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetBaseVisual() const -> Shared<PlatformViewLayerVisual>
{
    if (!_baseVisualStack.empty())
    {
        return _baseVisualStack.back();
    }
    return {};
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetCurrentNodeIndex() const -> SInt64
{
    return static_cast<SInt64>(std::ssize(_nodeStack)) - 1;
}

auto PlatformViewLayerVisualPropertyUpdater::InternalReplaceNextPushNodeFragment(Offset<Dp> const& offset, Rect<Dp> const& clipRect, Optional<Graphics::Path> const& clipPath, Float64 const& opacity) -> void
{
    if (auto const visual = InternalGetCurrentVisual())
    {
        FW_DEBUG_ASSERT(InternalGetBaseVisual());
        visual->ReplacePushNodeFragment(
          _currentFragmentIndex++,
          {
              .offset = offset,
              .clipRect = clipRect,
              .clipPath = clipPath,
              .opacity = opacity,
          });
    }
}

auto PlatformViewLayerVisualPropertyUpdater::InternalReplaceNextPopNodeFragment() -> void
{
    if (InternalGetCurrentVisual())
    {
        FW_DEBUG_ASSERT(InternalGetBaseVisual());
        _currentFragmentIndex++;
    }
}

auto PlatformViewLayerVisualPropertyUpdater::InternalReplaceNextDisplayListFragment(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& displayListOffset) -> void
{
    if (auto const visual = InternalGetCurrentVisual())
    {
        FW_DEBUG_ASSERT(InternalGetBaseVisual());
        visual->ReplaceDisplayListFragment(
          _currentFragmentIndex++,
          {
              .displayList = displayList,
              .displayListOffset = displayListOffset,
          });
    }
}
}
