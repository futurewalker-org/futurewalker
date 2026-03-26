// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TextEditType.hpp"
#include "Futurewalker.Application.TextEditEvent.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.InputEditableType.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"

#include "Futurewalker.Graphics.TextShaperType.hpp"
#include "Futurewalker.Graphics.GlyphRunType.hpp"
#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.FontFamily.hpp"
#include "Futurewalker.Graphics.FontWidth.hpp"
#include "Futurewalker.Graphics.FontWeight.hpp"
#include "Futurewalker.Graphics.FontStyleType.hpp"
#include "Futurewalker.Graphics.FontMetrics.hpp"
#include "Futurewalker.Graphics.Typeface.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Text.TextSelectionDirection.hpp"
#include "Futurewalker.Text.Text.hpp"

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
    auto SetTextColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetTextAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderWidth(AttributeArg<Dp> const& width) -> void;
    auto SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void;
    auto SetPadding(AttributeArg<EdgeInsets> const& padding) -> void;
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
    auto InternalInsertText(String const& text, CodePoint newSelection) -> void;
    auto InternalGetSelectionDirection() const -> TextSelectionDirection;
    auto InternalGetSelectionRange() -> Range<CodePoint>;
    auto InternalSetSelectionRange(Range<CodePoint> const& range, TextSelectionDirection direction) -> void;
    auto InternalGetComposingRange() -> Range<CodePoint>;
    auto InternalSetComposingRange(Range<CodePoint> const& range) -> void;
    auto InternalGetText() const -> String;
    auto InternalGetText(Range<CodePoint> range) -> String;
    auto InternalSetText(String const& text) -> void;
    auto InternalGetTextRange() const -> Range<CodePoint>;
    auto InternalDeleteBackward() -> void;
    auto InternalDeleteForward() -> void;
    auto InternalInvalidateTextCache() -> void;
    auto InternalUpdateTextCache() -> void;
    auto InternalInvalidateMeasureCache() -> void;
    auto InternalUpdateMeasureCache() -> void;
    auto InternalInvalidateArrangeCache() -> void;
    auto InternalUpdateArrangeCache() -> void;
    auto InternalInvalidateTypeface() -> void;
    auto InternalUpdateTypeface() -> void;
    auto InternalGetBackgroundColor() const -> RGBAColor;
    auto InternalGetTextColor() const -> RGBAColor;
    auto InternalGetBorderColor() const -> RGBAColor;
    auto InternalGetTypeface() const -> Shared<Graphics::Typeface>;

private:
    struct ShapedLine
    {
        CodePoint start;
        Dp advance;
        Graphics::FontMetrics metrics;
        std::vector<Shared<Graphics::GlyphRun>> runs;
    };

    struct ArrangedRun
    {
        Point<Dp> position;
        Shared<Graphics::GlyphRun> run;
    };

    struct ArrangedLine
    {
        Point<Dp> position;
        CodePoint start;
        Graphics::FontMetrics metrics;
        std::vector<ArrangedRun> runs;
    };

    struct SelectionPosition
    {
        SInt64 line = 0;
        CodePoint offset = 0;
    };

    struct TextCache
    {
        Text text;
        std::vector<std::pair<CodeUnit, String>> lines;
    };

    struct MeasureCache
    {
        std::vector<ShapedLine> shapedLines;
    };

    struct ArrangeCache
    {
        std::vector<ArrangedLine> arrangedLines;
    };

    SelectionPosition _selectionBegin;
    SelectionPosition _selectionEnd;
    Shared<InputEditable> _inputEditable;
    AttributeAccessor<RGBAColor> _backgroundColor;
    AttributeAccessor<Channel> _backgroundAlpha;
    AttributeAccessor<RGBAColor> _textColor;
    AttributeAccessor<Channel> _textAlpha;
    AttributeAccessor<RGBAColor> _borderColor;
    AttributeAccessor<Channel> _borderAlpha;
    AttributeAccessor<Dp> _borderWidth;
    AttributeAccessor<CornerRadius> _cornerRadius;
    AttributeAccessor<EdgeInsets> _padding;
    AttributeAccessor<Graphics::FontSize> _fontSize;
    AttributeAccessor<Graphics::FontWeight> _fontWeight;
    AttributeAccessor<Graphics::FontWidth> _fontWidth;
    AttributeAccessor<Graphics::FontSlant> _fontSlant;
    AttributeAccessor<Graphics::FontFamily> _fontFamily;
    Shared<Graphics::Typeface> _typeface;
    Shared<Graphics::TextShaper> _textShaper;
    Optional<TextCache> _textCache;
    Optional<MeasureCache> _measureCache;
    Optional<ArrangeCache> _arrangeCache;
};
}
}
