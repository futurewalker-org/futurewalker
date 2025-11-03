// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TextEditType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.InputMethodType.hpp"
#include "Futurewalker.Application.InputMethodEditableType.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Graphics.TextShaperType.hpp"
#include "Futurewalker.Graphics.ShapedTextType.hpp"
#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.FontFamily.hpp"
#include "Futurewalker.Graphics.FontWidth.hpp"
#include "Futurewalker.Graphics.FontWeight.hpp"
#include "Futurewalker.Graphics.FontStyleType.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Range.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TextEdit : public View
{
public:
    static auto Make() -> Shared<TextEdit>;

    TextEdit(PassKey<View>);

    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

    auto SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetDisabledBackgroundColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetDisabledBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetTextColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetTextAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetDisabledTextColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetDisabledTextAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetDisabledBorderColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetDisabledBorderAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderWidth(AttributeArg<Dp> const& width) -> void;
    auto SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void;
    auto SetFontSize(AttributeArg<Graphics::FontSize> const& size) -> void;
    auto SetFontWeight(AttributeArg<Graphics::FontWeight> const& weight) -> void;
    auto SetFontWidth(AttributeArg<Graphics::FontWidth> const& width) -> void;
    auto SetFontSlant(AttributeArg<Graphics::FontSlant> const& slant) -> void;
    auto SetFontFamily(AttributeArg<Graphics::FontFamily> const& family) -> void;

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveInputEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveKeyEvent(Event<>& event) -> Async<Bool>;
    auto ReceivePointerEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveFocusEvent(Event<>& event) -> Async<Bool>;

private:
    auto InternalSetInputMethod(Shared<InputMethod> const& context) -> void;
    auto InternalInsertText(String const& text, CodePoint newSelection) -> void;
    auto InternalGetSelectedRange() -> Range<CodePoint>;
    auto InternalSetSelectedRange(Range<CodePoint> range) -> void;
    auto InternalGetComposingRange() -> Range<CodePoint>;
    auto InternalSetComposingRange(Range<CodePoint> range) -> void;
    auto InternalGetText() const -> String;
    auto InternalGetText(Range<CodePoint> range) -> String;
    auto InternalSetText(String const& text) -> void;
    auto InternalGetTextRange() const -> Range<CodePoint>;
    auto InternalDeleteBackward() -> void;
    auto InternalDeleteForward() -> void;
    auto InternalInvalidateLayoutCache() -> void;
    auto InternalUpdateLayoutCache() -> void;
    auto InternalGetBackgroundCololr() const -> RGBAColor;
    auto InternalGetTextColor() const -> RGBAColor;
    auto InternalGetBorderColor() const -> RGBAColor;

private:
    Shared<InputMethod> _inputMethod;
    Shared<InputMethodEditable> _inputMethodEditable;
    AttributeAccessor<RGBAColor> _backgroundColor;
    AttributeAccessor<Channel> _backgroundAlpha;
    AttributeAccessor<RGBAColor> _disabledBackgroundColor;
    AttributeAccessor<Channel> _disabledBackgroundAlpha;
    AttributeAccessor<RGBAColor> _textColor;
    AttributeAccessor<Channel> _textAlpha;
    AttributeAccessor<RGBAColor> _disabledTextColor;
    AttributeAccessor<Channel> _disabledTextAlpha;
    AttributeAccessor<RGBAColor> _borderColor;
    AttributeAccessor<Channel> _borderAlpha;
    AttributeAccessor<RGBAColor> _disabledBorderColor;
    AttributeAccessor<Channel> _disabledBorderAlpha;
    AttributeAccessor<Dp> _borderWidth;
    AttributeAccessor<CornerRadius> _cornerRadius;
    AttributeAccessor<Graphics::FontSize> _fontSize;
    AttributeAccessor<Graphics::FontWeight> _fontWeight;
    AttributeAccessor<Graphics::FontWidth> _fontWidth;
    AttributeAccessor<Graphics::FontSlant> _fontSlant;
    AttributeAccessor<Graphics::FontFamily> _fontFamily;
    Shared<Graphics::TextShaper> _textShaper;
    Shared<Graphics::ShapedText> _shapedText;
};
}
}
