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

namespace FW_DETAIL_NS
{
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

    auto bindAndConnectAttribute = [this]<class Accessor>(Accessor& accessor, StaticAttributeRef<typename Accessor::ValueType> attribute, typename Accessor::ValueType const& value) {
        accessor.BindAttributeWithDefaultValue(*this, attribute, value);
        EventReceiver::Connect(accessor, *this, &TextView::ReceiveAttributeEvent);
    };
    bindAndConnectAttribute(_text, AttributeText, {});
    bindAndConnectAttribute(_color, AttributeColor, {});
    bindAndConnectAttribute(_alpha, AttributeAlpha, {});
    bindAndConnectAttribute(_fontSize, AttributeFontSize, {0});
    bindAndConnectAttribute(_fontWeight, AttributeFontWeight, {0});
    bindAndConnectAttribute(_fontWidth, AttributeFontWidth, {0});
    bindAndConnectAttribute(_fontSlant, AttributeFontSlant, {});
    bindAndConnectAttribute(_fontFamily, AttributeFontFamily, {});
    bindAndConnectAttribute(_horizontalAlignment, AttributeHorizontalAlignment, {TextViewHorizontalAlignment::Center});
    bindAndConnectAttribute(_verticalAlignment, AttributeVerticalAlignment, {TextViewVerticalAlignment::Middle});
}

///
/// @brief Measure.
///
auto TextView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const& widthConstraints = parameter.GetWidthConstraints();
    auto const& heightConstraints = parameter.GetHeightConstraints();

    auto const textSize = MeasureText(widthConstraints.GetMax());
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

    auto const textSize = GetCachedTextSize();

    auto y = Dp(0);
    if (vAlign == TextViewVerticalAlignment::Bottom)
    {
        y = rect.GetTop() + rect.GetHeight() - textSize.GetHeight();
    }
    else if (vAlign == TextViewVerticalAlignment::Middle)
    {
        y = rect.GetTop() + (rect.GetHeight() - textSize.GetHeight()) / 2;
    }

    for (auto i = SInt64(0); i < GetCachedTextLineCount(); ++i)
    {
        auto const lineSize = GetCachedTextLineSize(i);

        auto x = Dp(0);
        if ((hAlign == TextViewHorizontalAlignment::Leading && isRTL) || (hAlign == TextViewHorizontalAlignment::Trailing && !isRTL))
        {
            x = rect.GetLeft() + rect.GetWidth() - lineSize.GetWidth();
        }
        else if (hAlign == TextViewHorizontalAlignment::Center)
        {
            x = rect.GetLeft() + (rect.GetWidth() - lineSize.GetWidth()) / 2;
        }

        auto const alignedX = ViewLayoutFunction::AlignToPixelGridByRound(x, *this);
        auto const alignedY = ViewLayoutFunction::AlignToPixelGridByRound(y, *this);

        scene.PushTranslate({
            .x = alignedX,
            .y = alignedY,
        });
        DrawCachedTextLine(scene, i, color);
        scene.Pop({});

        y += lineSize.GetHeight();
    }
}

///
/// @brief Receive attribute event.
///
auto TextView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
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
    if (auto manager = Locator::GetInstance<Graphics::FontManager>())
    {
        auto const fontFamily = _fontFamily.GetValueOrDefault();
        auto const fontWeight = _fontWeight.GetValueOr(Graphics::FontWeight::Normal());
        auto const fontWidth = _fontWidth.GetValueOr(Graphics::FontWidth::Normal());
        auto const fontSlant = _fontSlant.GetValueOr(Graphics::FontSlant::Upright);
        return manager->FindTypefaceByFamilyAndStyle(fontFamily, Graphics::FontStyle(fontWeight, fontWidth, fontSlant));
    }
    return {};
}

///
/// @brief Get font size.
///
auto TextView::GetFontSize() const -> Graphics::FontSize
{
    return _fontSize.GetValueOr(16);
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
    _shapedText.Reset();
    _shapedTextMaxWidth = Dp::Infinity();
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

    auto const matchesMaxWidth = Dp::IsNearlyEqual(maxWidth, _shapedTextMaxWidth);
    auto const matchesWidth = Dp::IsNearlyEqual(GetCachedTextSize().GetWidth(), maxWidth);
    if (!_shapedText || (!matchesMaxWidth && !matchesWidth))
    {
        auto const size = GetFontSize();
        auto const text = GetText();
        if (size > 0 && !text.IsEmpty())
        {
            _shapedText = _shaper->ShapeText(Text(text), GetTypeface(), size, maxWidth);
            _shapedTextMaxWidth = maxWidth;
        }
    }
}

///
/// @brief Measure text and return its size.
///
auto TextView::MeasureText(Dp const maxWidth) -> Size<Dp>
{
    UpdateLayoutCache(maxWidth);
    return GetCachedTextSize();
}

///
/// @brief Get sized of cached text.
///
auto TextView::GetCachedTextSize() const -> Size<Dp>
{
    if (_shapedText)
    {
        auto width = Dp(0);
        auto height = Dp(0);
        for (auto const& line : _shapedText->GetLines())
        {
            auto const metrics = line.GetMetrics();
            auto const lineWidth = line.GetAdvance();
            auto const lineHeight = metrics.GetAscent() + metrics.GetDescent() + metrics.GetLeading();
            width = Dp::Max(width, lineWidth);
            height += lineHeight;
        }
        return {width, height};
    }
    return {};
}

///
/// @brief Get number of lines in cache.
///
auto TextView::GetCachedTextLineCount() const -> SInt64
{
    if (_shapedText)
    {
        return _shapedText->GetLineCount();
    }
    return 0;
}

///
/// @brief Get size of cached text line.
///
auto TextView::GetCachedTextLineSize(SInt64 const index) const -> Size<Dp>
{
    if (0 <= index && index < GetCachedTextLineCount())
    {
        auto const& line = _shapedText->GetLines()[static_cast<size_t>(index)];
        auto const metrics = line.GetMetrics();
        auto const lineWidth = line.GetAdvance();
        auto const lineHeight = metrics.GetAscent() + metrics.GetDescent() + metrics.GetLeading();
        return {lineWidth, lineHeight};
    }
    return {};
}

///
/// @brief Draw cached text line.
///
auto TextView::DrawCachedTextLine(Graphics::Scene& scene, SInt64 const index, RGBAColor const& color) -> void
{
    if (0 <= index && index < GetCachedTextLineCount())
    {
        auto const line = _shapedText->GetLines()[static_cast<size_t>(index)];
        auto const lineMetrics = line.GetMetrics();

        Dp offset = 0;
        for (auto const& run : line.GetRuns())
        {
            auto const runMetrics = run->GetMetrics();

            scene.PushTranslate({
                .x = offset,
                .y = lineMetrics.GetAscent() - runMetrics.GetAscent(),
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
}
