// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Icon.hpp"

#include "Futurewalker.Graphics.FontManager.hpp"
#include "Futurewalker.Graphics.Typeface.hpp"
#include "Futurewalker.Graphics.TextShaper.hpp"
#include "Futurewalker.Graphics.GlyphRun.hpp"
#include "Futurewalker.Graphics.ShapedText.hpp"
#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.SvgDocument.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
class Icon::Impl : NonCopyable
{
public:
    virtual ~Impl() = default;
    virtual auto Draw(Graphics::Scene& scene,  Size<Dp> const& size) const -> void = 0;
    virtual auto GetIntrinsicSize() const -> Size<Dp> = 0;
};

///
/// @brief Create non-empty but blank icon.
///
/// @param[in] intrinsicSize The intrinsic size of the icon.
///
auto Icon::MakeBlank(Size<Dp> const& intrinsicSize) -> Icon
{
    struct IconBlank final : public Icon::Impl
    {
        IconBlank(Size<Dp> const& size)
          : _size(size)
        {
        }

        auto Draw(Graphics::Scene& scene, Size<Dp> const& size) const -> void override
        {
            (void)scene;
            (void)size;
        }

        auto GetIntrinsicSize() const -> Size<Dp> override
        {
            return _size;
        }

        Size<Dp> _size;
    };
    return Icon(Shared<IconBlank>::Make(intrinsicSize));
}

///
/// @brief Create icon from font.
///
/// @param[in] family Font family.
/// @param[in] codePoint Unicode code point.
/// @param[in] size Font size.
///
auto Icon::MakeFromFont(Graphics::FontFamily const& family, char32_t const& codePoint, Graphics::FontSize const& size) -> Icon
{
    struct FontIconDrawable final : public Icon::Impl
    {
        static auto ShapeGlyph(Shared<Graphics::Typeface> const& typeface, char32_t const codePoint, Graphics::FontSize const& size) -> Graphics::ShapedText
        {
            auto const shaper = Graphics::TextShaper::Make();
            return shaper->ShapeGlyph(codePoint, typeface, size, Graphics::FontSmoothing::AntiAlias, {'L', 'a', 't', 'n'}, Graphics::TextShaper::Direction::DefaultLtr);
        }

        FontIconDrawable(Shared<Graphics::Typeface> const& typeface, char32_t const codePoint, Graphics::FontSize const& size)
          : _shapedGlyph(ShapeGlyph(typeface, codePoint, size))
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

        auto Draw(Graphics::Scene& scene, Size<Dp> const& size) const -> void override
        {
            auto const color = RGBAColor(0, 0, 0, 1);
            auto const glyphSize = GetGlyphSize(_shapedGlyph);
            auto const glyphAspectRatio = Float64(glyphSize.width / glyphSize.height);
            auto const sizeAspectRatio = Float64(size.width / size.height);
            if (glyphAspectRatio > sizeAspectRatio)
            {
                auto const scale = Float64(size.width / glyphSize.width);
                auto const offset = (size.height - glyphSize.height * Dp(scale)) / 2.0;
                scene.PushTranslate({.x = 0, .y = offset});
                scene.PushScale({.x = scale, .y = scale});
                DrawGlyph(scene, _shapedGlyph, color);
                scene.Pop({.count = 2});
            }
            else
            {
                auto const scale = Float64(size.height / glyphSize.height);
                auto const offset = (size.width - glyphSize.width * Dp(scale)) / 2.0;
                scene.PushTranslate({.x = offset, .y = 0});
                scene.PushScale({.x = scale, .y = scale});
                DrawGlyph(scene, _shapedGlyph, color);
                scene.Pop({.count = 2});
            }
        }

        auto GetIntrinsicSize() const -> Size<Dp> override
        {
            return GetGlyphSize(_shapedGlyph);
        }

        Graphics::ShapedText _shapedGlyph;
    };

    if (auto const fontManager = Locator::GetInstance<Graphics::FontManager>())
    {
        if (auto const typeface = fontManager->FindTypefaceByFamilyAndStyle(family, Graphics::FontStyle(Graphics::FontWeight::Normal(), Graphics::FontWidth::Normal(), Graphics::FontSlant::Upright)))
        {
            return Icon(Shared<FontIconDrawable>::Make(typeface, codePoint, size));
        }
    }
    return Icon(nullptr);
}

///
/// @brief Create icon from SVG document.
///
/// @param[in] svgDocument SVG document.
///
auto Icon::MakeFromSvgDocument(Unique<Graphics::SvgDocument> svgDocument) -> Icon
{
    struct SvgIcon final : public Icon::Impl
    {
        SvgIcon(Unique<Graphics::SvgDocument>&& svgDocument)
          : _svgDocument(std::move(svgDocument))
        {
        }

        auto Draw(Graphics::Scene& scene, Size<Dp> const& size) const -> void override
        {
            auto const svgSize = GetIntrinsicSize();
            auto const svgAspectRatio = Float64(svgSize.width / svgSize.height);
            auto const sizeAspectRatio = Float64(size.width / size.height);

            if (svgAspectRatio > sizeAspectRatio)
            {
                auto const scale = Float64(size.width / svgSize.width);
                auto const offset = (size.height - svgSize.height * Dp(scale)) / 2.0;
                scene.PushTranslate({.x = 0, .y = offset});
                scene.PushScale({.x = scale, .y = scale});
                if (_svgDocument)
                {
                    _svgDocument->Draw(scene, Size<Dp>(svgSize.width, svgSize.height));
                }
                scene.Pop({.count = 2});
            }
            else
            {
                auto const scale = Float64(size.height / svgSize.height);
                auto const offset = (size.width - svgSize.width * Dp(scale)) / 2.0;
                scene.PushTranslate({.x = offset, .y = 0});
                scene.PushScale({.x = scale, .y = scale});
                if (_svgDocument)
                {
                    _svgDocument->Draw(scene, Size<Dp>(svgSize.width, svgSize.height));
                }
                scene.Pop({.count = 2});
            }
        }

        auto GetIntrinsicSize() const -> Size<Dp> override
        {
            if (_svgDocument)
            {
                return _svgDocument->GetIntrinsicSize({});
            }
            return {};
        }
        Unique<Graphics::SvgDocument> _svgDocument;
    };
    return Icon(Shared<SvgIcon>::Make(std::move(svgDocument)));
}

///
/// @brief Draw icon to scene.
///
/// @param scene Scene to draw.
/// @param size Drawing rectangle.
///
auto Icon::Draw(Graphics::Scene& scene, Size<Dp> const& size) const -> void
{
    if (_impl)
    {
        _impl->Draw(scene, size);
    }
}

///
/// @brief Get the intrinsic size of the icon.
///
/// @return Intrinsic size of the icon.
///
auto Icon::GetIntrinsicSize() const -> Size<Dp>
{
    if (_impl)
    {
        return _impl->GetIntrinsicSize();
    }
    return {};
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
