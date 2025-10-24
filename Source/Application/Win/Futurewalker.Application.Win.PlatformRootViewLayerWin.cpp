// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformRootViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace
{
auto FindViewLayerById(PlatformViewLayerId const id, Shared<PlatformViewLayerWin> const& layer) -> Shared<PlatformViewLayerWin>
{
    if (layer->GetId() == id)
    {
        return layer;
    }

    for (auto const& child : layer->GetChildren())
    {
        if (auto const found = FindViewLayerById(id, child.As<PlatformViewLayerWin>()))
        {
            return found;
        }
    }
    return {};
}

auto GetVisualIndexByBaseLayerId(PlatformViewLayerId const id, PlatformViewLayerVisualWinArray const& visuals) -> Optional<SInt64>
{
    auto const visualCount = SInt64(std::ssize(visuals));
    for (auto i = SInt64(0); i < visualCount; ++i)
    {
        auto const& visual = visuals[static_cast<size_t>(i)];
        if (visual && visual->GetBaseLayerId() == id)
        {
            if (visual->GetFragmentIndexByLayerId(id))
            {
                return i;
            }
        }
    }
    return {};
}

auto GetRelativeOffset(Shared<PlatformViewLayerWin> const& target, Shared<PlatformViewLayerWin> const& ancestor)  -> Offset<Dp>
{
    auto offset = Offset<Dp>();
    auto current = target;
    while (current && current != ancestor)
    {
        offset += current->GetOffset();
        current = current->GetParent();
    }
    return offset;
}

auto GetRelativeClipRect(Shared<PlatformViewLayerWin> const& target, Shared<PlatformViewLayerWin> const& ancestor) -> Rect<Dp>
{
    auto offset = Offset<Dp>();
    auto clipRect = Rect<Dp>::Infinite();
    auto current = target;
    while (current && current != ancestor)
    {
        if (current->GetClipMode() == ViewClipMode::Bounds)
        {
            clipRect = Rect<Dp>::Intersect(Rect<Dp>({}, current->GetSize()), clipRect);
        }
        clipRect = Rect<Dp>::Offset(clipRect, current->GetOffset());
        offset += current->GetOffset();
        current = current->GetParent();
    }
    return Rect<Dp>::Offset(clipRect, -offset);
}

auto GetRelativeOpacity(Shared<PlatformViewLayerWin> const& target, Shared<PlatformViewLayerWin> const& ancestor) -> Float64
{
    auto opacity = Float64(1.0);
    auto current = target;
    while (current && current != ancestor)
    {
        opacity = opacity * current->GetOpacity();
        current = current->GetParent();
    }
    return opacity;
}

auto ShouldRasterizeLayer(Shared<PlatformViewLayerWin> const& layer) -> Bool
{
    if (layer)
    {
        auto const renderFlags = layer->GetRenderFlags();
        auto const shouldRasterize = (renderFlags & PlatformViewLayerRenderFlags::Rasterize) == PlatformViewLayerRenderFlags::Rasterize;
        return layer->IsRoot() || shouldRasterize;
    }
    return false;
}

auto GetRasterizingBaseLayer(Shared<PlatformViewLayerWin> const& layer) -> Shared<PlatformViewLayerWin>
{
    auto current = layer->GetParent();
    while (current)
    {
        if (ShouldRasterizeLayer(current))
        {
            return current;
        }
        current = current->GetParent();
    }
    return {};
}

auto HasRasterizingLayer(Shared<PlatformViewLayerWin> const& layer) -> Bool
{
    if (ShouldRasterizeLayer(layer))
    {
        return true;
    }

    for (auto const& child : layer->GetChildren())
    {
        if (HasRasterizingLayer(child.As<PlatformViewLayerWin>()))
        {
            return true;
        }
    }
    return false;
}
}

///
/// @brief
///
/// @param hwnd
/// @param dcompDevice
///
auto PlatformRootViewLayerWin::Make(HWND hwnd, Shared<PlatformDCompositionDeviceWin> dcompDevice) -> Shared<PlatformRootViewLayerWin>
{
    return PlatformViewLayer::MakeDerived<PlatformRootViewLayerWin>(hwnd, dcompDevice);
}

///
/// @brief 
///
/// @param key 
/// @param hwnd 
/// @param dcompDevice 
///
PlatformRootViewLayerWin::PlatformRootViewLayerWin(PassKey<PlatformViewLayer> key, HWND hwnd, Shared<PlatformDCompositionDeviceWin> dcompDevice)
  : PlatformViewLayerWin(key, dcompDevice)
  , _hwnd {hwnd}
{
    _target = MakeTarget(_hwnd);
    _visual = dcompDevice->CreateVisual();

    if (_target && _visual)
    {
        _target->SetRoot(_visual.Get());
    }
}

