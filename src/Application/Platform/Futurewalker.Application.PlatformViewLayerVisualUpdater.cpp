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

auto PlatformViewLayerVisualUpdater::Update(Shared<PlatformViewLayerVisual> const& visual, Shared<PlatformViewLayer> const& layer, PlatformViewLayerVisualRenderParams const& renderParams) -> void
{
    _nodeStack.clear();
    _baseVisualNodeIndexStack.clear();
    _baseVisualStack.clear();
    _currentVisual = visual;
    UpdateCore(layer, renderParams);
}

auto PlatformViewLayerVisualUpdater::PushVisual(PlatformViewLayerId const id, PlatformViewLayerVisualRenderParams const& renderParams) -> void
{
    FW_DEBUG_ASSERT(!_nodeStack.empty());

    if (auto const nextVisual = InternalRemoveUntilNextVisual(id))
    {
        nextVisual->ClearFragments();
        InternalSetCurrentVisualProperties(nextVisual, InternalGetCurrentNodeIndex(), renderParams);
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
    InternalSetCurrentVisualProperties(visual, InternalGetCurrentNodeIndex(), renderParams);
    InternalSetCurrentVisual(visual);
    InternalPushBaseVisual(visual);
}

auto PlatformViewLayerVisualUpdater::PopVisual() -> void
{
    InternalPopBaseVisual();
    InternalSetCurrentVisual(nullptr);
}

auto PlatformViewLayerVisualUpdater::PushNode(PlatformViewLayerId const& id, Vector2<Dp> const& offset, Matrix3x3<Dp> const& transform, Rect<Dp> const& clipRect, Optional<Graphics::Path> const& clipPath, Float64 const& opacity) -> void
{
    _nodeStack.push_back({
        .id = id,
        .offset = offset,
        .transform = transform,
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
              .transform = transform,
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

auto PlatformViewLayerVisualUpdater::AddFragment(
  PlatformViewLayerId const id,
  PlatformViewLayerVisualRenderParams const& renderParams,
  Shared<Graphics::DisplayList> const& displayList,
  Vector2<Dp> const& displayListOffset) -> void
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

    visual->AddDisplayListFragment(
      id,
      {
          .displayList = displayList,
          .displayListOffset = displayListOffset,
      });
}

auto PlatformViewLayerVisualUpdater::UpdateCore(Shared<PlatformViewLayer> const& layer, PlatformViewLayerVisualRenderParams const& renderParams) -> void
{
    auto const id = layer->GetId();
    auto const offset = layer->GetOffset();
    auto const transform = layer->GetTransform();
    auto const opacity = layer->GetOpacity();
    auto const clipRect = layer->GetClipMode() == ViewClipMode::Bounds ? Rect<Dp>::Make({}, layer->GetSize()) : Rect<Dp>::Infinite();
    auto const clipPath = layer->GetClipPath();
    PushNode(id, offset, transform, clipRect, clipPath, opacity);

    auto const needsSurface = layer->ShouldRasterize();
    if (needsSurface)
    {
        PushVisual(id, renderParams);
    }

    auto const displayList = layer->GetDisplayList();
    auto const displayListOffset = layer->GetDisplayListOffset();
    AddFragment(id, renderParams, displayList, displayListOffset);

    for (auto const& child : layer->GetChildren())
    {
        UpdateCore(child, renderParams);
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

auto PlatformViewLayerVisualUpdater::InternalSetCurrentVisualProperties(Shared<PlatformViewLayerVisual> const& visual, SInt64 const target, PlatformViewLayerVisualRenderParams const& renderParams) -> void
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
        visual->SetClipPaths(clipPaths);
        visual->SetOpacity(opacity);
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

auto PlatformViewLayerVisualUpdater::InternalGetNodeState(SInt64 const target, Vector2<Dp>& offset, Matrix3x3<Dp>& transform, Rect<Dp>& clipRect, std::vector<Graphics::Path>& clipPaths, Float64& opacity) const -> void
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
