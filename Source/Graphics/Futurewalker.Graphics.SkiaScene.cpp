// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaScene.hpp"
#include "Futurewalker.Graphics.SkiaDisplayList.hpp"
#include "Futurewalker.Graphics.SkiaGlyphRun.hpp"

#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace
{
auto PointToSkPoint(Point<Dp> const& point) -> SkPoint
{
    return SkPoint {
        .fX = static_cast<SkScalar>(point.GetX()),
        .fY = static_cast<SkScalar>(point.GetY()),
    };
}

auto RectToSkRect(Rect<Dp> const& rect)
{
    return SkRect {
        .fLeft = static_cast<SkScalar>(rect.GetLeft()),
        .fTop = static_cast<SkScalar>(rect.GetTop()),
        .fRight = static_cast<SkScalar>(rect.GetRight()),
        .fBottom = static_cast<SkScalar>(rect.GetBottom()),
    };
}

auto RadiusToSkVector(Radius<Dp> const& radius) -> SkVector
{
    return SkVector {
        .fX = static_cast<SkScalar>(radius.GetX()),
        .fY = static_cast<SkScalar>(radius.GetY()),
    };
}

auto RoundRectToSkRRect(RoundRect<Dp> const& roundRect) -> SkRRect
{
    auto rr = SkRRect();
    auto radii = std::array<SkVector, 4> {
        RadiusToSkVector(roundRect.GetTopLeftRadius()),
        RadiusToSkVector(roundRect.GetTopRightRadius()),
        RadiusToSkVector(roundRect.GetBottomRightRadius()),
        RadiusToSkVector(roundRect.GetBottomLeftRadius()),
    };
    rr.setRectRadii(RectToSkRect(roundRect.GetRect()), radii.data());
    return rr;
}

auto RGBAColorToSkColor4f(RGBAColor const& color)
{
    return SkColor4f {
        .fR = static_cast<SkScalar>(color.GetRedF64()),
        .fG = static_cast<SkScalar>(color.GetGreenF64()),
        .fB = static_cast<SkScalar>(color.GetBlueF64()),
        .fA = static_cast<SkScalar>(color.GetAlphaF64()),
    };
}

auto DrawStyleToSkPaintStyle(DrawStyle const& style)
{
    switch (style)
    {
        case DrawStyle::Fill:
            return SkPaint::kFill_Style;
        case DrawStyle::Stroke:
            return SkPaint::kStroke_Style;
    }
    return SkPaint::kFill_Style;
}

auto BlendModeToSkBlendMode(BlendMode const& mode)
{
    switch (mode)
    {
        case BlendMode::SrcOver:
            return SkBlendMode::kSrcOver;
        case BlendMode::DstOver:
            return SkBlendMode::kDstOver;
        case BlendMode::Src:
            return SkBlendMode::kSrc;
        case BlendMode::Dst:
            return SkBlendMode::kDst;
    }
    return SkBlendMode::kSrcOver;
}

auto SceneParamToSkPaint(auto const& param) -> SkPaint
{
    SkPaint p;

    if constexpr (requires { param.color; })
    {
        p.setColor4f(RGBAColorToSkColor4f(param.color));
    }

    if constexpr (requires { param.drawStyle; })
    {
        p.setStyle(DrawStyleToSkPaintStyle(param.drawStyle));
    }

    if constexpr (requires { param.blendMode; })
    {
        p.setBlendMode(BlendModeToSkBlendMode(param.blendMode));
    }

    if constexpr (requires { param.antiAlias; })
    {
        p.setAntiAlias(static_cast<bool>(param.antiAlias));
    }

    if constexpr (requires { param.strokeWidth; })
    {
        p.setStrokeWidth(static_cast<SkScalar>(param.strokeWidth));
    }

    return p;
}
}

///
/// @brief
///
/// @param canvas
///
auto SkiaScene::SetCanvas(Pointer<SkCanvas> const canvas) -> void
{
    _canvas = canvas;
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::PushLayer(LayerParam param) -> void
{
    if (_canvas)
    {
        auto const p = SceneParamToSkPaint(param);
        _canvas->saveLayer(nullptr, &p);
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::PushClipRect(ClipRectParam param) -> void
{
    if (_canvas)
    {
        auto const r = RectToSkRect(param.rect);
        auto const aa = static_cast<bool>(param.antiAlias);
        _canvas->save();
        _canvas->clipRect(r, aa);
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::PushClipRoundRect(ClipRoundRectParam param) -> void
{
    if (_canvas)
    {
        auto const rr = RoundRectToSkRRect(param.roundRect);
        auto const aa = static_cast<bool>(param.antiAlias);
        _canvas->save();
        _canvas->clipRRect(rr, aa);
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::PushTranslate(TranslateParam param) -> void
{
    if (_canvas)
    {
        auto const dx = static_cast<SkScalar>(param.x);
        auto const dy = static_cast<SkScalar>(param.y);
        _canvas->save();
        _canvas->translate(dx, dy);
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::PushRotate(RotateParam param) -> void
{
    if (_canvas)
    {
        auto const r = static_cast<SkScalar>(param.degree);
        _canvas->save();
        _canvas->rotate(r);
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::PushScale(ScaleParam param) -> void
{
    if (_canvas)
    {
        auto const sx = static_cast<SkScalar>(param.x);
        auto const sy = static_cast<SkScalar>(param.y);
        _canvas->save();
        _canvas->scale(sx, sy);
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::Pop(PopParam param) -> void
{
    if (_canvas)
    {
        for (auto i = SInt64(0); i < param.count; ++i)
        {
            _canvas->restore();
        }
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::AddLine(LineParam param) -> void
{
    if (_canvas)
    {
        if (param.drawStyle == DrawStyle::Stroke && param.strokeWidth <= 0)
        {
            // Skip drawing hairline stroke.
            return;
        }
        auto const p1 = PointToSkPoint(param.p1);
        auto const p2 = PointToSkPoint(param.p2);
        auto const p = SceneParamToSkPaint(param);
        _canvas->drawLine(p1, p2, p);
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::AddRect(RectParam param) -> void
{
    if (_canvas)
    {
        if (param.drawStyle == DrawStyle::Stroke && param.strokeWidth <= 0)
        {
            // Skip drawing hairline stroke.
            return;
        }
        auto const r = RectToSkRect(param.rect);
        auto const p = SceneParamToSkPaint(param);
        _canvas->drawRect(r, p);
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::AddRoundRect(RoundRectParam param) -> void
{
    if (_canvas)
    {
        if (param.drawStyle == DrawStyle::Stroke && param.strokeWidth <= 0)
        {
            // Skip drawing hairline stroke.
            return;
        }
        auto const rr = RoundRectToSkRRect(param.roundRect);
        auto const p = SceneParamToSkPaint(param);
        _canvas->drawRRect(rr, p);
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::AddGlyphRun(GlyphRunParam param) -> void
{
    if (auto const skiaGlyphRun = param.run.TryAs<SkiaGlyphRun>())
    {
        skiaGlyphRun->Draw(static_cast<SkCanvas*>(_canvas), SceneParamToSkPaint(param));
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::AddDisplayList(DisplayListParam param) -> void
{
    if (auto const skiaDisplayList = param.displayList.TryAs<SkiaDisplayList>())
    {
        if (auto const picture = skiaDisplayList->GetPicture())
        {
            if (_canvas)
            {
                auto const paint = SceneParamToSkPaint(param);
                auto const matrix = SkMatrix();
                _canvas->drawPicture(picture, &matrix, &paint);
            }
        }
    }
}
}
