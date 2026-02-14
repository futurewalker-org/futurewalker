// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ViewLayoutFunction.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp"

#include "Futurewalker.Attribute.AttributeObserver.hpp"

#include "Futurewalker.Graphics.FontManager.hpp"
#include "Futurewalker.Graphics.TextShaper.hpp"
#include "Futurewalker.Graphics.ShapedText.hpp"
#include "Futurewalker.Graphics.GlyphRun.hpp"
#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.FontFamily.hpp"

#include "Futurewalker.Text.LineBreakIterator.hpp"

#include "Futurewalker.Core.UnicodeFunction.hpp"

namespace FW_DETAIL_NS
{
namespace
{
///
/// @brief Trim end of text range that consists of break and space characters, and return substring of the trimmed range.
///
/// @param[in] text The text to trim.
/// @param[in] begin The index of the beginning of the text range to trim.
/// @param[in] end The index of the end of the text range to trim.
///
/// @return The substring of the trimmed text range.
///
auto StripTrailingBreakAndSpace(String const& text, String::IndexType const begin, String::IndexType const end) -> String
{
    auto idx = text.GetPrev(end);
    while (begin < idx)
    {
        auto c = String::CharType();
        if (text.GetChar(idx, c))
        {
            if (!UnicodeFunction::IsSpace(c) && !UnicodeFunction::IsBreak(c))
            {
                return text.GetSubString(begin, text.GetNext(idx));
            }
        }
        idx = text.GetPrev(idx);
    }
    return String();
}
}

///
/// @brief Create a new TextView instance.
///
auto TextView::Make() -> Shared<TextView>
{
    return View::MakeDerived<TextView>();
}

///
/// @brief Create a new TextView instance with specified text.
///
/// @param text
///
auto TextView::MakeWithText(AttributeArg<String> const& text) -> Shared<TextView>
{
    auto textView = TextView::Make();
    textView->SetText(text);
    return textView;
}

///
/// @brief Get text.
///
auto TextView::GetText() const -> String
{
    return _text.GetValueOrDefault();
}

///
/// @brief Set text.
///
/// @param text The text to set.
///
void TextView::SetText(AttributeArg<String> const& text)
{
    _text.SetAttributeArg(text);
}

///
/// @brief
///
/// @param color
///
auto TextView::SetColor(AttributeArg<RGBAColor> const& color) -> void
{
    _color.SetAttributeArg(color);
}

///
/// @brief 
///
/// @param alpha 
///
auto TextView::SetAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _alpha.SetAttributeArg(alpha);
}

///
/// @brief
///
/// @param size
///
auto TextView::SetFontSize(AttributeArg<Graphics::FontSize> const& size) -> void
{
    _fontSize.SetAttributeArg(size);
}

///
/// @brief
///
/// @param weight
///
auto TextView::SetFontWeight(AttributeArg<Graphics::FontWeight> const& weight) -> void
{
    _fontWeight.SetAttributeArg(weight);
}

///
/// @brief
///
/// @param width
///
auto TextView::SetFontWidth(AttributeArg<Graphics::FontWidth> const& width) -> void
{
    _fontWidth.SetAttributeArg(width);
}

///
/// @brief
///
/// @param slant
///
auto TextView::SetFontSlant(AttributeArg<Graphics::FontSlant> const& slant) -> void
{
    _fontSlant.SetAttributeArg(slant);
}

///
/// @brief
///
/// @param family
///
auto TextView::SetFontFamily(AttributeArg<Graphics::FontFamily> const& family) -> void
{
    _fontFamily.SetAttributeArg(family);
}

///
/// @brief
///
/// @param alignment
///
auto TextView::SetHorizontalAlignment(AttributeArg<TextViewHorizontalAlignment> const& alignment) -> void
{
    _horizontalAlignment.SetAttributeArg(alignment);
}

///
/// @brief
///
/// @param alignment
///
auto TextView::SetVerticalAlignment(AttributeArg<TextViewVerticalAlignment> const& alignment) -> void
{
    _verticalAlignment.SetAttributeArg(alignment);
}

///
/// @brief Constructor.
///
TextView::TextView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Initialize.
///
auto TextView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(String, AttributeText, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontSize, AttributeFontSize, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontWeight, AttributeFontWeight, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontWidth, AttributeFontWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontSlant, AttributeFontSlant, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontFamily, AttributeFontFamily, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewHorizontalAlignment, AttributeHorizontalAlignment, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewVerticalAlignment, AttributeVerticalAlignment, {});

    _shaper = Graphics::TextShaper::Make();

    _text.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeText, {});
    _color.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeColor, {});
    _alpha.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeAlpha, {});
    _fontSize.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeFontSize, {0});
    _fontWeight.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeFontWeight, {0});
    _fontWidth.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeFontWidth, {0});
    _fontSlant.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeFontSlant, {});
    _fontFamily.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeFontFamily, {});
    _horizontalAlignment.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeHorizontalAlignment, {TextViewHorizontalAlignment::Center});
    _verticalAlignment.BindAndConnectAttributeWithDefaultValue(*this, &TextView::ReceiveAttributeEvent, AttributeVerticalAlignment, {TextViewVerticalAlignment::Middle});
}

