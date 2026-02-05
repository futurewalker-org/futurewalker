// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaScene.hpp"
#include "Futurewalker.Graphics.SkiaDisplayList.hpp"
#include "Futurewalker.Graphics.SkiaGlyphRun.hpp"
#include "Futurewalker.Graphics.SkiaPathData.hpp"
#include "Futurewalker.Graphics.SkiaFunction.hpp"
#include "Futurewalker.Graphics.SkiaMaskFilter.hpp"

#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace
{
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
    auto p = SkPaint();

    if constexpr (requires { param.color; })
    {
        p.setColor4f(SkiaFunction::RGBAColorToSkColor4f(param.color));
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

    if constexpr (requires { param.maskFilter; })
    {
        if (auto const maskFilter = param.maskFilter.template TryAs<SkiaMaskFilter>())
        {
            p.setMaskFilter(maskFilter->GetSkiaMaskFilter());
        }
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
        auto const r = SkiaFunction::RectToSkRect(param.rect);
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
        auto const rr = SkiaFunction::RoundRectToSkRRect(param.roundRect);
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
auto SkiaScene::PushClipPath(ClipPathParam param) -> void
{
    if (_canvas)
    {
        auto skPath = SkPath();
        auto aa = false;
        if (auto const skiaPathData = param.path.GetData().TryAs<SkiaPathData const>())
        {
            skPath = skiaPathData->GetSkPath();
            aa = static_cast<bool>(param.antiAlias);
        }
        _canvas->save();
        _canvas->clipPath(skPath, aa);
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
        auto const p1 = SkiaFunction::PointToSkPoint(param.p1);
        auto const p2 = SkiaFunction::PointToSkPoint(param.p2);
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
        auto const r = SkiaFunction::RectToSkRect(param.rect);
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
        auto const rr = SkiaFunction::RoundRectToSkRRect(param.roundRect);
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
                // NOTE: Passing non-null paint will create temporary layer.
                _canvas->drawPicture(picture, nullptr, nullptr);
            }
        }
    }
}

///
/// @brief
///
/// @param param
///
auto SkiaScene::AddPath(PathParam param) -> void
{
    if (auto const skiaPathData = param.path.GetData().TryAs<SkiaPathData const>())
    {
        if (_canvas)
        {
            auto const p = SceneParamToSkPaint(param);
            _canvas->drawPath(skiaPathData->GetSkPath(), p);
        }
    }
}
}
