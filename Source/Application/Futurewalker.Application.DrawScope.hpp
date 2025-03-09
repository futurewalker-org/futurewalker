// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.DrawScopeType.hpp"
#include "Futurewalker.Application.DrawParameter.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.RootViewType.hpp"

#include "Futurewalker.Graphics.GraphicsCanvasType.hpp"
#include "Futurewalker.Graphics.RecordingSurfaceType.hpp"
#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.Rect.hpp"
#include "Futurewalker.Graphics.GraphicsUnits.hpp"

#include "Futurewalker.Core.ReferenceArg.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Draw scope.
///
class DrawScope : NonCopyable
{
public:
    DrawScope(PassKey<View>, View& view, DrawParameter const& parameter);

    auto GetParameter() const -> DrawParameter const&;

    auto GetCanvas() -> GraphicsCanvas&;

    auto SetClipMode(ViewClipMode const clipMode) -> void;
    auto SetOpacity(Float64 const opacity) -> void;

    auto GetDisplayList(PassKey<View>) -> Shared<DisplayList>;
    auto GetClipMode(PassKey<View>) -> ViewClipMode;
    auto GetOpacity(PassKey<View>) -> Float64;

    static auto DrawRootView(PassKey<RootView>, View& view) -> void;

private:
    auto BeginRecording() -> GraphicsCanvas&;
    auto EndRecording() -> Shared<DisplayList>;

private:
    View& _view;
    DrawParameter _parameter;
    Shared<RecordingSurface> _recordingSurface;
    Shared<GraphicsCanvas> _recordingCanvas;
    Shared<DisplayList> _recordedDisplayList;
    ViewClipMode _clipMode = ViewClipMode::None;
    Float64 _opacity = 1.0;
};
}
}