///
/// @brief Measure.
///
auto TextView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const& widthConstraints = parameter.GetWidthConstraints();
    auto const& heightConstraints = parameter.GetHeightConstraints();

    UpdateFontCache();
    UpdateLayoutCache(widthConstraints.GetMax());

    auto const textSize = GetShapedTextSize();
    auto const measuredWidth = AxisConstraints::Constrain(widthConstraints, textSize.GetWidth());
    auto const measuredHeight = AxisConstraints::Constrain(heightConstraints, textSize.GetHeight());
    scope.SetMeasuredSize(measuredWidth, measuredHeight);
}

///
/// @brief Draw.
///
auto TextView::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    auto const rect = GetContentRect();

    auto const color = GetTextColor();
    auto const isRTL = GetLayoutDirection() == LayoutDirection::RightToLeft;

    const auto hAlign = _horizontalAlignment.GetValueOr(TextViewHorizontalAlignment::Center);
    const auto vAlign = _verticalAlignment.GetValueOr(TextViewVerticalAlignment::Middle);

    auto const textSize = GetShapedTextSize();

    auto y = Dp(0);
    if (vAlign == TextViewVerticalAlignment::Bottom)
    {
        y = rect.GetTop() + rect.GetHeight() - textSize.GetHeight();
    }
    else if (vAlign == TextViewVerticalAlignment::Middle)
    {
        y = rect.GetTop() + (rect.GetHeight() - textSize.GetHeight()) / 2;
    }

    for (auto const& shapedText : _shapedTexts)
    {
        for (auto const& line : shapedText.GetLines())
        {
            if (line.GetRunCount() == 0)
            {
                if (auto const typeface = GetTypeface())
                {
                    auto const metrics = typeface->GetMetrics(GetFontSize());
                    y += metrics.ascent + metrics.descent + metrics.leading;
                }
                continue;
            }

            auto const lineWidth = line.GetAdvance();
            auto const lineMetrics = line.GetMetrics();

            auto x = Dp(0);
            if ((hAlign == TextViewHorizontalAlignment::Leading && isRTL) || (hAlign == TextViewHorizontalAlignment::Trailing && !isRTL))
            {
                x = rect.GetLeft() + rect.GetWidth() - lineWidth;
            }
            else if (hAlign == TextViewHorizontalAlignment::Center)
            {
                x = rect.GetLeft() + (rect.GetWidth() - lineWidth) / 2;
            }

            auto const alignedX = ViewLayoutFunction::AlignToPixelGridByRound(x, *this);
            auto const alignedY = ViewLayoutFunction::AlignToPixelGridByRound(y + lineMetrics.leading / 2, *this);

            scene.PushTranslate({
                .x = alignedX,
                .y = alignedY,
            });
            DrawTextLine(scene, line, color);
            scene.Pop({});

            y += lineMetrics.ascent + lineMetrics.descent + lineMetrics.leading;
        }
    }
}

///
/// @brief Receive attribute event.
///
auto TextView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateFontCache();
        InvalidateLayoutCache();
        InvalidateLayout();
        InvalidateVisual();
    }
    co_return false;
}

///
/// @brief Get typeface.
///
auto TextView::GetTypeface() const -> Shared<Graphics::Typeface>
{
    return _typeface;
}

///
/// @brief Get font size.
///
auto TextView::GetFontSize() const -> Graphics::FontSize
{
    return _fontSize.GetValueOr(16);
}

///
/// @brief Get font metrics.
///
auto TextView::GetFontMetrics() const -> Graphics::FontMetrics
{
    return _fontMetrics;
}

///
/// @brief Get current text color.
///
auto TextView::GetTextColor() const -> RGBAColor
{
    auto const color = _color.GetValueOrDefault();
    auto const alpha = _alpha.GetValueOrDefault();
    return RGBAColor(color.GetRed(), color.GetGreen(), color.GetBlue(), alpha.GetF64() * color.GetAlpha().GetF64());
}

///
/// @brief Invalidate layout cache.
///
auto TextView::InvalidateLayoutCache() -> void
{
    _shapedTexts.clear();
    _shapedTextsMaxWidth = Dp::Infinity();
    _shapedTextsIntrinsicWidth = Dp::Infinity();
    _shapedTextsWidth = 0;
    _shapedTextsHeight = 0;
}

