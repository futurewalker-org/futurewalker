// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Graphics.SceneRecorder.hpp"
#include "Futurewalker.Graphics.NoOpScene.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Construct draw scope.
///
/// @param view
/// @param parameter
///
DrawScope::DrawScope(PassKey<View>, View& view, DrawParameter const& parameter)
  : _view {view}
  , _parameter {parameter}
{
}

///
/// @brief
///
auto DrawScope::GetParameter() const -> DrawParameter const&
{
    return _parameter;
}

///
/// @brief
///
auto DrawScope::GetScene() -> Graphics::Scene&
{
    return BeginRecording();
}

///
/// @brief
///
auto DrawScope::SetClipMode( ViewClipMode const clipMode) -> void
{
    _clipMode = clipMode;
}

///
/// @brief
///
auto DrawScope::SetOpacity(Float64 const opacity) -> void
{
    _opacity = opacity;
}

///
/// @brief
///
auto DrawScope::GetDisplayList(PassKey<View>) -> Shared<Graphics::DisplayList>
{
    return EndRecording();
}

///
/// @brief
///
auto DrawScope::GetClipMode(PassKey<View>) -> ViewClipMode
{
    return _clipMode;
}

///
/// @brief
///
auto DrawScope::GetOpacity(PassKey<View>) -> Float64
{
    return _opacity;
}

///
/// @brief
///
/// @param view
///
auto DrawScope::DrawRootView(PassKey<RootView>, View& view) -> void
{
    view.EnterDrawScope({});
}

///
/// @brief
///
auto DrawScope::BeginRecording() -> Graphics::Scene&
{
    _recordedDisplayList = nullptr;

    if (!_recordingSurface)
    {
        _recordingSurface = Graphics::SceneRecorder::Make();
        _recordingCanvas = _recordingSurface->BeginRecording();
    }

    if (!_recordingCanvas)
    {
        FW_DEBUG_ASSERT(false);
        _recordingCanvas = Shared<Graphics::NoOpScene>::Make();
    }
    return *_recordingCanvas;
}

///
/// @brief
///
auto DrawScope::EndRecording() -> Shared<Graphics::DisplayList>
{
    if (_recordingSurface)
    {
        _recordingCanvas = nullptr;
        _recordedDisplayList = _recordingSurface->EndRecording();
        _recordingSurface = nullptr;
    }
    return _recordedDisplayList;
}
}
