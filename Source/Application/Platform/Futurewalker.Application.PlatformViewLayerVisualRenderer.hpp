// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformViewLayerVisualRenderer.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualContextType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerVisualRenderer : NonCopyable
{
public:
    struct Delegate
    {
        Function<void()> renderBegin;
        Function<void()> renderEnd;
        Function<void(Shared<PlatformViewLayerVisual> const& visual)> renderVisual;
    };
    PlatformViewLayerVisualRenderer(Delegate const& delegate, Shared<PlatformViewLayer> const& layer, Shared<PlatformViewLayerVisualContext> const& context);

    auto Render() -> void;
    auto RequestRebuildLayer() -> void;
    auto RequestUpdateLayer() -> void;

private:
    auto GetLayer() -> Shared<PlatformViewLayer>;
    auto FindBaseVisualByBaseLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisual>;
    auto RebuildVisual(Shared<PlatformViewLayer> const& baseLayer) -> void;
    auto UpdateVisual() -> void;
    auto RenderBegin() -> void;
    auto RenderEnd() -> void;
    auto RenderVisual(Shared<PlatformViewLayerVisual> const& visual) -> void;

private:
    Delegate _delegate;
    Weak<PlatformViewLayer> _layer;
    Shared<PlatformViewLayerVisualContext> _context;
    Shared<PlatformViewLayerVisual> _visual;
    Bool _shouldRebuildLayer = true;
    Bool _shouldUpdateLayer = true;
};
}
}
