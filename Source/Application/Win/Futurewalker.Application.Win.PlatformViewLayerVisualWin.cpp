// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformViewLayerVisualWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"
#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.DisplayList.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
PlatformViewLayerVisualWin::PlatformViewLayerVisualWin(Shared<PlatformDCompositionDeviceWin> const& device)
  : _device {device}
{
    _visual = _device->CreateVisual();
    _surface = Shared<PlatformViewLayerSurfaceWin>::Make();
    _surface->SetVisual(_visual);
}

auto PlatformViewLayerVisualWin::AddChild(Shared<PlatformViewLayerVisualWin> const& child) -> void
{
    if (child)
    {
        if (auto const& existingParent = child->GetParent())
        {
            existingParent->RemoveChild(child);
        }
        _children.push_back(child);
    }
}

auto PlatformViewLayerVisualWin::RemoveChild(Shared<PlatformViewLayerVisualWin> const& child) -> void
{
    if (child)
    {
        auto const it = std::find(_children.begin(), _children.end(), child);
        if (it != _children.end())
        {
            _children.erase(it);
        }
    }
}

auto PlatformViewLayerVisualWin::GetChildren() -> PlatformViewLayerVisualWinArray const&
{
    return _children;
}

auto PlatformViewLayerVisualWin::GetParent() -> Shared<PlatformViewLayerVisualWin>
{
    return _parent.Lock();
}

auto PlatformViewLayerVisualWin::GetVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    return _visual;
}

auto PlatformViewLayerVisualWin::GetBaseLayerId() const -> PlatformViewLayerId
{
    return _baseLayerId; 
}

auto PlatformViewLayerVisualWin::SetBaseLayerId(PlatformViewLayerId const layerId) -> void
{
    _baseLayerId = layerId;
}

auto PlatformViewLayerVisualWin::SetOffset(Offset<Dp> const& offset) -> void
{
    if (_offset != offset)
    {
        _offset = offset;
        UpdateOffset();
    }
}

auto PlatformViewLayerVisualWin::SetClipRect(Rect<Dp> const& clipRect) -> void
{
    if (!clipRect.IsFinite())
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_clipRect != clipRect)
    {
        _clipRect = clipRect;
        UpdateClipRect();
    }
}

auto PlatformViewLayerVisualWin::SetOpacity(Float64 const opacity) -> void
{
    if (!Float64::IsFinite(opacity))
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_opacity != opacity)
    {
        _opacity = opacity;
        UpdateOpacity();
    }
}

auto PlatformViewLayerVisualWin::SetDisplayScale(DisplayScale const displayScale) -> void
{
    if (!DisplayScale::IsFinite(displayScale))
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_displayScale != displayScale)
    {
        _displayScale = displayScale;
        UpdateOffset();
        UpdateClipRect();
        Invalidate();
    }
}

auto PlatformViewLayerVisualWin::SetBackingScale(BackingScale const backingScale) -> void
{
    if (!BackingScale::IsFinite(backingScale))
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_backingScale != backingScale)
    {
        _backingScale = backingScale;
        UpdateOffset();
        UpdateClipRect();
        Invalidate();
    }
}

auto PlatformViewLayerVisualWin::InsertFragment(const SInt64 index, PlatformViewLayerId const layerId, Fragment const& fragment) -> void
{
    if (0 <= index && index <= GetFragmentCount())
    {
        if (_fragments.Insert(index, layerId, fragment))
        {
            Invalidate();
        }
    }
}

auto PlatformViewLayerVisualWin::ReplaceFragment(const SInt64 index, PlatformViewLayerId const layerId, Fragment const& fragment) -> void
{
    if (0 <= index && index < GetFragmentCount())
    {
        if (_fragments.Replace(index, layerId, fragment))
        {
            Invalidate();
        }
    }
}

auto PlatformViewLayerVisualWin::ReplaceFragment(PlatformViewLayerId layerId, Fragment const& fragment) -> void
{
    if (auto const idx = _fragments.Find(layerId))
    {
        ReplaceFragment(*idx, layerId, fragment);
    }
}

auto PlatformViewLayerVisualWin::RemoveFragment(const SInt64 index) -> void
{
    if (0 <= index && index < GetFragmentCount())
    {
        if (_fragments.Erase(index))
        {
            Invalidate();
        }
    }
}

auto PlatformViewLayerVisualWin::ClearFragments() -> void
{
    if (!_fragments.IsEmpty())
    {
        _fragments.Clear();
        Invalidate();
    }
}

