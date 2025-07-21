// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ViewDrawInfo.hpp"
#include "Futurewalker.Application.ViewLayer.hpp"
#include "Futurewalker.Application.ViewLayerManager.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"
#include "Futurewalker.Application.DrawableViewLayer.hpp"

#include "Futurewalker.Graphics.DisplayList.hpp"

#include "Futurewalker.Unit.UnitFunction.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace
{
///
/// @brief Expand bounds to align to pixel grid.
///
auto AlignBoundsToPixelGridByExpand(Rect<Dp> const& bounds, DisplayScale const displayScale, BackingScale const backingScale) -> Rect<Dp>
{
    auto const l = Px::Floor(UnitFunction::ConvertDpToPx(bounds.GetLeft(), displayScale, backingScale));
    auto const t = Px::Floor(UnitFunction::ConvertDpToPx(bounds.GetTop(), displayScale, backingScale));
    auto const r = Px::Ceil(UnitFunction::ConvertDpToPx(bounds.GetRight(), displayScale, backingScale));
    auto const b = Px::Ceil(UnitFunction::ConvertDpToPx(bounds.GetBottom(), displayScale, backingScale));
    return UnitFunction::ConvertPxToDp(Rect<Px>(l, t, r, b), backingScale, displayScale);
}
}

///
/// @brief
///
ViewDrawInfo::ViewDrawInfo()
{
    _drawableLayerId = AddSubLayerInfo(ViewLayerKindDrawable);
}

///
/// @brief Invalidate layer.
///
/// @return true if invalidated.
///
auto ViewDrawInfo::InvalidateLayer() -> Bool
{
    if (_invalidated != true)
    {
        _invalidated = true;
        return true;
    }
    return false;
}

///
/// @brief Invalidate path.
///
/// @return true if invalidated.
///
auto ViewDrawInfo::InvalidatePath() -> Bool
{
    if (_invalidatedPath != true)
    {
        _invalidatedPath = true;
        return true;
    }
    return false;
}

///
/// @brief
///
auto ViewDrawInfo::IsInDrawPath() const -> Bool
{
    return _invalidatedPath;
}

///
/// @brief Begin drawing.
///
/// @param layer View layer to draw.
///
auto ViewDrawInfo::BeginDraw() -> Bool
{
    if (_invalidated)
    {
        return true;
    }
    return false;
}

///
/// @brief
///
auto ViewDrawInfo::EndDraw() -> void
{
    _invalidated = false;
}

///
/// @brief
///
auto ViewDrawInfo::BeginDrawPath() -> Bool
{
    if (_invalidatedPath)
    {
        return true;
    }
    return false;
}

///
/// @brief
///
auto ViewDrawInfo::EndDrawPath() -> void
{
    _invalidatedPath = false;
}

///
/// @brief
///
auto ViewDrawInfo::SetDisplayList(Shared<Graphics::DisplayList> const& displayList) -> void
{
    _displayList = displayList;
}

///
/// @brief
///
auto ViewDrawInfo::SetDisplayScale(DisplayScale const displayScale) -> void
{
    _displayScale = displayScale;
}

///
/// @brief
///
auto ViewDrawInfo::SetBackingScale(BackingScale const backingScale) -> void
{
    _backingScale = backingScale;
}

///
/// @brief
///
auto ViewDrawInfo::SetClipMode(ViewClipMode const clipMode) -> void
{
    _clipMode = clipMode;
}

///
/// @brief
///
auto ViewDrawInfo::SetOpacity(Float64 const opacity) -> void
{
    _opacity = opacity;
}

///
/// @brief
///
auto ViewDrawInfo::UpdateLayers(ViewLayer& layer) -> void
{
    layer.SetClipMode(_clipMode);
    layer.SetOpacity(_opacity);

    if (const auto drawableLayer = GetSubLayer(_drawableLayerId))
    {
        if (_displayList)
        {
            // We assume origin of the bounds is properly aligned by layout algorithm.
            auto const bounds = AlignBoundsToPixelGridByExpand(_displayList->GetBounds(), _displayScale, _backingScale);
            auto const offset = bounds.GetPosition().As<Offset>();
            auto const scale = static_cast<Float64>(_displayScale) * static_cast<Float64>(_backingScale);

            drawableLayer->SetOffset(offset);
            drawableLayer->SetSize(bounds.GetSize());
            drawableLayer.As<DrawableViewLayer>()->Draw(_displayList, -offset, scale);
        }
        else
        {
            drawableLayer.As<DrawableViewLayer>()->Draw(nullptr, {}, 1.0);
        }
    }
}

