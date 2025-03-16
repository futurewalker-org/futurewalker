// SPDX-License-Identifier: MIT

#include "Futurewalker.Graphics.NoOpScene.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
auto NoOpScene::PushLayer(LayerParam param) -> void
{
    (void)param;
}

auto NoOpScene::PushClipRect(ClipRectParam param) -> void
{
    (void)param;
}

auto NoOpScene::PushClipRoundRect(ClipRoundRectParam param) -> void
{
    (void)param;
}

auto NoOpScene::PushTranslate(TranslateParam param) -> void
{
    (void)param;
}

auto NoOpScene::PushRotate(RotateParam param) -> void
{
    (void)param;
}

auto NoOpScene::PushScale(ScaleParam param) -> void
{
    (void)param;
}

auto NoOpScene::Pop(PopParam param) -> void
{
    (void)param;
}

auto NoOpScene::AddLine(LineParam param) -> void
{
    (void)param;
}

auto NoOpScene::AddRect(RectParam param) -> void
{
    (void)param;
}

auto NoOpScene::AddRoundRect(RoundRectParam param) -> void
{
    (void)param;
}

auto NoOpScene::AddShapedText(ShapedTextParam param) -> void
{
    (void)param;
}

auto NoOpScene::AddDisplayList(DisplayListParam param) -> void
{
    (void)param;
}
}
