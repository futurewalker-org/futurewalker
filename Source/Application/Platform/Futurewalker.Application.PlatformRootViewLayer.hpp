// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformRootViewLayerType.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp" 
#include "Futurewalker.Application.PlatformViewLayerVisualContextType.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualType.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformRootViewLayer : public PlatformViewLayer
{
public:
    PlatformRootViewLayer(PassKey<PlatformViewLayer> key, Shared<PlatformViewLayerVisualContext> const& visualContext);

    auto Render() -> void;

protected:
    auto Initialize() -> void final;

private:
    auto RootOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void final;
    auto RootSizeChanged(Shared<PlatformViewLayer> const& layer) -> void final;
    auto RootClipModeChanged(Shared<PlatformViewLayer> const& layer) -> void final;
    auto RootOpacityChanged(Shared<PlatformViewLayer> const& layer) -> void final;
    auto RootRenderFlagsChanged(Shared<PlatformViewLayer> const& layer) -> void final;
    auto RootDisplayListChanged(Shared<PlatformViewLayer> const& layer) -> void final;
    auto RootDisplayListOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void final;
    auto RootChildAdded(Shared<PlatformViewLayer> const& child) -> void final;
    auto RootChildRemoved(Shared<PlatformViewLayer> const& parent) -> void final;

private:
    ///
    /// @brief Called before rendering begins.
    ///
    virtual auto RenderBegin() -> void = 0;
    ///
    /// @brief Called after rendering ends.
    ///
    virtual auto RenderEnd() -> void = 0;
    ///
    /// @brief Called for each visual during rendering.
    ///
    virtual auto RenderVisual(Shared<PlatformViewLayerVisual> const& visual) -> void = 0;

private:
    auto FindBaseVisualByBaseLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisual>;
    auto QueueRebuildLayer(PlatformViewLayerId const layerId) -> void;
    auto QueueUpdateLayer() -> void;
    auto RebuildVisual(Shared<PlatformViewLayer> const& baseLayer) -> void;
    auto UpdateVisual() -> void;

private:
    Shared<PlatformViewLayerVisualContext> _visualContext;
    Shared<PlatformViewLayerVisual> _visual;
    std::vector<PlatformViewLayerId> _layersToRebuild;
    Bool _shouldUpdateLayer = false;
};
}
}