///
/// @brief
///
/// @param id
///
auto ViewDrawInfo::GetSubLayer(ViewLayerId const id) -> Shared<ViewLayer>
{
    const auto it = std::find_if(_subLayerInfo.begin(), _subLayerInfo.end(), [&](const auto& info) { return *info.id == id; });
    if (it != _subLayerInfo.end())
    {
        return it->layer;
    }
    return {};
}

///
/// @brief
///
/// @param kind
///
auto ViewDrawInfo::AddSubLayerInfo(ViewLayerKind const kind) -> ViewLayerId
{
    const auto& info = _subLayerInfo.emplace_back(
      SubLayerInfo {
          .id = Unique<UniqueViewLayerId>::Make(),
          .kind = kind,
          .layer = {},
      });
    return *info.id;
}

///
/// @brief
///
/// @param id
///
auto ViewDrawInfo::RemoveSubLayerInfo(ViewLayerId const id) -> void
{
    const auto it = std::remove_if(_subLayerInfo.begin(), _subLayerInfo.end(), [&](const auto& info) { return *info.id == id; });
    _subLayerInfo.erase(it, _subLayerInfo.end());
}

///
/// @brief
///
/// @param layer
/// @param layerManager
/// @param id
///
auto ViewDrawInfo::AttachSubLayer(ViewLayer& layer, ViewLayerManager& layerManager, ViewLayerId const id) -> void
{
    const auto it = std::remove_if(_subLayerInfo.begin(), _subLayerInfo.end(), [&](const auto& info) { return *info.id == id; });
    if (it != _subLayerInfo.end())
    {
        if (!it->layer)
        {
            it->layer = layerManager.MakeLayer(it->kind);
        }

        if (it->layer)
        {
            layer.AddChild(it->layer, FindSubLayerAttachPosition(layer, id).GetPointer());
        }
    }
}

///
/// @brief
///
/// @param layer
/// @param id
///
auto ViewDrawInfo::DetachSubLayer(ViewLayer& layer, ViewLayerId const id) -> void
{
    const auto it = std::remove_if(_subLayerInfo.begin(), _subLayerInfo.end(), [&](const auto& info) { return *info.id == id; });
    if (it != _subLayerInfo.end())
    {
        if (it->layer)
        {
            layer.RemoveChild(it->layer);
        }
    }
}

///
/// @brief
///
/// @param layer
/// @param layerManager
///
auto ViewDrawInfo::AttachAllSubLayers(ViewLayer& layer, ViewLayerManager& layerManager) -> void
{
    for (auto& info : _subLayerInfo)
    {
        if (!info.layer)
        {
            info.layer = layerManager.MakeLayer(info.kind);
        }

        if (info.layer)
        {
            layer.AddChild(info.layer, FindSubLayerAttachPosition(layer, *info.id).GetPointer());
        }
    }
}

///
/// @brief
///
/// @param layer
///
auto ViewDrawInfo::DetachAllSubLayers(ViewLayer& layer) -> void
{
    for (auto& info : _subLayerInfo)
    {
        if (info.layer)
        {
            layer.RemoveChild(info.layer);
        }
    }
}

///
/// @brief
///
/// @param layer
/// @param id
///
auto ViewDrawInfo::FindSubLayerAttachPosition(ViewLayer& layer, ViewLayerId const id) -> Shared<ViewLayer>
{
    auto it = std::find_if(_subLayerInfo.begin(), _subLayerInfo.end(), [&](const auto& info) { return *info.id == id; });
    if (it == _subLayerInfo.end())
    {
        return layer.GetChildAt(0);
    }

    for (it = std::next(it); it != _subLayerInfo.end(); ++it)
    {
        if (it->layer)
        {
            FW_DEBUG_ASSERT(layer.GetChildIndex(it->layer));
            return it->layer;
        }
        FW_DEBUG_ASSERT(false);
    }
    return layer.GetChildAt(0);
}
}
