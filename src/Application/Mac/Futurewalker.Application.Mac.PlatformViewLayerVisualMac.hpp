// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformViewLayerVisualMacType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS implementation of the view layer visual.
///
class PlatformViewLayerVisualMac : public PlatformViewLayerVisual
{
public:
    static auto Make() -> Shared<PlatformViewLayerVisualMac>;

    PlatformViewLayerVisualMac(PassKey<PlatformViewLayerVisual>);

    auto Render(NSWindow* window) -> void;
    auto GetView() -> NSView*;

protected:
    auto Initialize() -> void override;
    auto OnFragmentChanged() -> void override;
    auto OnOffsetChanged() -> void override;
    auto OnClipRectChanged() -> void override;
    auto OnClipPathChanged() -> void override;
    auto OnOpacityChanged() -> void override;
    auto OnDisplayScaleChanged() -> void override;
    auto OnBackingScaleChanged() -> void override;

private:
    __strong NSView* _view = nil;
    Bool _needsUpdateGeometry = true;
    Bool _needsUpdateContents = true;
};
}
}
