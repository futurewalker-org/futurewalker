// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TextViewType.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"

#include "Futurewalker.Graphics.SceneType.hpp"
#include "Futurewalker.Graphics.ShapedText.hpp"
#include "Futurewalker.Graphics.TextShaperType.hpp"
#include "Futurewalker.Graphics.TypefaceType.hpp"
#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.FontWeight.hpp"
#include "Futurewalker.Graphics.FontWidth.hpp"
#include "Futurewalker.Graphics.FontStyleType.hpp"
#include "Futurewalker.Graphics.FontFamilyType.hpp"

#include "Futurewalker.Core.Optional.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Text view.
///
class TextView final : public View
{
public:
    static auto Make() -> Shared<TextView>;
    static auto MakeWithText(AttributeArg<String> const& text) -> Shared<TextView>;

    auto GetText() const -> String;
    auto SetText(AttributeArg<String> const& text) -> void;

    auto SetColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetFontSize(AttributeArg<Graphics::FontSize> const& size) -> void;
    auto SetFontWeight(AttributeArg<Graphics::FontWeight> const& weight) -> void;
    auto SetFontWidth(AttributeArg<Graphics::FontWidth> const& width) -> void;
    auto SetFontSlant(AttributeArg<Graphics::FontSlant> const& slant) -> void;
    auto SetFontFamily(AttributeArg<Graphics::FontFamily> const& family) -> void;
    auto SetHorizontalAlignment(AttributeArg<TextViewHorizontalAlignment> const& alignment) -> void;
    auto SetVerticalAlignment(AttributeArg<TextViewVerticalAlignment> const& alignment) -> void;

    TextView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Draw(DrawScope& scope) -> void override;

private:
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;
    auto GetTypeface() const -> Shared<Graphics::Typeface>;
    auto GetFontSize() const -> Graphics::FontSize;
    auto GetTextColor() const -> RGBAColor;
    auto InvalidateLayoutCache() -> void;
    auto UpdateLayoutCache(Dp const maxWidth) -> void;
    auto MeasureText(Dp const maxWidth) -> Size<Dp>;
    auto GetCachedTextSize() const -> Size<Dp>;
    auto GetCachedTextLineCount() const -> SInt64;
    auto GetCachedTextLineSize(SInt64 const index) const -> Size<Dp>;
    auto DrawCachedTextLine(Graphics::Scene& scene, SInt64 const index, RGBAColor const& color) -> void;

private:
    AttributeAccessor<String> _text;
    AttributeAccessor<RGBAColor> _color;
    AttributeAccessor<Channel> _alpha;
    AttributeAccessor<Graphics::FontSize> _fontSize;
    AttributeAccessor<Graphics::FontWeight> _fontWeight;
    AttributeAccessor<Graphics::FontWidth> _fontWidth;
    AttributeAccessor<Graphics::FontSlant> _fontSlant;
    AttributeAccessor<Graphics::FontFamily> _fontFamily;
    AttributeAccessor<TextViewHorizontalAlignment> _horizontalAlignment;
    AttributeAccessor<TextViewVerticalAlignment> _verticalAlignment;
    Shared<Graphics::TextShaper> _shaper;
    Optional<Graphics::ShapedText> _shapedText;
    Dp _shapedTextMaxWidth = Dp::Infinity();
};
}
}
