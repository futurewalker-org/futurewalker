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

auto PlatformViewLayerVisual::GetOffset() const -> Vector2<Dp>
{
    return _offset;
}

auto PlatformViewLayerVisual::SetOffset(Vector2<Dp> const& offset) -> void
{
    if (_offset != offset)
    {
        _offset = offset;
        OnOffsetChanged();
    }
}

auto PlatformViewLayerVisual::GetTransform() const -> Matrix3x3<Dp>
{
    return _transform;
}

auto PlatformViewLayerVisual::SetTransform(Matrix3x3<Dp> const& transform) -> void
{
    if (_transform != transform)
    {
        _transform = transform;
        OnTransformChanged();
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

auto PlatformViewLayerVisual::SetClipPaths(std::vector<Graphics::Path> clipPaths) -> void
{
    if (_clipPaths != clipPaths)
    {
        _clipPaths = std::move(clipPaths);
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

auto PlatformViewLayerVisual::GetRenderParams() const -> PlatformViewLayerVisualRenderParams const&
{
    return _renderParams;
}

auto PlatformViewLayerVisual::SetRenderParams(PlatformViewLayerVisualRenderParams const& renderParams) -> void
{
    if (_renderParams != renderParams)
    {
        _renderParams = renderParams;
        OnRenderParamsChanged();
    }
}

auto PlatformViewLayerVisual::AddDisplayListFragment(PlatformViewLayerId layerId, DisplayListFragment&& fragment) -> void
{
    _fragments.push_back(
      FragmentInfo {
          .layerId = layerId,
          .type = FragmentType::DisplayList,
          .index = SInt32(std::ssize(_displayListFragments)),
      });
    _displayListFragments.push_back(std::move(fragment));
    OnFragmentChanged();
}

auto PlatformViewLayerVisual::AddPushNodeFragment(PlatformViewLayerId layerId, PushNodeFragment&& fragment) -> void
{
    _fragments.push_back(
      FragmentInfo {
          .layerId = layerId,
          .type = FragmentType::PushNode,
          .index = SInt32(std::ssize(_pushNodeFragments)),
      });
    _pushNodeFragments.push_back(std::move(fragment));
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

auto PlatformViewLayerVisual::ReplaceDisplayListFragment(SInt32 const index, DisplayListFragment&& fragment) -> void
{
    if (0 <= index && index < SInt32(std::ssize(_fragments)))
    {
        auto const& fragmentInfo = _fragments[static_cast<size_t>(index)];
        if (fragmentInfo.type == FragmentType::DisplayList)
        {
            auto& existingFragment = _displayListFragments[static_cast<size_t>(fragmentInfo.index)];
            if (existingFragment != fragment)
            {
                existingFragment = std::move(fragment);
                OnFragmentChanged();
            }
            return;
        }
    }
    FW_DEBUG_ASSERT(false);
}

auto PlatformViewLayerVisual::ReplacePushNodeFragment(SInt32 const index, PushNodeFragment&& fragment) -> void
{
    if (0 <= index && index < SInt32(std::ssize(_fragments)))
    {
        auto const& fragmentInfo = _fragments[static_cast<size_t>(index)];
        if (fragmentInfo.type == FragmentType::PushNode)
        {
            auto& existingFragment = _pushNodeFragments[static_cast<size_t>(fragmentInfo.index)];
            if (existingFragment != fragment)
            {
                existingFragment = std::move(fragment);
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
    static constexpr auto transformClipRect = [](Rect<Dp> const& rect, Matrix3x3<Dp> const& transform) -> Rect<Dp> {
        if (rect.IsFinite())
        {
            auto const topLeft = transform * Vector3<Dp>(rect.x0, rect.y0, 1.0);
            auto const topRight = transform * Vector3<Dp>(rect.x1, rect.y0, 1.0);
            auto const bottomLeft = transform * Vector3<Dp>(rect.x0, rect.y1, 1.0);
            auto const bottomRight = transform * Vector3<Dp>(rect.x1, rect.y1, 1.0);
            auto const minX = std::min({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
            auto const maxX = std::max({topLeft.x, topRight.x, bottomLeft.x, bottomRight.x});
            auto const minY = std::min({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
            auto const maxY = std::max({topLeft.y, topRight.y, bottomLeft.y, bottomRight.y});
            return Rect<Dp>(minX, minY, maxX, maxY);
        }
        return Rect<Dp>::Infinite();
    };

    auto transforms = std::vector<Matrix3x3<Dp>>();
    auto clipRects = std::vector<Rect<Dp>>();

    transforms.push_back(Matrix3x3<Dp>::MakeIdentity());
    clipRects.push_back(GetClipRect());

    auto unionRect = Rect<Dp>();
    ForEachFragment([&](auto const& fragmentInfo) {
        if (fragmentInfo.type == FragmentType::PushNode)
        {
            if (auto const fragment = GetPushNodeFragment(fragmentInfo.index))
            {
                auto const currentTransform = transforms.empty() ? Matrix3x3<Dp>::MakeIdentity() : transforms.back();
                auto const currentClipRect = clipRects.empty() ? GetClipRect() : clipRects.back();
                transforms.push_back(currentTransform * Matrix3x3<Dp>::MakeTranslation(fragment->offset) * fragment->transform);
                clipRects.push_back(Rect<Dp>::Intersect(currentClipRect, transformClipRect(fragment->clipRect, transforms.back())));
            }
        }
        else if (fragmentInfo.type == FragmentType::PopNode)
        {
            FW_DEBUG_ASSERT(!transforms.empty());
            FW_DEBUG_ASSERT(!clipRects.empty());
            transforms.pop_back();
            clipRects.pop_back();
        }
        else if (fragmentInfo.type == FragmentType::DisplayList)
        {
            if (auto const fragment = GetDisplayListFragment(fragmentInfo.index))
            {
                if (fragment->displayList)
                {
                    auto const currentTransform = transforms.empty() ? Matrix3x3<Dp>::MakeIdentity() : transforms.back();
                    auto const currentClipRect = clipRects.empty() ? GetClipRect() : clipRects.back();

                    auto bounds = currentClipRect;
                    auto const localDisplayListBounds = Rect<Dp>::Offset(fragment->displayList->GetBounds(), fragment->displayListOffset);
                    auto const transformedDisplayListBounds = transformClipRect(localDisplayListBounds, currentTransform);
                    if (transformedDisplayListBounds.IsFinite())
                    {
                        bounds = Rect<Dp>::Intersect(bounds, transformedDisplayListBounds);
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