///
/// @brief Update layout cache.
///
auto TextView::UpdateLayoutCache(Dp const maxWidth) -> void
{
    if (!_shaper)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    auto const matchesMaxWidth = Dp::IsNearlyEqual(maxWidth, _shapedTextsMaxWidth);
    auto const matchesWidth = Dp::IsNearlyEqual(GetShapedTextSize().GetWidth(), maxWidth);
    auto const belowIntrinsicWidth = _shapedTextsIntrinsicWidth < maxWidth;
    if (_shapedTexts.empty() || (!matchesMaxWidth && !matchesWidth && !belowIntrinsicWidth))
    {
        auto const typeface = GetTypeface();
        auto const fontSize = GetFontSize();
        auto const text = GetText();
        if (fontSize > 0 && !text.IsEmpty())
        {
            auto breakIterator = LineBreakIterator("en-US");
            breakIterator.SetText(text);

            auto shapedTexts = std::vector<Graphics::ShapedText>();
            auto shapedTextsIntrinsicWidth = Dp::Infinity();
            auto shapedTextsMaxWidth = Dp(0);
            auto shapedTextsWidth = Dp(0);
            auto shapedTextsHeight = Dp(0);

            auto prevIndex = breakIterator.GetCurrent();
            while (true)
            {
                if (auto nextIndex = breakIterator.GetNext())
                {
                    if (*nextIndex != text.GetEnd() && breakIterator.GetCurrentBreakCategory() != LineBreakIterator::BreakCategory::Hard)
                    {
                        continue;
                    }

                    // Break characters are not printable. Trim them before passing to the shaper.
                    // We also trim spaces for consistency with soft break.
                    auto const lineText = StripTrailingBreakAndSpace(text, prevIndex, *nextIndex);
                    auto const shapedLine = _shaper->ShapeText(Text(lineText), typeface, fontSize, maxWidth);
                    if (shapedLine.GetLineCount() == 1)
                    {
                        auto const lineWidth = shapedLine.GetLines()[0].GetAdvance();
                        if (Dp::IsFinite(shapedTextsIntrinsicWidth))
                        {
                            shapedTextsIntrinsicWidth = Dp::Max(shapedTextsIntrinsicWidth, lineWidth);
                        }
                        else
                        {
                            shapedTextsIntrinsicWidth = lineWidth;
                        }
                    }

                    for (auto const& line : shapedLine.GetLines())
                    {
                        if (line.GetRunCount() == 0)
                        {
                            auto const fontMetrics = GetFontMetrics();
                            shapedTextsHeight += fontMetrics.ascent + fontMetrics.descent + fontMetrics.leading;
                            continue;
                        }
                        auto const advance = line.GetAdvance();
                        auto const metrics = line.GetMetrics();
                        shapedTextsMaxWidth = Dp::Max(shapedTextsMaxWidth, advance);
                        shapedTextsWidth = Dp::Max(shapedTextsWidth, advance);
                        shapedTextsHeight += metrics.ascent + metrics.descent + metrics.leading;
                    }
                    shapedTexts.push_back(std::move(shapedLine));
                    prevIndex = *nextIndex;
                    continue;
                }
                break;
            }
            _shapedTexts = std::move(shapedTexts);
            _shapedTextsIntrinsicWidth = shapedTextsIntrinsicWidth;
            _shapedTextsMaxWidth = shapedTextsMaxWidth;
            _shapedTextsWidth = shapedTextsWidth;
            _shapedTextsHeight = shapedTextsHeight;
        }
        else
        {
            InvalidateLayoutCache();
        }
    }
}

///
/// @brief Invalidate font cache.
///
auto TextView::InvalidateFontCache() -> void
{
    _typeface = nullptr;
    _fontMetrics = {};
}

///
/// @brief Update font cache.
///
auto TextView::UpdateFontCache() -> void
{
    if (!_typeface)
    {
        auto const fontWidth = _fontWidth.GetValueOr(Graphics::FontWidth::Normal());
        auto const fontWeight = _fontWeight.GetValueOr(Graphics::FontWeight::Normal());
        auto const fontSlant = _fontSlant.GetValueOrDefault();
        auto fontStyle = Graphics::FontStyle(fontWeight, fontWidth, fontSlant);
        if (auto const fontManager = Locator::GetInstance<Graphics::FontManager>())
        {
            _typeface = fontManager->FindTypefaceByFamilyAndStyle(_fontFamily.GetValueOrDefault(), fontStyle);
        }

        auto const fontSize = GetFontSize();
        if (_typeface)
        {
            _fontMetrics = _typeface->GetMetrics(fontSize);
        }
        else
        {
            // TODO: Better fallback.
            _fontMetrics = Graphics::FontMetrics {
                .ascent = static_cast<Dp>(fontSize),
            };
        }
    }
}

///
/// @brief Get sized of cached text.
///
auto TextView::GetShapedTextSize() const -> Size<Dp>
{
    return {_shapedTextsWidth, _shapedTextsHeight};
}

///
/// @brief Draw cached text line.
///
auto TextView::DrawTextLine(Graphics::Scene& scene, Graphics::ShapedText::Line const& line, RGBAColor const& color) -> void
{
    auto const lineMetrics = line.GetMetrics();

    auto offset = Dp(0);
    for (auto const& run : line.GetRuns())
    {
        auto const runMetrics = run->GetMetrics();

        scene.PushTranslate({
            .x = offset,
            .y = lineMetrics.ascent - runMetrics.ascent,
        });
        scene.AddGlyphRun({
            .run = run,
            .color = color,
        });
        scene.Pop({});

        // TODO: support RTL layout.
        offset += run->GetAdvance();
    }
}
}