///
/// @brief
///
auto PlatformRootViewLayerWin::GetControl() -> Shared<PlatformViewLayerControl>
{
    return {};
}

auto PlatformRootViewLayerWin::Render() -> void
{
    _visual->RemoveAllVisuals();

    for (auto visual : _visuals)
    {
        if (visual && visual->GetFragmentCount() != 0)
        {
            _visual->AddVisual(visual->GetVisual().Get(), FALSE, nullptr);
        }
    }

    for (auto const& visual : _visuals)
    {
        if (visual && visual->GetFragmentCount() != 0)
        {
            visual->Render();
        }
    }
}

auto PlatformRootViewLayerWin::Initialize() -> void
{
    auto const visual = Shared<PlatformViewLayerVisualWin>::Make(GetCompositionDevice());
    visual->SetBaseLayerId(GetId());
    _visuals = {visual};
    RebuildVisual(GetSelf());
    SetClipMode(ViewClipMode::Bounds);
}

///
/// @brief
///
auto PlatformRootViewLayerWin::RootGetWindowHandle() const -> HWND
{
    return _hwnd;
}

///
/// @brief
///
auto PlatformRootViewLayerWin::RootGetDisplayScale() const -> DisplayScale
{
    if (_hwnd)
    {
        auto const dpi = ::GetDpiForWindow(_hwnd);
        return DisplayScale(dpi) / DisplayScale(USER_DEFAULT_SCREEN_DPI);
    }
    return 1.0;
}

auto PlatformRootViewLayerWin::RootOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    UpdateVisual(layer);
}

auto PlatformRootViewLayerWin::RootSizeChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    UpdateVisual(layer);
}

auto PlatformRootViewLayerWin::RootClipModeChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    UpdateVisual(layer);
}

auto PlatformRootViewLayerWin::RootOpacityChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    UpdateVisual(layer);
}

class PlatformViewLayerVisualUpdater
{
    struct NodeInfo
    {
        PlatformViewLayerId id;
        Offset<Dp> offset;
        Rect<Dp> clipRect;
        Float64 opacity = 1.0;
    };
    PlatformViewLayerVisualWinArray& _visuals;
    Shared<PlatformDCompositionDeviceWin> _dcompDevice;
    SInt64 _visualIndex = 0;
    std::vector<NodeInfo> _nodeStack;
    Shared<PlatformViewLayerVisualWin> _currentVisual;
    std::vector<SInt64> _visualNodeIndexStack;

    auto InternalInsertVisual(PlatformViewLayerId const id, SInt64 const base, SInt64 const target) -> Shared<PlatformViewLayerVisualWin>
    {
        auto const where = _visuals.begin() + static_cast<ptrdiff_t>(_visualIndex);
        auto const visual = Shared<PlatformViewLayerVisualWin>::Make(_dcompDevice);
        visual->SetBaseLayerId(id);
        visual->SetClipRect(GetClipRect(base, target));
        visual->SetOffset(GetOffset(base, target));
        visual->SetOpacity(GetOpacity(base, target));
        _visuals.emplace(where, visual);
        return visual;
    }

    auto InternalGetCurrentVisual() -> Shared<PlatformViewLayerVisualWin>
    {
        return _currentVisual;
    }

    auto InternalSetCurrentVisual(Shared<PlatformViewLayerVisualWin> const& visual) -> void
    {
        _currentVisual = visual;
    }

    auto InternalPushVisualNodeIndex() -> void
    {
        _visualNodeIndexStack.push_back(std::ssize(_nodeStack) - 1);
    }

    auto InternalPopVisualNodeIndex() -> void
    {
        _visualNodeIndexStack.pop_back();
    }

    auto GetOffset(SInt64 const base, SInt64 const target) const -> Offset<Dp>
    {
        auto result = Offset<Dp>();
        for (auto i = base + 1; i <= target; ++i)
        {
            result = result + _nodeStack[static_cast<size_t>(i)].offset;
        }
        return result;
    }

    auto GetClipRect(SInt64 const base, SInt64 const target) const -> Rect<Dp>
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

    auto GetOpacity(SInt64 const base, SInt64 const target) const -> Float64
    {
        auto result = Float64(1.0);
        for (auto i = base + 1; i <= target; ++i)
        {
            result = result * _nodeStack[static_cast<size_t>(i)].opacity;
        }
        return result;
    }

    auto GetBaseNodeIndex() const -> SInt64
    {
        if (!_visualNodeIndexStack.empty())
        {
            return _visualNodeIndexStack.back();
        }
        return 0;
    }

    auto GetCurrentNodeIndex() const -> SInt64
    {
        return static_cast<SInt64>(std::ssize(_nodeStack)) - 1;
    }

    auto GetVisualCount() const -> SInt64
    {
        return static_cast<SInt64>(std::ssize(_visuals));
    }

