// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.ViewDrawInfoType.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.ViewLayerType.hpp"
#include "Futurewalker.Application.ViewLayerManagerType.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerContextType.hpp"
#include "Futurewalker.Application.PlatformViewLayerType.hpp"

#include "Futurewalker.Graphics.RecordingSurfaceType.hpp"
#include "Futurewalker.Graphics.GraphicsCanvasType.hpp"
#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.GraphicsUnits.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
///
/// @brief Draw info.
///
class ViewDrawInfo
{
public:
    ViewDrawInfo();

    auto InvalidateLayer() -> Bool;
    auto InvalidatePath() -> Bool;

    auto IsInDrawPath() const -> Bool;

    auto BeginDraw() -> Bool;
    auto EndDraw() -> void;

    auto BeginDrawPath() -> Bool;
    auto EndDrawPath() -> void;

    auto SetDisplayList(Shared<DisplayList> const& displayList) -> void;
    auto SetDisplayScale(DisplayScale const displayScale) -> void;
    auto SetBackingScale(BackingScale const backingScale) -> void;
    auto SetClipMode(ViewClipMode const clipMode) -> void;
    auto SetOpacity(Float64 const opacity) -> void;
    auto UpdateLayers(ViewLayer& layer) -> void;

    auto GetSubLayer(ViewLayerId const id) -> Shared<ViewLayer>;

    auto AddSubLayerInfo(ViewLayerKind const kind) -> ViewLayerId;
    auto RemoveSubLayerInfo(ViewLayerId const id) -> void;

    auto AttachSubLayer(ViewLayer& layer, ViewLayerManager& layerManager, ViewLayerId const id) -> void;
    auto DetachSubLayer(ViewLayer& layer, ViewLayerId const id) -> void;

    auto AttachAllSubLayers(ViewLayer& layer, ViewLayerManager& layerManager) -> void;
    auto DetachAllSubLayers(ViewLayer& layer) -> void;

private:
    auto FindSubLayerAttachPosition(ViewLayer& layer, ViewLayerId const id) -> Shared<ViewLayer>;

private:
    struct SubLayerInfo
    {
        Unique<UniqueViewLayerId> id;
        ViewLayerKind kind;
        Shared<ViewLayer> layer;
    };
    Bool _invalidated = false;
    Bool _invalidatedPath = false;
    std::vector<SubLayerInfo> _subLayerInfo;
    ViewLayerId _drawableLayerId = ViewLayerId(0U);
    Shared<DisplayList> _displayList;
    DisplayScale _displayScale = 1.0;
    BackingScale _backingScale = 1.0;
    ViewClipMode _clipMode = ViewClipMode::None;
    Float64 _opacity = 1.0;
};
}
