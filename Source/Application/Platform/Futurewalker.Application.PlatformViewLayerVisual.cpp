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

auto PlatformViewLayerVisual::GetClipPaths() const -> std::vector<Graphics::Path> const&
{
    return _clipPaths;
}

auto PlatformViewLayerVisual::SetClipPaths(std::vector<Graphics::Path> const& clipPaths) -> void
{
    if (_clipPaths != clipPaths)
    {
        _clipPaths = clipPaths;
        OnClipPathChanged();
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

auto PlatformViewLayerVisual::AddDisplayListFragment(PlatformViewLayerId layerId, DisplayListFragment const& fragment) -> void
{
    _fragments.push_back(
      FragmentInfo {
          .layerId = layerId,
          .type = FragmentType::DisplayList,
          .index = std::ssize(_displayListFragments),
      });
    _displayListFragments.push_back(fragment);
    OnFragmentChanged();
}

auto PlatformViewLayerVisual::AddPushNodeFragment(PlatformViewLayerId layerId, PushNodeFragment const& fragment) -> void
{
    _fragments.push_back(
      FragmentInfo {
          .layerId = layerId,
          .type = FragmentType::PushNode,
          .index = std::ssize(_pushNodeFragments),
      });
    _pushNodeFragments.push_back(fragment);
    OnFragmentChanged();
}

auto PlatformViewLayerVisual::AddPopNodeFragment(PlatformViewLayerId layerId) -> void
{
    _fragments.push_back(
      FragmentInfo {
          .layerId = layerId,
          .type = FragmentType::PopNode,
          .index = 0,
      });
    _popNodeFragments.push_back(
      PopNodeFragment {
          .pushNodeIndex = std::ssize(_pushNodeFragments) - 1,
      });
    OnFragmentChanged();
}

auto PlatformViewLayerVisual::ReplaceDisplayListFragment(SInt64 const index, DisplayListFragment const& fragment) -> void
{
    if (0 <= index && index < std::ssize(_fragments))
    {
        auto const& fragmentInfo = _fragments[static_cast<size_t>(index)];
        if (fragmentInfo.type == FragmentType::DisplayList)
        {
            auto& existingFragment = _displayListFragments[static_cast<size_t>(fragmentInfo.index)];
            if (existingFragment != fragment)
            {
                existingFragment = fragment;
                OnFragmentChanged();
            }
            return;
        }
    }
    FW_DEBUG_ASSERT(false);
}

auto PlatformViewLayerVisual::ReplacePushNodeFragment(SInt64 const index, PushNodeFragment const& fragment) -> void
{
    if (0 <= index && index < std::ssize(_fragments))
    {
        auto const& fragmentInfo = _fragments[static_cast<size_t>(index)];
        if (fragmentInfo.type == FragmentType::PushNode)
        {
            auto& existingFragment = _pushNodeFragments[static_cast<size_t>(fragmentInfo.index)];
            if (existingFragment != fragment)
            {
                existingFragment = fragment;
                OnFragmentChanged();
            }
            return;
        }
    }
    FW_DEBUG_ASSERT(false);
}

auto PlatformViewLayerVisual::ClearFragments() -> void
{
    if (!_fragments.empty())
    {
        _fragments.clear();
        _displayListFragments.clear();
        _pushNodeFragments.clear();
        _popNodeFragments.clear();
        OnFragmentChanged();
    }
}

auto PlatformViewLayerVisual::ForEachFragment(Function<void(FragmentInfo const&)> const& func) const -> void
{
    for (auto const& fragment : _fragments)
    {
        if (func)
        {
            func(fragment);
        }
    }
}

auto PlatformViewLayerVisual::GetDisplayListFragment(SInt64 const index) const -> Pointer<DisplayListFragment const>
{
    if (0 <= index && index < std::ssize(_displayListFragments))
    {
        return Pointer<DisplayListFragment const>(&_displayListFragments[static_cast<size_t>(index)]);
    }
    FW_DEBUG_ASSERT(false);
    return nullptr;
}

auto PlatformViewLayerVisual::GetPushNodeFragment(SInt64 const index) const -> Pointer<PushNodeFragment const>
{
    if (0 <= index && index < std::ssize(_pushNodeFragments))
    {
        return Pointer<PushNodeFragment const>(&_pushNodeFragments[static_cast<size_t>(index)]);
    }
    FW_DEBUG_ASSERT(false);
    return nullptr;
}

auto PlatformViewLayerVisual::GetPopNodeFragment(SInt64 const index) const -> Pointer<PopNodeFragment const>
{
    if (0 <= index && index < std::ssize(_popNodeFragments))
    {
        return Pointer<PopNodeFragment const>(&_popNodeFragments[static_cast<size_t>(index)]);
    }
    FW_DEBUG_ASSERT(false);
    return nullptr;
}

auto PlatformViewLayerVisual::GetFragmentCount() const -> SInt64
{
    return std::ssize(_fragments);
}

auto PlatformViewLayerVisual::Initialize() -> void
{
}
}
