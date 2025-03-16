// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.SceneType.hpp"

#include "Futurewalker.Graphics.DisplayListType.hpp"
#include "Futurewalker.Graphics.ShapedTextType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Base class of scene builders.
///
class Scene
{
public:
    struct LayerParam
    {
        Rect<Dp> rect;
        RGBAColor color = RGBAColor(0, 0, 0, 1);
        BlendMode blendMode = BlendMode::SrcOver;
    };

    struct ClipRectParam
    {
        Rect<Dp> rect;
        Bool antiAlias = false;
    };

    struct ClipRoundRectParam
    {
        RoundRect<Dp> roundRect;
        Bool antiAlias = false;
    };

    struct TranslateParam
    {
        Offset<Dp> translate;
    };

    struct RotateParam
    {
        Degree rotate = 0;
    };

    struct ScaleParam
    {
        Float64 x = 0;
        Float64 y = 0;
    };

    struct PopParam
    {
    };

    struct LineParam
    {
        Point<Dp> p1;
        Point<Dp> p2;
        RGBAColor color = RGBAColor(0, 0, 0, 1);
        DrawStyle drawStyle = DrawStyle::Fill;
        BlendMode blendMode = BlendMode::SrcOver;
        Bool antiAlias = false;
        Dp strokeWidth = 1.0;
    };

    struct RectParam
    {
        Rect<Dp> rect;
        RGBAColor color = RGBAColor(0, 0, 0, 1);
        DrawStyle drawStyle = DrawStyle::Fill;
        BlendMode blendMode = BlendMode::SrcOver;
        Bool antiAlias = false;
        Dp strokeWidth = 1.0;
    };

    struct RoundRectParam
    {
        RoundRect<Dp> roundRect;
        RGBAColor color = RGBAColor(0, 0, 0, 1);
        DrawStyle drawStyle = DrawStyle::Fill;
        BlendMode blendMode = BlendMode::SrcOver;
        Bool antiAlias = false;
        Dp strokeWidth = 1.0;
    };

    struct ShapedTextParam
    {
        Shared<ShapedText> shapedText;
    };

    struct DisplayListParam
    {
        Shared<DisplayList> displayList;
    };

    virtual ~Scene() = 0;
    virtual auto PushLayer(LayerParam param) -> void = 0;
    virtual auto PushClipRect(ClipRectParam param) -> void = 0;
    virtual auto PushClipRoundRect(ClipRoundRectParam param) -> void = 0;
    virtual auto PushTranslate(TranslateParam param) -> void = 0;
    virtual auto PushRotate(RotateParam param) -> void = 0;
    virtual auto PushScale(ScaleParam param) -> void = 0;
    virtual auto Pop(PopParam param) -> void = 0;
    virtual auto AddLine(LineParam param) -> void = 0;
    virtual auto AddRect(RectParam param) -> void = 0;
    virtual auto AddRoundRect(RoundRectParam param) -> void = 0;
    virtual auto AddShapedText(ShapedTextParam param) -> void = 0;
    virtual auto AddDisplayList(DisplayListParam param) -> void = 0;
};
}
}
