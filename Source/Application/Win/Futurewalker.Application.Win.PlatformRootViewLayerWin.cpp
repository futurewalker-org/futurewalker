// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformRootViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualPropertyUpdaterWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualUpdaterWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include <chrono>

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

auto GetRasterizingLayer(Shared<PlatformViewLayerWin> const& layer) -> Shared<PlatformViewLayerWin>
{
    auto current = layer;
    while (current)
    {
        if (current->ShouldRasterize())
        {
            return current;
        }
        current = current->GetParent();
    }
    return {};
}

auto GetRasterizingBaseLayer(Shared<PlatformViewLayerWin> const& layer) -> Shared<PlatformViewLayerWin>
{
    if (layer)
    {
        return GetRasterizingLayer(layer->GetParent());
    }
    return {};
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
    _dcompTarget = MakeTarget(_hwnd);
    _dcompVisual = dcompDevice->CreateVisual();

    if (_dcompTarget && _dcompVisual)
    {
        _dcompTarget->SetRoot(_dcompVisual.Get());
    }
}

///
/// @brief
///
auto PlatformRootViewLayerWin::GetControl() -> Shared<PlatformViewLayerControl>
{
    return {};
}

///
/// @brief Render all visuals.
///
auto PlatformRootViewLayerWin::Render() -> void
{
    auto bgn = std::chrono::high_resolution_clock::now();

    _dcompVisual->RemoveAllVisuals();

    auto rebuildBgn = std::chrono::high_resolution_clock::now();
    for (auto const& layerId : _layersToRebuild)
    {
        // Check if the layer still exists.
        // TODO: We can cache weak references to avoid tree traversal.
        auto const layer = FindViewLayerById(layerId, GetSelf());
        if (layer)
        {
            RebuildVisual(layer);
        }
    }
    _layersToRebuild.clear();
    auto rebuildEnd = std::chrono::high_resolution_clock::now();
    auto rebuildDur = std::chrono::duration_cast<std::chrono::milliseconds>(rebuildEnd - rebuildBgn).count();

    auto updateBgn = std::chrono::high_resolution_clock::now();
    if (_shouldUpdateLayer)
    {
        // TODO: Improve algo so that we don't need to O(n) traverse all layers.
        UpdateVisual();
        _shouldUpdateLayer = false;
    }
    auto updateEnd = std::chrono::high_resolution_clock::now();
    auto updateDur = std::chrono::duration_cast<std::chrono::milliseconds>(updateEnd - updateBgn).count();

    auto traverseVisual = [](this auto self, Shared<PlatformViewLayerVisualWin> const& visual, auto&& func) -> void {
        if (visual)
        {
            func(visual);
            for (auto const& child : visual->GetChildren())
            {
                self(child, func);
            }
        }
    };

    traverseVisual(_visual, [&](auto const& visual) {
        if (visual)
        {
            _dcompVisual->AddVisual(visual->GetVisual().Get(), FALSE, nullptr);
            visual->Render();
        }
    });

    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - bgn).count();
    FW_DEBUG_LOG_INFO("PlatformRootViewLayerWin::Render: took {} ms, rebuild: {} ms, update: {} ms", dur, rebuildDur, updateDur);
}

///
/// @brief Initialize.
///
auto PlatformRootViewLayerWin::Initialize() -> void
{
    _visual = Shared<PlatformViewLayerVisualWin>::Make(GetCompositionDevice());
    _visual->SetBaseLayerId(GetId());
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

///
/// @brief Notify offset change.
///
auto PlatformRootViewLayerWin::RootOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
    QueueUpdateLayer();
}

///
/// @brief Notify size change.
///
auto PlatformRootViewLayerWin::RootSizeChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
    QueueUpdateLayer();
}

///
/// @brief Notify clip mode change.
///
auto PlatformRootViewLayerWin::RootClipModeChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
    QueueUpdateLayer();
}

///
/// @brief Notify opacity change.
///
auto PlatformRootViewLayerWin::RootOpacityChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
    QueueUpdateLayer();
}

///
/// @brief Notify render flags change.
///
auto PlatformRootViewLayerWin::RootRenderFlagsChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    if (auto const baseLayer = GetRasterizingBaseLayer(layer))
    {
        QueueRebuildLayer(baseLayer->GetId());
    }
}

///
/// @brief Notify display list change.
///
auto PlatformRootViewLayerWin::RootDisplayListChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    // TODO: This should not require full update.
    (void)layer;
    QueueUpdateLayer();
}

///
/// @brief Notify display list offset change.
///
auto PlatformRootViewLayerWin::RootDisplayListOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    // TODO: This should not require full update.
    (void)layer;
    QueueUpdateLayer();
}

///
/// @brief Notify child added.
///
auto PlatformRootViewLayerWin::RootChildAdded(Shared<PlatformViewLayerWin> const& child) -> void
{
    if (auto const baseLayer = GetRasterizingBaseLayer(child))
    {
        QueueRebuildLayer(baseLayer->GetId());
    }
}

///
/// @brief Notify child removed.
///
auto PlatformRootViewLayerWin::RootChildRemoved(Shared<PlatformViewLayerWin> const& parent) -> void
{
    if (auto const layer = GetRasterizingLayer(parent))
    {
        QueueRebuildLayer(layer->GetId());
    }
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

///
/// @brief 
///
/// @param layerId 
///
auto PlatformRootViewLayerWin::FindBaseVisualByBaseLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisualWin>
{
    auto traverse = [](this auto self, Shared<PlatformViewLayerVisualWin> const& visual, PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisualWin> {
        if (visual)
        {
            if (visual->GetBaseLayerId() == layerId)
            {
                return visual;
            }

            for (auto const& child : visual->GetChildren())
            {
                if (auto const found = self(child, layerId))
                {
                    return found;
                }
            }
        }
        return {};
    };
    return traverse(_visual, layerId);
}

///
/// @brief 
///
/// @param baseLayer 
///
auto PlatformRootViewLayerWin::RebuildVisual(Shared<PlatformViewLayerWin> const& baseLayer) -> void
{
    FW_DEBUG_LOG_INFO("Rebuilding visuals starting from layer ID {}", (uint64_t)(UInt64)baseLayer->GetId());

    // find visual corresponding to base layer.
    auto const baseVisual = FindBaseVisualByBaseLayerId(baseLayer->GetId());

    // rebuild with builder.
    auto builder = PlatformViewLayerVisualUpdaterWin(GetCompositionDevice());
    builder.Update(baseVisual, baseLayer);
}

///
/// @brief 
///
auto PlatformRootViewLayerWin::UpdateVisual() -> void
{
    auto updater = PlatformViewLayerVisualPropertyUpdaterWin();
    updater.Update(_visual, GetSelf());
}

///
/// @brief Request rebuild.
///
auto PlatformRootViewLayerWin::QueueRebuildLayer(PlatformViewLayerId const layerId) -> void
{
    for (auto const& id : _layersToRebuild)
    {
        if (id == layerId)
        {
            return;
        }
    }
    _layersToRebuild.push_back(layerId);
}

///
/// @brief Request update.
///
auto PlatformRootViewLayerWin::QueueUpdateLayer() -> void
{
    _shouldUpdateLayer = true;
}
}
