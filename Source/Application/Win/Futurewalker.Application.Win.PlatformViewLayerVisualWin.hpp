// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerVisualWinType.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceWinType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.OrderedHashMap.hpp"

#include <wrl/client.h>
#include <dcomp.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Visual object.
///
class PlatformViewLayerVisualWin : NonCopyable
{
public:
    explicit PlatformViewLayerVisualWin(Shared<PlatformDCompositionDeviceWin> const& device);

    auto GetVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;

    auto GetBaseLayerId() const -> PlatformViewLayerId;
    auto SetBaseLayerId(PlatformViewLayerId const layerId) -> void;

    auto SetOffset(Offset<Dp> const& offset) -> void;
    auto SetClipRect(Rect<Dp> const& clipRect) -> void;
    auto SetOpacity(Float64 const opacity) -> void;
    auto SetDisplayScale(DisplayScale const scale) -> void;
    auto SetBackingScale(BackingScale const scale) -> void;

    struct Fragment
    {
        Offset<Dp> offset;
        Rect<Dp> clipRect;
        Float64 opacity = 1.0;
        Shared<Graphics::DisplayList> displayList;
        Offset<Dp> displayListOffset;
    };
    auto InsertFragment(const SInt64 index, PlatformViewLayerId layerId, Fragment const& fragment) -> void;
    auto ReplaceFragment(const SInt64 index, PlatformViewLayerId layerId, Fragment const& fragment) -> void;
    auto RemoveFragment(const SInt64 index) -> void;
    auto ClearFragments() -> void;
    auto GetFragmentIndexByLayerId(PlatformViewLayerId const layerId) const -> Optional<SInt64>;
    auto GetFragment(SInt64 const index) const -> Optional<Fragment>;
    auto GetFragmentCount() const -> SInt64;

    auto Render() -> void;

private:
    auto Invalidate() -> void;
    auto UpdateOffset() -> void;
    auto UpdateClipRect() -> void;
    auto UpdateOpacity() -> void;

private:
    Shared<PlatformDCompositionDeviceWin> _device;
    Shared<PlatformViewLayerSurfaceWin> _surface;
    Microsoft::WRL::ComPtr<IDCompositionVisual3> _visual;
    PlatformViewLayerId _baseLayerId = PlatformViewLayerId(0U);
    Bool _invalidated = true;
    Offset<Dp> _offset;
    Rect<Dp> _clipRect;
    Float64 _opacity = 1.0;
    OrderedHashMap<PlatformViewLayerId, Fragment> _fragments;
    DisplayScale _displayScale = 1.0;
    BackingScale _backingScale = 1.0;
};
}
}
