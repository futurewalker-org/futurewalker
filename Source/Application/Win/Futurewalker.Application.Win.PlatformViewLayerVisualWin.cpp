// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformViewLayerVisualWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"
#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.DisplayList.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto PlatformViewLayerVisualWin::Make(Shared<PlatformDCompositionDeviceWin> const& device) -> Shared<PlatformViewLayerVisualWin>
{
    return PlatformViewLayerVisual::MakeDerived<PlatformViewLayerVisualWin>(device);
}

PlatformViewLayerVisualWin::PlatformViewLayerVisualWin(PassKey<PlatformViewLayerVisual> key, Shared<PlatformDCompositionDeviceWin> const& device)
  : PlatformViewLayerVisual(key)
  , _device {device}

{
}

auto PlatformViewLayerVisualWin::GetVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    return _visual;
}

auto PlatformViewLayerVisualWin::Render() -> void
{
    if (!_invalidated)
    {
        return;
    }

    auto unionRect = Rect<Dp>();
    ForEachFragment([&](auto const& fragment) {
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
                unionRect = GetClipRect();
            }
        }
    });
    FW_DEBUG_ASSERT(unionRect.IsFinite());

    auto const displayScale = GetDisplayScale();
    auto const backingScale = GetBackingScale();
    auto const scale = static_cast<Float64>(displayScale) * static_cast<Float64>(backingScale);

    _surface->SetSize(unionRect.GetSize());
    _surface->SetDisplayScale(GetDisplayScale());
    _surface->SetBackingScale(GetBackingScale());
    _surface->Draw([&](Graphics::Scene& scene) {
        ForEachFragment([&](auto const& fragment) {
            if (fragment.displayList)
            {
                scene.PushTranslate({.x = fragment.offset.GetDeltaX() - unionRect.GetLeft(), .y = fragment.offset.GetDeltaY() - unionRect.GetTop()});
                scene.PushScale({.x = scale, .y = scale});
                scene.PushClipRect({.rect = fragment.clipRect});
                scene.PushTranslate({.x = fragment.displayListOffset.GetDeltaX(), .y = fragment.displayListOffset.GetDeltaY()});
                scene.AddDisplayList({.displayList = fragment.displayList});
                scene.Pop({.count = 4});
            }
        });
    });

    auto const translation = D2D1::Matrix4x4F::Translation(
      static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(unionRect.GetPosition().GetX(), displayScale, backingScale))),
      static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(unionRect.GetPosition().GetY(), displayScale, backingScale))),
      0);
    _visual->SetTransform(translation);

    _invalidated = false;
}

auto PlatformViewLayerVisualWin::Invalidate() -> void
{
    if (!_invalidated)
    {
        _invalidated = true;
    }
}

auto PlatformViewLayerVisualWin::Initialize() -> void
{
    _visual = _device->CreateVisual();
    _surface = Shared<PlatformViewLayerSurfaceWin>::Make();
    _surface->SetVisual(_visual);
}

auto PlatformViewLayerVisualWin::OnFragmentChanged() -> void
{
    Invalidate();
}

auto PlatformViewLayerVisualWin::OnOffsetChanged() -> void
{
    if (_visual)
    {
        auto const offset = GetOffset();
        auto const displayScale = GetDisplayScale();
        auto const backingScale = GetBackingScale();
        _visual->SetOffsetX(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(offset.GetDeltaX(), displayScale, backingScale))));
        _visual->SetOffsetY(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(offset.GetDeltaY(), displayScale, backingScale))));
    }
}

auto PlatformViewLayerVisualWin::OnClipRectChanged() -> void
{
    if (_visual)
    {
        auto const clipRect = GetClipRect();
        auto const displayScale = GetDisplayScale();
        auto const backingScale = GetBackingScale();
        if (clipRect.IsFinite())
        {
            auto const clip = _device->CreateRectangleClip();
            clip->SetLeft(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(clipRect.GetLeft(), displayScale, backingScale))));
            clip->SetTop(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(clipRect.GetTop(), displayScale, backingScale))));
            clip->SetRight(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(clipRect.GetRight(), displayScale, backingScale))));
            clip->SetBottom(static_cast<FLOAT>(Px::Round(UnitFunction::ConvertDpToPx(clipRect.GetBottom(), displayScale, backingScale))));
            _visual->SetClip(clip.Get());
        }
        else
        {
            _visual->SetClip(nullptr);
        }
    }
}

auto PlatformViewLayerVisualWin::OnOpacityChanged() -> void
{
    if (_visual)
    {
        _visual->SetOpacity(static_cast<FLOAT>(GetOpacity()));
    }
}

auto PlatformViewLayerVisualWin::OnDisplayScaleChanged() -> void
{
    Invalidate();
}

auto PlatformViewLayerVisualWin::OnBackingScaleChanged() -> void
{
    Invalidate();
}
}