    auto GetVisualAt(SInt64 const index) -> Shared<PlatformViewLayerVisualWin>
    {
        if (index >= 0 && index < GetVisualCount())
        {
            return _visuals[static_cast<size_t>(index)];
        }
        return {};
    }

    auto RemoveUntilNextVisual(SInt64 const index, PlatformViewLayerId const id) -> Shared<PlatformViewLayerVisualWin>
    {
        auto const visualCount = GetVisualCount();
        if (0 <= index && index < visualCount)
        {
            auto found = visualCount;
            for (auto i = index; i < visualCount; ++i)
            {
                if (GetVisualAt(i)->GetBaseLayerId() == id)
                {
                    found = i;
                    break;
                }
            }

            if (found < visualCount)
            {
                auto const visual = GetVisualAt(found);
                _visuals.erase(_visuals.begin() + static_cast<ptrdiff_t>(index), _visuals.begin() + static_cast<ptrdiff_t>(found));
                return visual;
            }
        }
        return {};
    }

public:
    PlatformViewLayerVisualUpdater(PlatformViewLayerVisualWinArray& visuals, Shared<PlatformDCompositionDeviceWin> const& dcompDevice, SInt64 const visualIndex)
      : _visuals {visuals}
      , _dcompDevice {dcompDevice}
      , _visualIndex {visualIndex}
    {
    }

    auto PushVisual(PlatformViewLayerId const id) -> void
    {
        if (auto const nextVisual = RemoveUntilNextVisual(_visualIndex++, id))
        {
            if (_visualNodeIndexStack.empty())
            {
                nextVisual->ClearFragments();
                InternalSetCurrentVisual(nextVisual);
                InternalPushVisualNodeIndex();
            }
            else
            {
                InternalSetCurrentVisual(nullptr);
            }
            return;
        }
        FW_DEBUG_ASSERT(!_nodeStack.empty());
        InternalSetCurrentVisual(InternalInsertVisual(id, 0, GetCurrentNodeIndex()));
        InternalPushVisualNodeIndex();
    }

    auto PopVisual() -> void
    {
        InternalPopVisualNodeIndex();
        InternalSetCurrentVisual(nullptr);
    }

    auto PushNode(PlatformViewLayerId const& id, Offset<Dp> const& offset, Rect<Dp> const& clipRect, Float64 const& opacity) -> void
    {
        _nodeStack.push_back({
            .id = id,
            .offset = offset,
            .clipRect = clipRect,
            .opacity = opacity,
        });
    }

    auto PopNode() -> void
    {
        FW_DEBUG_ASSERT(!_nodeStack.empty());
        _nodeStack.pop_back();
    }

    auto AddFragment(PlatformViewLayerId const id, Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& displayListOffset) -> void
    {
        auto visual = InternalGetCurrentVisual();
        if (!visual)
        {
            // When one of child nodes has its own visual, rest of siblings needs another visual for them.
            visual = InternalInsertVisual(id, 0, GetBaseNodeIndex());
            InternalSetCurrentVisual(visual);
            InternalPushVisualNodeIndex();
        }

        // Append fragment.
        auto const baseIndex = GetBaseNodeIndex();
        visual->InsertFragment(
          visual->GetFragmentCount(),
          id,
          {
              .offset = GetOffset(baseIndex, GetCurrentNodeIndex()),
              .clipRect = GetClipRect(baseIndex, GetCurrentNodeIndex()),
              .opacity = GetOpacity(baseIndex, GetCurrentNodeIndex()),
              .displayList = displayList,
              .displayListOffset = displayListOffset,
          });
    }
};

auto PlatformRootViewLayerWin::RootRenderFlagsChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    // Find closest base layer.
    if (auto const baseLayer = GetRasterizingBaseLayer(layer))
    {
        RebuildVisual(baseLayer);
    }
    else
    {
        FW_DEBUG_ASSERT(false);
    }
}

auto PlatformRootViewLayerWin::RootDisplayListChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    UpdateVisual(layer);
}

auto PlatformRootViewLayerWin::RootDisplayListOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    UpdateVisual(layer);
}

auto PlatformRootViewLayerWin::RootChildAdded(Shared<PlatformViewLayerWin> const& child) -> void
{
    if (auto const baseLayer = GetRasterizingBaseLayer(child))
    {
        RebuildVisual(baseLayer);
    }
    else
    {
        FW_DEBUG_ASSERT(false);
    }
}

auto PlatformRootViewLayerWin::RootChildRemoved(Shared<PlatformViewLayerWin> const& parent) -> void
{
    auto baseLayer = parent;
    while (baseLayer)
    {
        if (ShouldRasterizeLayer(baseLayer))
        {
            break;
        }
        baseLayer = baseLayer->GetParent();
    }
    RebuildVisual(baseLayer);
}

