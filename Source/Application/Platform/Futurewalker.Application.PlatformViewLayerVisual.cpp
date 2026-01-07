// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
PlatformViewLayerVisual::PlatformViewLayerVisual(PassKey<PlatformViewLayerVisual>)
{
}

PlatformViewLayerVisual::~PlatformViewLayerVisual() = default;

auto PlatformViewLayerVisual::AddChild(Shared<PlatformViewLayerVisual> const& child) -> void
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

auto PlatformViewLayerVisual::RemoveChild(Shared<PlatformViewLayerVisual> const& child) -> void
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

auto PlatformViewLayerVisual::GetChildren() -> std::vector<Shared<PlatformViewLayerVisual>> const&
{
    return _children;
}

auto PlatformViewLayerVisual::GetParent() -> Shared<PlatformViewLayerVisual>
{
    return _parent.Lock();
}

auto PlatformViewLayerVisual::GetBaseLayerId() const -> PlatformViewLayerId
{
    return _baseLayerId;
}

auto PlatformViewLayerVisual::SetBaseLayerId(PlatformViewLayerId const layerId) -> void
{
    _baseLayerId = layerId;
}

auto PlatformViewLayerVisual::GetOffset() const -> Offset<Dp>
{
    return _offset;
}

auto PlatformViewLayerVisual::SetOffset(Offset<Dp> const& offset) -> void
{
    if (_offset != offset)
    {
        _offset = offset;
        OnOffsetChanged();
    }
}

auto PlatformViewLayerVisual::GetClipRect() const -> Rect<Dp>
{
    return _clipRect;
}

auto PlatformViewLayerVisual::SetClipRect(Rect<Dp> const& clipRect) -> void
{
    if (!clipRect.IsFinite())
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_clipRect != clipRect)
    {
        _clipRect = clipRect;
        OnClipRectChanged();
    }
}

auto PlatformViewLayerVisual::GetOpacity() const -> Float64
{
    return _opacity;
}

auto PlatformViewLayerVisual::SetOpacity(Float64 const opacity) -> void
{
    if (!Float64::IsFinite(opacity))
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_opacity != opacity)
    {
        _opacity = opacity;
        OnOpacityChanged();
    }
}

auto PlatformViewLayerVisual::GetDisplayScale() const -> DisplayScale
{
    return _displayScale;
}

auto PlatformViewLayerVisual::SetDisplayScale(DisplayScale const displayScale) -> void
{
    if (!DisplayScale::IsFinite(displayScale))
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_displayScale != displayScale)
    {
        _displayScale = displayScale;
        OnDisplayScaleChanged();
    }
}

auto PlatformViewLayerVisual::GetBackingScale() const -> BackingScale
{
    return _backingScale;
}

auto PlatformViewLayerVisual::SetBackingScale(BackingScale const backingScale) -> void
{
    if (!BackingScale::IsFinite(backingScale))
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_backingScale != backingScale)
    {
        _backingScale = backingScale;
        OnBackingScaleChanged();
    }
}

auto PlatformViewLayerVisual::InsertFragment(const SInt64 index, PlatformViewLayerId layerId, Fragment const& fragment) -> void
{
    if (0 <= index && index <= GetFragmentCount())
    {
        if (_fragments.Insert(index, layerId, fragment))
        {
            OnFragmentChanged();
        }
    }
}

auto PlatformViewLayerVisual::ReplaceFragment(const SInt64 index, PlatformViewLayerId layerId, Fragment const& fragment) -> void
{
    if (0 <= index && index <= GetFragmentCount())
    {
        if (_fragments.Replace(index, layerId, fragment))
        {
            OnFragmentChanged();
        }
    }
}

auto PlatformViewLayerVisual::ReplaceFragment(PlatformViewLayerId layerId, Fragment const& fragment) -> void
{
    if (auto const idx = _fragments.Find(layerId))
    {
        ReplaceFragment(*idx, layerId, fragment);
    }
}

auto PlatformViewLayerVisual::RemoveFragment(const SInt64 index) -> void
{
    if (0 <= index && index < GetFragmentCount())
    {
        if (_fragments.Erase(index))
        {
            OnFragmentChanged();
        }
    }
}
auto PlatformViewLayerVisual::ClearFragments() -> void
{
    if (!_fragments.IsEmpty())
    {
        _fragments.Clear();
        OnFragmentChanged();
    }
}

auto PlatformViewLayerVisual::GetFragmentIndexByLayerId(PlatformViewLayerId const layerId) const -> Optional<SInt64>
{
    return _fragments.Find(layerId);
}

auto PlatformViewLayerVisual::GetFragment(SInt64 const index) const -> Optional<Fragment>
{
    if (0 <= index && index < GetFragmentCount())
    {
        return _fragments.GetValue(index);
    }
    return {};
}

auto PlatformViewLayerVisual::GetFragmentCount() const -> SInt64
{
    return _fragments.GetSize();
}

auto PlatformViewLayerVisual::Initialize() -> void
{
}
}
