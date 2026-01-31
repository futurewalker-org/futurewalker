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

    auto const unionRect = CalcFragmentBounds();
    FW_DEBUG_ASSERT(unionRect.IsFinite());

    auto const& clipPaths = GetClipPaths();
    auto const displayScale = GetDisplayScale();
    auto const backingScale = GetBackingScale();
    auto const scale = static_cast<Float64>(displayScale) * static_cast<Float64>(backingScale);

    _surface->SetSize(unionRect.GetSize());
    _surface->SetOffset(unionRect.GetPosition().As<Offset>());
    _surface->SetDisplayScale(GetDisplayScale());
    _surface->SetBackingScale(GetBackingScale());
    _surface->Draw([&](Graphics::Scene& scene) {
        for (auto const& clipPath : clipPaths)
        {
            scene.PushClipPath({.path = clipPath});
        }
        scene.PushTranslate({.x = -unionRect.GetLeft(), .y = -unionRect.GetTop()});
        ForEachFragment([&](auto const& fragmentInfo) {
            if (fragmentInfo.type == FragmentType::PushNode)
            {
                if (auto const fragment = GetPushNodeFragment(fragmentInfo.index))
                {
                    scene.PushTranslate({.x = fragment->offset.GetDeltaX(), .y = fragment->offset.GetDeltaY()});
                    scene.PushScale({.x = scale, .y = scale});
                    scene.PushClipRect({.rect = fragment->clipRect});

                    if (fragment->clipPath)
                    {
                        scene.PushClipPath({.path = *fragment->clipPath});
                    }
                }
            }
            else if (fragmentInfo.type == FragmentType::PopNode)
            {
                if (auto const fragment = GetPopNodeFragment(fragmentInfo.index))
                {
                    scene.Pop({.count = 3});

                    if (auto const pushFragment = GetPushNodeFragment(fragment->pushNodeIndex))
                    {
                        if (pushFragment->clipPath)
                        {
                            scene.Pop({.count = 1});
                        }
                    }
                }
            }
            else if (fragmentInfo.type == FragmentType::DisplayList)
            {
                if (auto const fragment = GetDisplayListFragment(fragmentInfo.index))
                {
                    scene.PushTranslate({.x = fragment->displayListOffset.GetDeltaX(), .y = fragment->displayListOffset.GetDeltaY()});
                    scene.AddDisplayList({.displayList = fragment->displayList});
                    scene.Pop({.count = 1});
                }
            }
        });
        scene.Pop({.count = 1});
        scene.Pop({.count = std::ssize(clipPaths)});
    });

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

auto PlatformViewLayerVisualWin::CalcFragmentBounds() const -> Rect<Dp>
{
    auto offsets = std::vector<Offset<Dp>>();
    auto clipRects = std::vector<Rect<Dp>>();

    offsets.push_back(GetOffset());
    clipRects.push_back(GetClipRect());

    auto unionRect = Rect<Dp>();
    ForEachFragment([&](auto const& fragmentInfo) {
        if (fragmentInfo.type == FragmentType::PushNode)
        {
            if (auto const fragment = GetPushNodeFragment(fragmentInfo.index))
            {
                auto const currentOffset = offsets.empty() ? Offset<Dp>() : offsets.back();
                auto const currentClipRect = clipRects.empty() ? GetClipRect() : clipRects.back();
                offsets.push_back(currentOffset + fragment->offset);
                clipRects.push_back(Rect<Dp>::Intersect(currentClipRect, Rect<Dp>::Offset(fragment->clipRect, offsets.back())));
            }
        }
        else if (fragmentInfo.type == FragmentType::PopNode)
        {
            FW_DEBUG_ASSERT(!offsets.empty());
            FW_DEBUG_ASSERT(!clipRects.empty());
            if (!offsets.empty())
            {
                offsets.pop_back();
            }
            if (!clipRects.empty())
            {
                clipRects.pop_back();
            }
        }
        else if (fragmentInfo.type == FragmentType::DisplayList)
        {
            if (auto const fragment = GetDisplayListFragment(fragmentInfo.index))
            {
                if (fragment->displayList)
                {
                    auto const currentOffset = offsets.empty() ? Offset<Dp>() : offsets.back();
                    auto const currentClipRect = clipRects.empty() ? GetClipRect() : clipRects.back();

                    auto bounds = currentClipRect;
                    auto const displayListBounds = Rect<Dp>::Offset(fragment->displayList->GetBounds(), currentOffset + fragment->displayListOffset);
                    if (displayListBounds.IsFinite())
                    {
                        bounds = Rect<Dp>::Intersect(bounds, displayListBounds);
                    }
                    unionRect = Rect<Dp>::Union(unionRect, bounds);
                }
            }
        }
    });
    FW_DEBUG_ASSERT(unionRect.IsFinite());
    return unionRect;
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

auto PlatformViewLayerVisualWin::OnClipPathChanged() -> void
{
    // TODO: Is there any way to set clip path in DCompositionVisual?
    Invalidate();
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
