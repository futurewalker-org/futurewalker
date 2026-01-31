// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.SkiaSceneType.hpp"
#include "Futurewalker.Graphics.Scene.hpp"

#include "Futurewalker.Core.Pointer.hpp"

#include <include/core/SkCanvas.h>

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief
///
class SkiaScene : public Scene
{
public:
    auto SetCanvas(Pointer<SkCanvas> const canvas) -> void;

    auto PushLayer(LayerParam param) -> void override;
    auto PushClipRect(ClipRectParam param) -> void override;
    auto PushClipRoundRect(ClipRoundRectParam param) -> void override;
    auto PushClipPath(ClipPathParam param) -> void override;
    auto PushTranslate(TranslateParam param) -> void override;
    auto PushRotate(RotateParam param) -> void override;
    auto PushScale(ScaleParam param) -> void override;
    auto Pop(PopParam param) -> void override;
    auto AddLine(LineParam param) -> void override;
    auto AddRect(RectParam param) -> void override;
    auto AddRoundRect(RoundRectParam param) -> void override;
    auto AddGlyphRun(GlyphRunParam param) -> void override;
    auto AddDisplayList(DisplayListParam param) -> void override;
    auto AddPath(PathParam param) -> void override;

private:
    Pointer<SkCanvas> _canvas = nullptr;
};
}
