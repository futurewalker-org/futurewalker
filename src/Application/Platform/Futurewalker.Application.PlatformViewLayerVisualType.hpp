// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Platform.Prelude.hpp"

#include "Futurewalker.Graphics.PixelGeometry.hpp"

#include "Futurewalker.Unit.Graphics.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerVisual;

///
/// @brief Rendering parameters of PlatformViewLayerVisual.
///
struct PlatformViewLayerVisualRenderParams
{
    DisplayScale displayScale = 1.0; /// Display scale.
    BackingScale backingScale = 1.0; /// Backing scale.
    Graphics::PixelGeometry pixelGeometry = Graphics::PixelGeometry::Unknown; /// Pixel geometry of target screen.
    Float64 textGamma = 0.0; /// Text gamma.
    Float64 textContrast = 0.0; /// Text contrast.

    friend auto operator==(PlatformViewLayerVisualRenderParams const&, PlatformViewLayerVisualRenderParams const&) -> bool = default;
    friend auto operator!=(PlatformViewLayerVisualRenderParams const&, PlatformViewLayerVisualRenderParams const&) -> bool = default;
};
}
}
