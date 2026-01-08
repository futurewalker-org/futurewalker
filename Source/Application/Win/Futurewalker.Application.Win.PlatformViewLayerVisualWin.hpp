// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerVisualWinType.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceWinType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <wrl/client.h>
#include <dcomp.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Visual object.
///
/// Visual object stores contents of multiple view layers as fragments.
/// Parent-child relationship is only for retaining information of original view layer hierarchy.
///
/// There are visuals those that correspond to rasterizing view layers and those that do not.
/// A visual that correspond to rasterizing view layer includes a fragment for the base layer itself.
///
class PlatformViewLayerVisualWin : public PlatformViewLayerVisual
{
public:
    static auto Make(Shared<PlatformDCompositionDeviceWin> const& device) -> Shared<PlatformViewLayerVisualWin>;

    PlatformViewLayerVisualWin(PassKey<PlatformViewLayerVisual>, Shared<PlatformDCompositionDeviceWin> const& device);

    auto GetVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;
    auto Render() -> void;

private:
    auto Invalidate() -> void;
    auto Initialize() -> void override;
    auto OnFragmentChanged() -> void override;
    auto OnOffsetChanged() -> void override;
    auto OnClipRectChanged() -> void override;
    auto OnOpacityChanged() -> void override;
    auto OnDisplayScaleChanged() -> void override;
    auto OnBackingScaleChanged() -> void override;

private:
    Shared<PlatformDCompositionDeviceWin> _device;
    Shared<PlatformViewLayerSurfaceWin> _surface;
    Microsoft::WRL::ComPtr<IDCompositionVisual3> _visual;
    Bool _invalidated = true;
};
}
}
