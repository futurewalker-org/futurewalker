// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.DrawScopeType.hpp"
#include "Futurewalker.Application.DrawParameter.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.RootViewType.hpp"

#include "Futurewalker.Graphics.SceneType.hpp"
#include "Futurewalker.Graphics.SceneRecorderType.hpp"
#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.Path.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.ReferenceArg.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Optional.hpp"

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

    auto GetScene() -> Graphics::Scene&;

    auto SetClipPath(Graphics::Path const& path) -> void;
    auto SetClipMode(ViewClipMode const clipMode) -> void;
    auto SetOpacity(Float64 const opacity) -> void;

    auto GetDisplayList(PassKey<View>) -> Shared<Graphics::DisplayList>;
    auto GetClipPath(PassKey<View>) -> Optional<Graphics::Path>;
    auto GetClipMode(PassKey<View>) -> ViewClipMode;
    auto GetOpacity(PassKey<View>) -> Float64;

    static auto DrawRootView(PassKey<RootView>, View& view) -> void;

private:
    auto BeginRecording() -> Graphics::Scene&;
    auto EndRecording() -> Shared<Graphics::DisplayList>;

private:
    View& _view;
    DrawParameter _parameter;
    Shared<Graphics::SceneRecorder> _recordingSurface;
    Shared<Graphics::Scene> _recordingCanvas;
    Shared<Graphics::DisplayList> _recordedDisplayList;
    Optional<Graphics::Path> _clipPath;
    ViewClipMode _clipMode = ViewClipMode::None;
    Float64 _opacity = 1.0;
};
}
}
