// SPDX-License-Identifier: MIT

#include "Futurewalker.Graphics.SkiaSceneRecorder.hpp"
#include "Futurewalker.Graphics.SkiaScene.hpp"
#include "Futurewalker.Graphics.SkiaDisplayList.hpp"

#include <include/core/SkBBHFactory.h>

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief
///
auto SkiaSceneRecorder::BeginRecording() -> Shared<Scene>
{
    if (!_recording)
    {
        auto const infinityBounds = SkRect::MakeLTRB(-SK_ScalarInfinity, -SK_ScalarInfinity, SK_ScalarInfinity, SK_ScalarInfinity);
        _canvas = Shared<SkiaScene>::Make();
        _canvas->SetCanvas(_recorder.beginRecording(infinityBounds, SkRTreeFactory()()));
        _recording = true;
    }
    return _canvas;
}

///
/// @brief
///
auto SkiaSceneRecorder::EndRecording() -> Shared<DisplayList>
{
    if (_recording)
    {
        auto displayList = Shared<SkiaDisplayList>::Make(_recorder.finishRecordingAsPicture());
        _canvas->SetCanvas(nullptr);
        _canvas = nullptr;
        _recording = false;
        return displayList;
    }
    return {};
}
}
