// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformViewLayerVisualUpdaterWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
PlatformViewLayerVisualUpdaterWin::PlatformViewLayerVisualUpdaterWin(Shared<PlatformDCompositionDeviceWin> const& dcompDevice)
  : _dcompDevice {dcompDevice}
{
}

auto PlatformViewLayerVisualUpdaterWin::Update(Shared<PlatformViewLayerVisualWin> const& visual, Shared<PlatformViewLayerWin> const& layer) -> void
{
    _nodeStack.clear();
    _visualNodeIndexStack.clear();
    _currentVisual = visual;
    UpdateCore(layer);
}

auto PlatformViewLayerVisualUpdaterWin::PushVisual(PlatformViewLayerId const id) -> Bool
{
    if (auto const nextVisual = InternalRemoveUntilNextVisual(id))
    {
        if (_visualNodeIndexStack.empty())
        {
            nextVisual->ClearFragments();
            InternalSetCurrentVisual(nextVisual);
            InternalPushVisualNodeIndex();
            return true;
        }
        InternalSetCurrentVisual(nullptr);
        return false;
    }
    FW_DEBUG_ASSERT(!_nodeStack.empty());
    InternalSetCurrentVisual(InternalInsertVisual(id, 0, InternalGetCurrentNodeIndex()));
    InternalPushVisualNodeIndex();
    return true;
}

auto PlatformViewLayerVisualUpdaterWin::PopVisual() -> void
{
    InternalPopVisualNodeIndex();
    InternalSetCurrentVisual(nullptr);
}

auto PlatformViewLayerVisualUpdaterWin::PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Float64 const& opacity) -> void
{
    _nodeStack.push_back({
        .id = id,
        .offset = offset,
        .clipRect = clipRect,
        .opacity = opacity,
    });
}

auto PlatformViewLayerVisualUpdaterWin::PopNode() -> void
{
    FW_DEBUG_ASSERT(!_nodeStack.empty());
    _nodeStack.pop_back();
}

auto PlatformViewLayerVisualUpdaterWin::AddFragment(PlatformViewLayerId const id, Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& displayListOffset) -> void
{
    auto visual = InternalGetCurrentVisual();
    if (!visual)
    {
        // When one of child nodes has its own visual, rest of siblings needs another visual for them.
        visual = InternalInsertVisual(id, 0, InternalGetBaseNodeIndex());
        InternalSetCurrentVisual(visual);
        InternalPushVisualNodeIndex();
    }

    // Append fragment.
    auto const baseIndex = InternalGetBaseNodeIndex();
    visual->InsertFragment(
      visual->GetFragmentCount(),
      id,
      {
          .offset = InternalGetOffset(baseIndex, InternalGetCurrentNodeIndex()),
          .clipRect = InternalGetClipRect(baseIndex, InternalGetCurrentNodeIndex()),
          .opacity = InternalGetOpacity(baseIndex, InternalGetCurrentNodeIndex()),
          .displayList = displayList,
          .displayListOffset = displayListOffset,
      });
}

auto PlatformViewLayerVisualUpdaterWin::UpdateCore(Shared<PlatformViewLayerWin> const& layer) -> void
{
    auto const id = layer->GetId();
    auto const offset = layer->GetOffset();
    auto const opacity = layer->GetOpacity();
    auto const clipMode = layer->GetClipMode() == ViewClipMode::Bounds ? Rect<Dp>({}, layer->GetSize()) : Rect<Dp>::Infinite();
    PushNode(id, offset, clipMode, opacity);

    auto const needsSurface = layer->ShouldRasterize();
    if (needsSurface)
    {
        if (!PushVisual(id))
        {
            PopNode();
            return;
        }
    }

    auto const displayList = layer->GetDisplayList();
    auto const displayListOffset = layer->GetDisplayListOffset();
    AddFragment(id, displayList, displayListOffset);

    for (auto const& child : layer->GetChildren())
    {
        UpdateCore(child.As<PlatformViewLayerWin>());
    }

    PopNode();

    if (needsSurface)
    {
        PopVisual();
    }
}

auto PlatformViewLayerVisualUpdaterWin::InternalInsertVisual(PlatformViewLayerId const id, SInt64 const base, SInt64 const target) -> Shared<PlatformViewLayerVisualWin>
{
    auto const visual = Shared<PlatformViewLayerVisualWin>::Make(_dcompDevice);
    visual->SetBaseLayerId(id);
    visual->SetClipRect(InternalGetClipRect(base, target));
    visual->SetOffset(InternalGetOffset(base, target));
    visual->SetOpacity(InternalGetOpacity(base, target));
    if (auto const currentVisual = InternalGetCurrentVisual())
    {
        currentVisual->AddChild(visual);
        return visual;
    }
    FW_DEBUG_ASSERT(false);
    return {};
}

auto PlatformViewLayerVisualUpdaterWin::InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisualWin>
{
    return _currentVisual;
}

auto PlatformViewLayerVisualUpdaterWin::InternalSetCurrentVisual(Shared<PlatformViewLayerVisualWin> const& visual) -> void
{
    _currentVisual = visual;
}

auto PlatformViewLayerVisualUpdaterWin::InternalPushVisualNodeIndex() -> void
{
    _visualNodeIndexStack.push_back(std::ssize(_nodeStack) - 1);
}

auto PlatformViewLayerVisualUpdaterWin::InternalPopVisualNodeIndex() -> void
{
    _visualNodeIndexStack.pop_back();
}

auto PlatformViewLayerVisualUpdaterWin::InternalGetOffset(SInt64 const base, SInt64 const target) const -> Offset<Dp>
{
    auto result = Offset<Dp>();
    for (auto i = base + 1; i <= target; ++i)
    {
        result = result + _nodeStack[static_cast<size_t>(i)].offset;
    }
    return result;
}

auto PlatformViewLayerVisualUpdaterWin::InternalGetClipRect(SInt64 const base, SInt64 const target) const -> Rect<Dp>
{
    auto offset = Offset<Dp>();
    auto clipRect = Rect<Dp>::Infinite();
    for (auto i = base + 1; i <= target; ++i)
    {
        auto const& node = _nodeStack[static_cast<size_t>(i)];
        offset = offset + node.offset;
        clipRect = Rect<Dp>::Intersect(clipRect, Rect<Dp>::Offset(node.clipRect, offset));
    }
    return Rect<Dp>::Offset(clipRect, -offset);
}

auto PlatformViewLayerVisualUpdaterWin::InternalGetOpacity(SInt64 const base, SInt64 const target) const -> Float64
{
    auto result = Float64(1.0);
    for (auto i = base + 1; i <= target; ++i)
    {
        result = result * _nodeStack[static_cast<size_t>(i)].opacity;
    }
    return result;
}

auto PlatformViewLayerVisualUpdaterWin::InternalGetBaseNodeIndex() const -> SInt64
{
    if (!_visualNodeIndexStack.empty())
    {
        return _visualNodeIndexStack.back();
    }
    return 0;
}

auto PlatformViewLayerVisualUpdaterWin::InternalGetCurrentNodeIndex() const -> SInt64
{
    return static_cast<SInt64>(std::ssize(_nodeStack)) - 1;
}

auto PlatformViewLayerVisualUpdaterWin::InternalRemoveUntilNextVisual(PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisualWin>
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