///
/// @brief
///
/// @param hwnd
///
auto PlatformRootViewLayerWin::MakeTarget(HWND hwnd) -> Microsoft::WRL::ComPtr<IDCompositionTarget>
{
    if (const auto dcompDevice = GetCompositionDevice())
    {
        return dcompDevice->CreateTargetForHwnd(hwnd);
    }
    return {};
}

auto PlatformRootViewLayerWin::FindVisualByBaseLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisualWin>
{
    for (auto const& visual : _visuals)
    {
        if (!visual)
        {
            FW_DEBUG_ASSERT(false);
            continue;
        }

        if (visual->GetBaseLayerId() == layerId)
        {
            return visual;
        }
    }
    return {};
}

auto PlatformRootViewLayerWin::FindVisualByLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisualWin>
{
    for (auto const& visual : _visuals)
    {
        if (!visual)
        {
            FW_DEBUG_ASSERT(false);
            continue;
        }

        if (visual->GetFragmentIndexByLayerId(layerId))
        {
            return visual;
        }
    }
    return {};
}


auto PlatformRootViewLayerWin::RebuildVisual(Shared<PlatformViewLayerWin> const& baseLayer) -> void
{
    FW_DEBUG_LOG_INFO("Rebuilding visuals starting from layer ID {}", (uint64_t)(UInt64)baseLayer->GetId());

    // find index of visual corresponding to base layer.
    auto const visualIndex = GetVisualIndexByBaseLayerId(baseLayer->GetId(), _visuals);

    // rebuild with builder.
    auto builder = PlatformViewLayerVisualUpdater(_visuals, GetCompositionDevice(), visualIndex.GetValueOr(0));
    RebuildVisualCore(builder, baseLayer);
}

auto PlatformRootViewLayerWin::RebuildVisualCore(auto& builder, Shared<PlatformViewLayerWin> const& layer) -> void
{
    auto const id = layer->GetId();
    auto const offset = layer->GetOffset();
    auto const opacity = layer->GetOpacity();
    auto const clipMode = layer->GetClipMode() == ViewClipMode::Bounds ? Rect<Dp>({}, layer->GetSize()) : Rect<Dp>::Infinite();
    builder.PushNode(id, offset, clipMode, opacity);

    auto const needsSurface = (layer->IsRoot() || ShouldRasterizeLayer(layer));
    if (needsSurface)
    {
        builder.PushVisual(id);
    }

    auto const displayList = layer->GetDisplayList();
    auto const displayListOffset = layer->GetDisplayListOffset();
    builder.AddFragment(id, displayList, displayListOffset);

    for (auto const& child : layer->GetChildren())
    {
        RebuildVisualCore(builder, child.As<PlatformViewLayerWin>());
    }

    builder.PopNode();

    if (needsSurface)
    {
        builder.PopVisual();
    }
}

auto PlatformRootViewLayerWin::UpdateVisual(Shared<PlatformViewLayerWin> const& layer) -> void
{
    UpdateVisualCore(layer, {});
}

auto PlatformRootViewLayerWin::UpdateVisualCore(Shared<PlatformViewLayerWin> layer, Shared<PlatformViewLayerVisualWin> currentVisual) -> void
{
    if (!layer)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    auto const layerId = layer->GetId();

    if (ShouldRasterizeLayer(layer))
    {
        if (auto const visual = FindVisualByBaseLayerId(layerId))
        {
            visual->SetOffset(GetRelativeOffset(layer, GetSelf()));
            visual->SetClipRect(GetRelativeClipRect(layer, GetSelf()));
            visual->SetOpacity(GetRelativeOpacity(layer, GetSelf()));
        }
        else
        {
            FW_DEBUG_ASSERT(false);
        }
    }
    else
    {
        if (!currentVisual)
        {
            currentVisual = FindVisualByLayerId(layer->GetId());
            if (!currentVisual)
            {
                FW_DEBUG_ASSERT(currentVisual);
                return;
            }
        }

        if (auto const index = currentVisual->GetFragmentIndexByLayerId(layerId))
        {
            auto const baseLayer = FindViewLayerById(currentVisual->GetBaseLayerId(), GetSelf());
            currentVisual->ReplaceFragment(
              *index,
              layerId,
              {
                  .offset = GetRelativeOffset(layer, baseLayer),
                  .clipRect = GetRelativeClipRect(layer, baseLayer),
                  .opacity = GetRelativeOpacity(layer, baseLayer),
                  .displayList = layer->GetDisplayList(),
                  .displayListOffset = layer->GetDisplayListOffset(),
              });

            for (auto const& child : layer->GetChildren())
            {
                UpdateVisualCore(child.As<PlatformViewLayerWin>(), currentVisual);
            }
        }
    }
}
}
