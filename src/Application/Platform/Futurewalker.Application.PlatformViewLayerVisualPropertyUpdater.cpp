// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayerVisualPropertyUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto PlatformViewLayerVisualPropertyUpdater::PushNode(PlatformViewLayerId const& id, Vector2<Dp> const& offset, Matrix3x3<Dp> const& transform, Rect<Dp> const& clipRect, Optional<Graphics::Path> const& clipPath, Float64 const& opacity) -> void
{
    _nodeStack.push_back({
        .id = id,
        .offset = offset,
        .transform = transform,
        .clipRect = clipRect,
        .clipPath = clipPath,
        .opacity = opacity,
    });

    // Ignore initial PushNode() before first visual is pushed.
    if (InternalGetBaseVisual())
    {
        InternalReplaceNextPushNodeFragment(offset, transform, clipRect, clipPath, opacity);
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

auto PlatformViewLayerVisualPropertyUpdater::PushVisual(PlatformViewLayerId const id, PlatformViewLayerVisualRenderParams const& renderParams) -> void
{
    if (auto const nextVisual = FindNextVisual(id))
    {
        InternalSetCurrentVisual(nextVisual);
        InternalSetCurrentVisualProperties(nextVisual, InternalGetCurrentNodeIndex(), renderParams);
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

auto PlatformViewLayerVisualPropertyUpdater::UpdateFragment(
  PlatformViewLayerId const id,
  PlatformViewLayerVisualRenderParams const& renderParams,
  Shared<Graphics::DisplayList> const& displayList,
  Vector2<Dp> const& displayListOffset) -> void
{
    auto visual = InternalGetCurrentVisual();
    if (!visual)
    {
        visual = FindNextVisual(id);

        for (auto i = InternalGetBaseNodeIndex() + 1; i <= InternalGetCurrentNodeIndex(); ++i)
        {
            auto const& node = _nodeStack[static_cast<size_t>(i)];
            visual->ReplacePushNodeFragment(
              _currentFragmentIndex++,
              {
                  .offset = node.offset,
                  .transform = node.transform,
                  .clipRect = node.clipRect,
                  .clipPath = node.clipPath,
                  .opacity = node.opacity,
              });
        }
        InternalSetCurrentVisualProperties(visual, InternalGetCurrentNodeIndex(), renderParams);
        InternalSetCurrentVisual(visual);
        InternalPushBaseVisual(visual);
    }
    InternalReplaceNextDisplayListFragment(displayList, displayListOffset);
}

auto PlatformViewLayerVisualPropertyUpdater::UpdateCore(Shared<PlatformViewLayer> const& layer, PlatformViewLayerVisualRenderParams const& renderParams) -> void
{
    auto const layerId = layer->GetId();
    auto const layerOffset = layer->GetOffset();
    auto const layerTransform = layer->GetTransform();
    auto const layerOpacity = layer->GetOpacity();
    auto const layerClipRect = layer->GetClipMode() == ViewClipMode::Bounds ? Rect<Dp>::Make({}, layer->GetSize()) : Rect<Dp>::Infinite();
    auto const layerClipPath = layer->GetClipPath();
    PushNode(layerId, layerOffset, layerTransform, layerClipRect, layerClipPath, layerOpacity);

    auto const needsSurface = layer->ShouldRasterize();
    if (needsSurface)
    {
        PushVisual(layerId, renderParams);
    }

    auto const displayList = layer->GetDisplayList();
    auto const displayListOffset = layer->GetDisplayListOffset();
    UpdateFragment(layerId, renderParams, displayList, displayListOffset);

    for (auto const& child : layer->GetChildren())
    {
        UpdateCore(child.As<PlatformViewLayer>(), renderParams);
    }

    if (needsSurface)
    {
        PopVisual();
    }
    PopNode();
}

auto PlatformViewLayerVisualPropertyUpdater::Update(Shared<PlatformViewLayerVisual> const& visual, Shared<PlatformViewLayer> const& layer, PlatformViewLayerVisualRenderParams const& renderParams) -> void
{
    _nodeStack.clear();
    _baseVisualNodeIndexStack.clear();
    _baseVisualStack.clear();
    _currentVisual = visual;
    _currentFragmentIndex = 0;
    UpdateCore(layer, renderParams);
}

auto PlatformViewLayerVisualPropertyUpdater::InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisual>
{
    return _currentVisual;
}

auto PlatformViewLayerVisualPropertyUpdater::InternalSetCurrentVisual(Shared<PlatformViewLayerVisual> const& visual) -> void
{
    _currentVisual = visual;
}

auto PlatformViewLayerVisualPropertyUpdater::InternalSetCurrentVisualProperties(Shared<PlatformViewLayerVisual> const& visual, SInt64 const target, PlatformViewLayerVisualRenderParams const& renderParams) -> void
{
    if (visual)
    {
        auto offset = Vector2<Dp>();
        auto transform = Matrix3x3<Dp>::MakeIdentity();
        auto clipRect = Rect<Dp>();
        auto clipPaths = std::vector<Graphics::Path>();
        auto opacity = Float64(1.0);
        InternalGetNodeState(target, offset, transform, clipRect, clipPaths, opacity);
        visual->SetRenderParams(renderParams);
        visual->SetOffset(offset);
        visual->SetTransform(transform);
        visual->SetClipRect(clipRect);
        visual->SetClipPaths(std::move(clipPaths));
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

auto PlatformViewLayerVisualPropertyUpdater::InternalGetNodeState(SInt64 const target, Vector2<Dp>& offset, Matrix3x3<Dp>& transform, Rect<Dp>& clipRect, std::vector<Graphics::Path>& clipPaths, Float64& opacity) const -> void
{
    auto transformClipRect = [](Rect<Dp> const& rect, Matrix3x3<Dp> const& transform) -> Rect<Dp> {
        auto const topLeft = transform * Vector3<Dp>(rect.x0, rect.y0, 1.0);
        auto const topRight = transform * Vector3<Dp>(rect.x1, rect.y0, 1.0);
        auto const bottomLeft = transform * Vector3<Dp>(rect.x0, rect.y1, 1.0);
        auto const bottomRight = transform * Vector3<Dp>(rect.x1, rect.y1, 1.0);
        auto const minX = std::min({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
        auto const maxX = std::max({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
        auto const minY = std::min({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
        auto const maxY = std::max({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
        return Rect<Dp>(minX, minY, maxX, maxY);
    };

    auto currentTransform = Matrix3x3<Dp>::MakeIdentity();
    auto currentClipRect = Rect<Dp>::Infinite();
    auto currentClipPaths = std::vector<Graphics::Path>();
    auto accumulatedOpacity = Float64(1.0);
    for (auto i = SInt64(0); i <= target; ++i)
    {
        auto const& node = _nodeStack[static_cast<size_t>(i)];
        currentTransform = currentTransform * Matrix3x3<Dp>::MakeTranslation(node.offset) * node.transform;
        currentClipRect = Rect<Dp>::Intersect(currentClipRect, transformClipRect(node.clipRect, currentTransform));
        accumulatedOpacity *= node.opacity;

        if (node.clipPath)
        {
            currentClipPaths.push_back(*node.clipPath);
            currentClipPaths.back().Transform(currentTransform);
        }
    }

    auto const currentClipRectOffset = currentClipRect.GetPosition().As<Vector2>();
    auto const currentClipRectSize = currentClipRect.GetSize();

    for (auto& clipPath : currentClipPaths)
    {
        clipPath.Translate(-currentClipRectOffset);
    }

    offset = currentClipRectOffset;
    transform = currentTransform;
    clipRect = Rect<Dp>::Make({0, 0}, currentClipRectSize);
    clipPaths = std::move(currentClipPaths);
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

auto PlatformViewLayerVisualPropertyUpdater::InternalReplaceNextPushNodeFragment(Vector2<Dp> const& offset, Matrix3x3<Dp> const& transform, Rect<Dp> const& clipRect, Optional<Graphics::Path> const& clipPath, Float64 const& opacity) -> void
{
    if (auto const visual = InternalGetCurrentVisual())
    {
        FW_DEBUG_ASSERT(InternalGetBaseVisual());
        visual->ReplacePushNodeFragment(
          _currentFragmentIndex++,
          {
              .offset = offset,
              .transform = transform,
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

auto PlatformViewLayerVisualPropertyUpdater::InternalReplaceNextDisplayListFragment(Shared<Graphics::DisplayList> const& displayList, Vector2<Dp> const& displayListOffset) -> void
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
