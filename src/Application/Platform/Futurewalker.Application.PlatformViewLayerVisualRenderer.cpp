// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformViewLayerVisualRenderer.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualContext.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisual.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualPropertyUpdater.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
PlatformViewLayerVisualRenderer::PlatformViewLayerVisualRenderer(Delegate const& delegate, Shared<PlatformViewLayer> const& layer, Shared<PlatformViewLayerVisualContext> const& context)
  : _delegate {delegate}
  , _layer {layer}
  , _context {context}
{
    if (context && layer)
    {
        _visual = context->CreateVisual();
        _visual->SetBaseLayerId(layer->GetId());
        _visualUpdater = Shared<PlatformViewLayerVisualUpdater>::Make(context);
        _visualPropertyUpdater = Shared<PlatformViewLayerVisualPropertyUpdater>::Make();
        RebuildVisual();
    }
    else
    {
        FW_DEBUG_LOG_ERROR("PlatformRootViewLayer: visualContext is null");
        throw Exception(ErrorCode::InvalidArgument);
    }
}

auto PlatformViewLayerVisualRenderer::Render() -> void
{
    RenderBegin();

    if (_shouldRebuildLayer)
    {
        RebuildVisual();
        _shouldRebuildLayer = false;
        _shouldUpdateLayer = false;
    }
    else if (_shouldUpdateLayer)
    {
        UpdateVisual();
        _shouldUpdateLayer = false;
    }

    auto traverseVisual = [this](this auto self, Shared<PlatformViewLayerVisual> const& visual) -> void {
        if (visual)
        {
            RenderVisual(visual);

            for (auto const& child : visual->GetChildren())
            {
                self(child);
            }
        }
    };
    traverseVisual(_visual);

    RenderEnd();
}

auto PlatformViewLayerVisualRenderer::RequestRebuildLayer() -> void
{
    _shouldRebuildLayer = true;
}

auto PlatformViewLayerVisualRenderer::RequestUpdateLayer() -> void
{
    _shouldUpdateLayer = true;
}

auto PlatformViewLayerVisualRenderer::GetLayer() -> Shared<PlatformViewLayer>
{
    return _layer.Lock();
}

auto PlatformViewLayerVisualRenderer::FindBaseVisualByBaseLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisual>
{
    auto traverse = [](this auto self, Shared<PlatformViewLayerVisual> const& visual, PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisual> {
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

auto PlatformViewLayerVisualRenderer::RebuildVisual() -> void
{
    if (_visualUpdater)
    {
        _visualUpdater->Update(_visual, GetLayer());
    }
}

auto PlatformViewLayerVisualRenderer::UpdateVisual() -> void
{
    if (_visualPropertyUpdater)
    {
        _visualPropertyUpdater->Update(_visual, GetLayer());
    }
}

auto PlatformViewLayerVisualRenderer::RenderBegin() -> void
{
    if (_delegate.renderBegin)
    {
        _delegate.renderBegin();
    }
}

auto PlatformViewLayerVisualRenderer::RenderEnd() -> void
{
    if (_delegate.renderEnd)
    {
        _delegate.renderEnd();
    }
}

auto PlatformViewLayerVisualRenderer::RenderVisual(Shared<PlatformViewLayerVisual> const& visual) -> void
{
    if (_delegate.renderVisual)
    {
        _delegate.renderVisual(visual);
    }
}
}
