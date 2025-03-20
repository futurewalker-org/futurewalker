// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.SkiaSceneRecorderType.hpp"
#include "Futurewalker.Graphics.SkiaSceneType.hpp"
#include "Futurewalker.Graphics.SceneRecorder.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include <include/core/SkPictureRecorder.h>

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief
///
class SkiaSceneRecorder : public SceneRecorder
{
public:
    auto BeginRecording() -> Shared<Scene> override;
    auto EndRecording() -> Shared<DisplayList> override;

private:
    Bool _recording = false;
    Size<Dp> _size;
    SkPictureRecorder _recorder;
    Shared<SkiaScene> _canvas;
};
}
