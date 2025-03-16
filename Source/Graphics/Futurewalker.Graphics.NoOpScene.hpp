// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.NoOpSceneType.hpp"
#include "Futurewalker.Graphics.Scene.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class NoOpScene : public Scene
{
public:
    auto PushLayer(LayerParam param) -> void override;
    auto PushClipRect(ClipRectParam param) -> void override;
    auto PushClipRoundRect(ClipRoundRectParam param) -> void override;
    auto PushTranslate(TranslateParam param) -> void override;
    auto PushRotate(RotateParam param) -> void override;
    auto PushScale(ScaleParam param) -> void override;
    auto Pop(PopParam param) -> void override;
    auto AddLine(LineParam param) -> void override;
    auto AddRect(RectParam param) -> void override;
    auto AddRoundRect(RoundRectParam param) -> void override;
    auto AddShapedText(ShapedTextParam param) -> void override;
    auto AddDisplayList(DisplayListParam param) -> void override;
};
}
}
