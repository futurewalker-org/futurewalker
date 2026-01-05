// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayerVisualPropertyUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto PlatformViewLayerVisualPropertyUpdater::PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Float64 const& opacity) -> void
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

auto PlatformViewLayerVisualPropertyUpdater::PopNode() -> void
{
    FW_DEBUG_ASSERT(!_nodeStack.empty());
    _nodeStack.pop_back();
}

auto PlatformViewLayerVisualPropertyUpdater::FindNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisual>
{
    if (auto const visual = InternalGetCurrentVisual())
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
    return {};
}

auto PlatformViewLayerVisualPropertyUpdater::PushVisual(PlatformViewLayerId const id) -> void
{
    if (auto const nextVisual = FindNextVisual(id))
    {
        InternalSetCurrentVisual(nextVisual);
        InternalPushVisualNodeIndex();
        InternalResetRelativePropertiesFromBase();
    }
    else
    {
        FW_DEBUG_ASSERT(false);
    }
}

auto PlatformViewLayerVisualPropertyUpdater::PopVisual() -> void
{
    InternalPopVisualNodeIndex();
    InternalSetCurrentVisual(nullptr);
}

auto PlatformViewLayerVisualPropertyUpdater::GetBaseNodeIndex() const -> SInt64
{
    if (!_visualNodeIndexStack.empty())
    {
        return _visualNodeIndexStack.back();
    }
    return 0;
}

auto PlatformViewLayerVisualPropertyUpdater::GetCurrentNodeIndex() const -> SInt64
{
    return static_cast<SInt64>(std::ssize(_nodeStack)) - 1;
}

auto PlatformViewLayerVisualPropertyUpdater::UpdateFragment(Shared<PlatformViewLayer> const& layer) -> void
{
    if (!layer)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    auto const id = layer->GetId();
    if (auto const visual = InternalGetCurrentVisual())
    {
        if (visual->GetBaseLayerId() == id)
        {
            visual->SetOffset(InternalGetCurrentOffset());
            visual->SetClipRect(InternalGetCurrentClipRect());
            visual->SetOpacity(InternalGetCurrentOpacity());

            for (auto const& child : visual->GetChildren())
            {
                if (child->GetBaseLayerId() == id)
                {
                    child->SetOffset(InternalGetCurrentOffset());
                    child->SetClipRect(InternalGetCurrentClipRect());
                    child->SetOpacity(InternalGetCurrentOpacity());
                }
            }
        }

        visual->ReplaceFragment(
          id,
          {
              .offset = InternalGetCurrentOffsetFromBase(),
              .clipRect = InternalGetCurrentClipRectFromBase(),
              .opacity = InternalGetCurrentOpacityFromBase(),
              .displayList = layer->GetDisplayList(),
              .displayListOffset = layer->GetDisplayListOffset(),
          });
    }
}

auto PlatformViewLayerVisualPropertyUpdater::UpdateCore(Shared<PlatformViewLayer> const& layer) -> void
{
    auto const layerId = layer->GetId();
    auto const layerOffset = layer->GetOffset();
    auto const layerClipRect = layer->GetClipMode() == ViewClipMode::Bounds ? Rect<Dp>({}, layer->GetSize()) : Rect<Dp>::Infinite();
    auto const layerOpacity = layer->GetOpacity();
    PushNode(layerId, layerOffset, layerClipRect, layerOpacity);

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
    _visualNodeIndexStack.clear();
    _currentVisual = visual;
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

auto PlatformViewLayerVisualPropertyUpdater::InternalPushVisualNodeIndex() -> void
{
    _visualNodeIndexStack.push_back(std::ssize(_nodeStack) - 1);
}

auto PlatformViewLayerVisualPropertyUpdater::InternalPopVisualNodeIndex() -> void
{
    _visualNodeIndexStack.pop_back();
}

auto PlatformViewLayerVisualPropertyUpdater::InternalResetRelativePropertiesFromBase() -> void
{
    if (!_nodeStack.empty())
    {
        auto& back = _nodeStack.back();
        back.currentOffsetFromBase = Offset<Dp>();
        back.currentClipRectFromBase = Rect<Dp>::Infinite();
        back.currentOpacityFromBase = 1.0;
    }
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetCurrentOffset() const -> Offset<Dp>
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentOffset;
    }
    return Offset<Dp>();
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetCurrentClipRect() const -> Rect<Dp>
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentClipRect;
    }
    return Rect<Dp>::Infinite();
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetCurrentOpacity() const -> Float64
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentOpacity;
    }
    return 1.0;
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetCurrentOffsetFromBase() const -> Offset<Dp>
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentOffsetFromBase;
    }
    return Offset<Dp>();
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetCurrentClipRectFromBase() const -> Rect<Dp>
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentClipRectFromBase;
    }
    return Rect<Dp>::Infinite();
}
auto PlatformViewLayerVisualPropertyUpdater::InternalGetCurrentOpacityFromBase() const -> Float64
{
    if (!_nodeStack.empty())
    {
        return _nodeStack.back().currentOpacityFromBase;
    }
    return 1.0;
}

}