auto PlatformViewLayerVisualWin::GetFragmentIndexByLayerId(PlatformViewLayerId const layerId) const -> Optional<SInt64>
{
    return _fragments.Find(layerId);
}

auto PlatformViewLayerVisualWin::GetFragment(SInt64 const index) const -> Optional<Fragment>
{
    if (0 <= index && index < GetFragmentCount())
    {
        return _fragments.GetValue(index);
    }
    return {};
}

auto PlatformViewLayerVisualWin::GetFragmentCount() const -> SInt64
{
    return _fragments.GetSize();
}

auto PlatformViewLayerVisualWin::Render() -> void
{
    if (!_invalidated)
    {
        FW_DEBUG_LOG_INFO("PlatformViewLayerVisualWin::Render: id={} (skipped)", (int)static_cast<UInt64>(_baseLayerId));
        return;
    }

    FW_DEBUG_LOG_INFO("PlatformViewLayerVisualWin::Render: id={}", (int)static_cast<UInt64>(_baseLayerId));

    auto unionRect = Rect<Dp>();
    for (auto const& [_, fragment] : _fragments)
    {
        if (fragment.displayList)
        {
            if (fragment.clipRect.IsFinite())
            {
                auto const rect = Rect<Dp>::Offset(fragment.clipRect, fragment.offset);
                unionRect = Rect<Dp>::Union(unionRect, rect);
            }
            else if (fragment.displayList->GetBounds().IsFinite())
            {
                auto const rect = Rect<Dp>::Offset(fragment.displayList->GetBounds(), fragment.offset + fragment.displayListOffset);
                unionRect = Rect<Dp>::Union(unionRect, rect);
            }
            else
            {
                unionRect = _clipRect;
            }
        }
    }
    FW_DEBUG_ASSERT(unionRect.IsFinite());

    _surface->SetSize(unionRect.GetSize());
    _surface->SetDisplayScale(_displayScale);
    _surface->SetBackingScale(_backingScale);
    _surface->Draw([&](Graphics::Scene& scene) {
        for (auto const& [_, fragment] : _fragments)
        {
            if (fragment.displayList)
            {
                auto const scale = static_cast<Float64>(_displayScale) * static_cast<Float64>(_backingScale);
                scene.PushTranslate({.x = fragment.offset.GetDeltaX() - unionRect.GetLeft(), .y = fragment.offset.GetDeltaY() - unionRect.GetTop()});
                scene.PushScale({.x = scale, .y = scale});
                scene.PushClipRect({.rect = fragment.clipRect});
                scene.PushTranslate({.x = fragment.displayListOffset.GetDeltaX(), .y = fragment.displayListOffset.GetDeltaY()});
                scene.AddDisplayList({.displayList = fragment.displayList});
                scene.Pop({.count = 4});
            }
        }
    });

    auto const translation = D2D1::Matrix4x4F::Translation(
      static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(unionRect.GetPosition().GetX(), _displayScale, _backingScale))),
      static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(unionRect.GetPosition().GetY(), _displayScale, _backingScale))),
      0);
    _visual->SetTransform(translation);
}

auto PlatformViewLayerVisualWin::Invalidate() -> void
{
    if (!_invalidated)
    {
        FW_DEBUG_LOG_INFO("PlatformViewLayerVisualWin::Invalidate: id={}", (int)static_cast<UInt64>(_baseLayerId));
        _invalidated = true;
    }
}

auto PlatformViewLayerVisualWin::UpdateOffset() -> void
{
    if (_visual)
    {
        _visual->SetOffsetX(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(_offset.GetDeltaX(), _displayScale, _backingScale))));
        _visual->SetOffsetY(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(_offset.GetDeltaY(), _displayScale, _backingScale))));
    }
}

auto PlatformViewLayerVisualWin::UpdateClipRect() -> void
{
    if (_visual)
    {
        if (_clipRect.IsFinite())
        {
            auto const clip = _device->CreateRectangleClip();
            clip->SetLeft(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(_clipRect.GetLeft(), _displayScale, _backingScale))));
            clip->SetTop(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(_clipRect.GetTop(), _displayScale, _backingScale))));
            clip->SetRight(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(_clipRect.GetRight(), _displayScale, _backingScale))));
            clip->SetBottom(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(_clipRect.GetBottom(), _displayScale, _backingScale))));
            _visual->SetClip(clip.Get());
        }
        else
        {
            _visual->SetClip(nullptr);
        }
    }
}

auto PlatformViewLayerVisualWin::UpdateOpacity() -> void
{
    if (_visual)
    {
        _visual->SetOpacity(static_cast<FLOAT>(_opacity));
    }
}
}
