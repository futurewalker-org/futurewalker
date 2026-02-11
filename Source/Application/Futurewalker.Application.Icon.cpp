// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Icon.hpp"

#include "Futurewalker.Graphics.FontManager.hpp"
#include "Futurewalker.Graphics.Typeface.hpp"
#include "Futurewalker.Graphics.TextShaper.hpp"
#include "Futurewalker.Graphics.GlyphRun.hpp"
#include "Futurewalker.Graphics.ShapedText.hpp"
#include "Futurewalker.Graphics.Scene.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
class Icon::Impl : NonCopyable
{
public:
    virtual ~Impl() = default;
    virtual auto Draw(Graphics::Scene& scene,  Rect<Dp> const& rect, RGBAColor const& color) const -> void = 0;
};

///
/// @brief Create non-empty but blank icon.
///
auto Icon::MakeBlank() -> Icon
{
    struct IconBlank final : public Icon::Impl
    {
        auto Draw(Graphics::Scene&, Rect<Dp> const&, RGBAColor const&) const -> void override
        {
        }
    };
    return Icon(Shared<IconBlank>::Make());
}

///
/// @brief Create icon from font.
///
/// @param family Font family.
/// @param codePoint Unicode code point.
///
auto Icon::MakeFromFont(Graphics::FontFamily const& family, char32_t const& codePoint) -> Icon
{
    struct FontIconDrawable final : public Icon::Impl
    {
        FontIconDrawable(Shared<Graphics::Typeface> typeface, char32_t codePoint)
          : _typeface(typeface)
          , _codePoint(codePoint)
        {
        }

        auto GetGlyphSize(Graphics::ShapedText const& glyph) const -> Size<Dp>
        {
            if (glyph.GetLineCount() > 0)
            {
                auto const line = glyph.GetLines()[0];
                if (line.GetRunCount() > 0)
                {
                    if (auto const run = line.GetRuns()[0])
                    {
                        auto const runMetrics = run->GetMetrics();
                        auto const width = line.GetAdvance();
                        auto const height = runMetrics.ascent + runMetrics.descent + runMetrics.leading;
                        return {width, height};
                    }
                }
            }
            return {};
        }

        auto DrawGlyph(Graphics::Scene& scene, Graphics::ShapedText const& glyph, RGBAColor const& color) const -> void
        {
            if (glyph.GetLineCount() > 0)
            {
                auto const line = glyph.GetLines()[0];
                if (line.GetRunCount() > 0)
                {
                    if (auto const run = line.GetRuns()[0])
                    {
                        scene.AddGlyphRun({.run = run, .color = color});
                    }
                }
            }
        }

        auto Draw(Graphics::Scene& scene, Rect<Dp> const& rect, RGBAColor const& color) const -> void override
        {
            scene.PushTranslate({.x = rect.GetLeft(), .y = rect.GetTop()});
            {
                auto const shaper = Graphics::TextShaper::Make();
                auto const shapedGlyph = shaper->ShapeGlyph(_codePoint, _typeface, 16);
                auto const size = GetGlyphSize(shapedGlyph);
                auto const glyphAspectRatio = Float64(size.GetWidth() / size.GetHeight());
                auto const sizeAspectRatio = Float64(rect.GetWidth() / rect.GetHeight());
                if (glyphAspectRatio > sizeAspectRatio)
                {
                    auto const scale = Float64(rect.GetWidth() / size.GetWidth());
                    auto const offset = (rect.GetHeight() - size.GetHeight() * Dp(scale)) / 2.0;
                    scene.PushTranslate({.x = 0, .y = offset});
                    scene.PushScale({.x = scale, .y = scale});
                    DrawGlyph(scene, shapedGlyph, color);
                    scene.Pop({.count = 2});
                }
                else
                {
                    auto const scale = Float64(rect.GetHeight() / size.GetHeight());
                    auto const offset = (rect.GetWidth() - size.GetWidth() * Dp(scale)) / 2.0;
                    scene.PushTranslate({.x = offset, .y = 0});
                    scene.PushScale({.x = scale, .y = scale});
                    DrawGlyph(scene, shapedGlyph, color);
                    scene.Pop({.count = 2});
                }
            }
            scene.Pop({});
        }

        Shared<Graphics::Typeface> _typeface;
        char32_t _codePoint = 0;
    };

    if (auto const fontManager = Locator::GetInstance<Graphics::FontManager>())
    {
        if (auto const typeface = fontManager->FindTypefaceByFamilyAndStyle(family, Graphics::FontStyle(Graphics::FontWeight::Normal(), Graphics::FontWidth::Normal(), Graphics::FontSlant::Upright)))
        {
            return Icon(Shared<FontIconDrawable>::Make(typeface, codePoint));
        }
    }
    return Icon(nullptr);
}

///
/// @brief Draw icon to scene.
///
/// @param scene Scene to draw.
/// @param rect Drawing rectangle.
/// @param color Color of the icon.
/// @param alpha Alpha channel of the icon.
///
auto Icon::Draw(Graphics::Scene& scene, Rect<Dp> const& rect, RGBAColor const& color, Channel const& alpha) const -> void
{
    if (_impl)
    {
        auto const drawColor = RGBAColor(color.GetRGBColor(), color.GetAlpha().GetF64() * alpha.GetF64());
        _impl->Draw(scene, rect, drawColor);
    }
}

///
/// @brief Check if the icon is empty.
///
auto Icon::IsEmpty() const -> Bool
{
    return !_impl;
}

///
/// @brief Constructor.
///
Icon::Icon(Shared<Impl const> const& impl)
  : _impl {impl}
{
}
}
