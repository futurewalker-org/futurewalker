// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"

#include "Futurewalker.Graphics.DisplayList.hpp"

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

auto PlatformViewLayerVisual::GetOffset() const -> Vector<Dp>
{
    return _offset;
}

auto PlatformViewLayerVisual::SetOffset(Vector<Dp> const& offset) -> void
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
          .index = SInt32(std::ssize(_displayListFragments)),
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
          .index = SInt32(std::ssize(_pushNodeFragments)),
      });
    _pushNodeFragments.push_back(fragment);
    _pushNodeIndexStack.push_back(SInt32(std::ssize(_pushNodeFragments) - 1));
    OnFragmentChanged();
}

auto PlatformViewLayerVisual::AddPopNodeFragment(PlatformViewLayerId layerId) -> void
{
    _fragments.push_back(
      FragmentInfo {
          .layerId = layerId,
          .type = FragmentType::PopNode,
          .index = SInt32(std::ssize(_popNodeFragments)),
      });

    if (_pushNodeIndexStack.empty())
    {
        FW_DEBUG_ASSERT(false);
        _popNodeFragments.push_back(
          PopNodeFragment {
              .pushNodeIndex = 0,
          });
    }
    else
    {
        _popNodeFragments.push_back(
          PopNodeFragment {
              .pushNodeIndex = _pushNodeIndexStack.back(),
          });
        _pushNodeIndexStack.pop_back();
    }
    OnFragmentChanged();
}

auto PlatformViewLayerVisual::ReplaceDisplayListFragment(SInt32 const index, DisplayListFragment const& fragment) -> void
{
    if (0 <= index && index < SInt32(std::ssize(_fragments)))
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

auto PlatformViewLayerVisual::ReplacePushNodeFragment(SInt32 const index, PushNodeFragment const& fragment) -> void
{
    if (0 <= index && index < SInt32(std::ssize(_fragments)))
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
        _pushNodeIndexStack.clear();
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

auto PlatformViewLayerVisual::GetDisplayListFragment(SInt32 const index) const -> Pointer<DisplayListFragment const>
{
    if (0 <= index && index < SInt32(std::ssize(_displayListFragments)))
    {
        return Pointer<DisplayListFragment const>(&_displayListFragments[static_cast<size_t>(index)]);
    }
    FW_DEBUG_ASSERT(false);
    return nullptr;
}

auto PlatformViewLayerVisual::GetPushNodeFragment(SInt32 const index) const -> Pointer<PushNodeFragment const>
{
    if (0 <= index && index < SInt32(std::ssize(_pushNodeFragments)))
    {
        return Pointer<PushNodeFragment const>(&_pushNodeFragments[static_cast<size_t>(index)]);
    }
    FW_DEBUG_ASSERT(false);
    return nullptr;
}

auto PlatformViewLayerVisual::GetPopNodeFragment(SInt32 const index) const -> Pointer<PopNodeFragment const>
{
    if (0 <= index && index < SInt32(std::ssize(_popNodeFragments)))
    {
        return Pointer<PopNodeFragment const>(&_popNodeFragments[static_cast<size_t>(index)]);
    }
    FW_DEBUG_ASSERT(false);
    return nullptr;
}

auto PlatformViewLayerVisual::GetFragmentCount() const -> SInt32
{
    return SInt32(std::ssize(_fragments));
}

auto PlatformViewLayerVisual::CalcFragmentBounds() const -> Rect<Dp>
{
    auto offsets = std::vector<Vector<Dp>>();
    auto clipRects = std::vector<Rect<Dp>>();

    offsets.push_back(GetOffset());
    clipRects.push_back(GetClipRect());

    auto unionRect = Rect<Dp>();
    ForEachFragment([&](auto const& fragmentInfo) {
        if (fragmentInfo.type == FragmentType::PushNode)
        {
            if (auto const fragment = GetPushNodeFragment(fragmentInfo.index))
            {
                auto const currentOffset = offsets.empty() ? Vector<Dp>() : offsets.back();
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
                    auto const currentOffset = offsets.empty() ? Vector<Dp>() : offsets.back();
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

auto PlatformViewLayerVisual::Initialize() -> void
{
}
}
