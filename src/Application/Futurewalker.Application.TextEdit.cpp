// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextEdit.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.InputEvent.hpp"
#include "Futurewalker.Application.FocusEvent.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.Key.hpp"
#include "Futurewalker.Application.InputEditable.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp" 

#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.TextShaper.hpp"
#include "Futurewalker.Graphics.GlyphRun.hpp"
#include "Futurewalker.Graphics.FontManager.hpp"

#include "Futurewalker.Text.LineBreakIterator.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto TextEdit::Make() -> Shared<TextEdit>
{
    return View::MakeDerived<TextEdit>();
}

TextEdit::TextEdit(PassKey<View> key)
  : View(key)
{
}

auto TextEdit::GetText() const -> String
{
    return InternalGetText();
}

auto TextEdit::SetText(String const& text) -> void
{
    InternalSetText(text);
}

auto TextEdit::SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    _backgroundColor.SetAttributeArg(color);
}

auto TextEdit::SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _backgroundAlpha.SetAttributeArg(alpha);
}

auto TextEdit::SetTextColor(AttributeArg<RGBAColor> const& color) -> void
{
    _textColor.SetAttributeArg(color);
}

auto TextEdit::SetTextAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _textAlpha.SetAttributeArg(alpha);
}

auto TextEdit::SetBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    _borderColor.SetAttributeArg(color);
}

auto TextEdit::SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _borderAlpha.SetAttributeArg(alpha);
}

auto TextEdit::SetBorderWidth(AttributeArg<Dp> const& width) -> void
{
    _borderWidth.SetAttributeArg(width);
}

auto TextEdit::SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void
{
    _cornerRadius.SetAttributeArg(radius);
}

auto TextEdit::SetPadding(AttributeArg<EdgeInsets> const& padding) -> void
{
    _padding.SetAttributeArg(padding);
}

auto TextEdit::SetFontSize(AttributeArg<Graphics::FontSize> const& size) -> void
{
    _fontSize.SetAttributeArg(size);
}

auto TextEdit::SetFontWeight(AttributeArg<Graphics::FontWeight> const& weight) -> void
{
    _fontWeight.SetAttributeArg(weight);
}

auto TextEdit::SetFontWidth(AttributeArg<Graphics::FontWidth> const& width) -> void
{
    _fontWidth.SetAttributeArg(width);
}

auto TextEdit::SetFontSlant(AttributeArg<Graphics::FontSlant> const& slant) -> void
{
    _fontSlant.SetAttributeArg(slant);
}

auto TextEdit::SetFontFamily(AttributeArg<Graphics::FontFamily> const& family) -> void
{
    _fontFamily.SetAttributeArg(family);
}

auto TextEdit::Initialize() -> void
{
    SetPointerTrackingFlags(ViewPointerTrackingFlags::All);
    SetFocusTrackingFlags(ViewFocusTrackingFlags::All);

    _textShaper = Graphics::TextShaper::Make();
    _inputEditable = InputEditable::Make();
    SetInputEditable(_inputEditable);

    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBackgroundAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeTextColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeTextAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBorderAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeBorderWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(EdgeInsets, AttributePadding, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontSize, AttributeFontSize, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontWeight, AttributeFontWeight, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontWidth, AttributeFontWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontSlant, AttributeFontSlant, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontFamily, AttributeFontFamily, {});

    _backgroundColor.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeBackgroundColor, {});
    _backgroundAlpha.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeBackgroundAlpha, {});
    _textColor.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeTextColor, {});
    _textAlpha.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeTextAlpha, {});
    _borderColor.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeBorderColor, {});
    _borderAlpha.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeBorderAlpha, {});
    _borderWidth.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeBorderWidth, {0});
    _cornerRadius.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeCornerRadius, {});
    _padding.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributePadding, {});
    _fontSize.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeFontSize, {0});
    _fontWeight.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeFontWeight, {0});
    _fontWidth.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeFontWidth, {0});
    _fontSlant.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeFontSlant, {});
    _fontFamily.BindAndConnectAttributeWithDefaultValue(*this, &TextEdit::ReceiveAttributeEvent, AttributeFontFamily, {});

    EventReceiver::Connect(*_inputEditable, *this, &TextEdit::ReceiveInputEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveInputEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveKeyEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveFocusEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceivePointerEvent);

    InternalUpdateTypeface();
}

auto TextEdit::Draw(DrawScope& scope) -> void
{
    auto const backgroundColor = InternalGetBackgroundColor();
    auto const borderColor = InternalGetBorderColor();
    auto const borderWidth = _borderWidth.GetValueOr(0);
    auto const cornerRadius = _cornerRadius.GetValueOrDefault();
    auto const padding = _padding.GetValueOrDefault();

    auto& scene = scope.GetScene();

    ViewDrawFunction::DrawRoundRect(scene, GetContentRect(), cornerRadius, backgroundColor, GetLayoutDirection());

    if (_arrangeCache)
    {
        auto const& arrangedLines = _arrangeCache->arrangedLines;
        if (_selectionBegin == _selectionEnd)
        {
            InternalDrawLines(scene, arrangedLines);
            InternalDrawCaret(scene, _selectionBegin, arrangedLines);
        }
        else
        {
            InternalDrawSelectionHighlight(scene, _selectionBegin, _selectionEnd, arrangedLines);
            InternalDrawLines(scene, arrangedLines);
        }
    }

    ViewDrawFunction::DrawRoundRectBorder(scene, GetContentRect(), cornerRadius, borderColor, borderWidth, GetLayoutDirection());
}

auto TextEdit::Measure(MeasureScope& scope) -> void
{
    InternalUpdateTypeface();
    InternalUpdateTextCache();
    InternalUpdateMeasureCache();

    auto const padding = _padding.GetValueOrDefault();
    auto height = padding.GetTotalHeight();
    if (_measureCache)
    {
        for (auto const& shapedLine : _measureCache->shapedLines)
        {
            auto const metrics = shapedLine.metrics;
            height += metrics.ascent + metrics.descent + metrics.leading;
        }
    }

    auto const widthConstraints = scope.GetParameter().GetWidthConstraints();
    auto const heightConstraints = scope.GetParameter().GetHeightConstraints();
    auto const measuredWidth = widthConstraints.IsBounded() ? widthConstraints.GetMax() : widthConstraints.GetMin();
    auto const measuredHeight = AxisConstraints::Constrain(heightConstraints, height);
    scope.SetMeasuredSize(measuredWidth, measuredHeight);
}

auto TextEdit::Arrange(ArrangeScope& scope) -> void
{
    InternalUpdateArrangeCache();

    if (_inputEditable)
    {
        _inputEditable->SetLayoutRect(GetContentRect());
    }
    View::Arrange(scope);
}

auto TextEdit::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InternalInvalidateTypeface();
        InternalInvalidateMeasureCache();
        InvalidateLayout();
        InvalidateVisual();
    }
    co_return false;
}

auto TextEdit::ReceiveKeyEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<KeyEvent>())
    {
        if (!_inputEditable)
        {
            co_return false;
        }

        if (event.As<KeyEvent>()->IsComposing())
        {
            co_return true;
        }

        auto const key = event.As<KeyEvent>()->GetKey();

        if (key == Key::Enter)
        {
            if (event.Is<KeyEvent::Down>())
            {
                _inputEditable->InsertLineBreak();
            }
            co_return true;
        }
        else if (key == Key::Backspace || key == Key::Delete)
        {
            if (event.Is<KeyEvent::Down>())
            {
                auto const selection = _inputEditable->GetSelectionRange();
                if (selection.GetLength() == 0)
                {
                    if (key == Key::Backspace)
                    {
                        _inputEditable->DeleteSurroundingText(1, 0);
                    }
                    else
                    {
                        _inputEditable->DeleteSurroundingText(0, 1);
                    }
                }
                else
                {
                    _inputEditable->InsertText({}, 0);
                }
            }
            co_return true;
        }
        else if (key == Key::ArrowLeft || key == Key::ArrowRight)
        {
            if (event.Is<KeyEvent::Down>())
            {
                auto const modifiers = event.As<KeyEvent::Down>()->GetModifiers();
                auto const selection = _inputEditable->GetSelectionRange();
                auto const selectionDirection = _inputEditable->GetSelectionDirection();
                auto const textRange = _inputEditable->GetStringRange();
                auto const isRtl = GetLayoutDirection() == LayoutDirection::RightToLeft;
                auto const isShift = (modifiers & ModifierKeyFlags::Shift) != ModifierKeyFlags::None;
                auto const isLeftArrow = (key == Key::ArrowLeft);

                // FIXME: Offset should be based on grapheme cluster.
                auto const basePositionOffset = (isRtl == isLeftArrow) ? 1 : -1;

                if (isShift)
                {
                    if (selection.IsEmpty())
                    {
                        if (isRtl == isLeftArrow)
                        {
                            auto const anchorPosition = selection.end;
                            auto const newCaretPosition = Range<CodePoint>::Clamp(anchorPosition + basePositionOffset, textRange);
                            _inputEditable->SetSelectionRange({selection.begin, newCaretPosition}, TextSelectionDirection::Forward);
                        }
                        else
                        {
                            auto const anchorPosition = selection.begin;
                            auto const newCaretPosition = Range<CodePoint>::Clamp(anchorPosition + basePositionOffset, textRange);
                            _inputEditable->SetSelectionRange({newCaretPosition, selection.end}, TextSelectionDirection::Backward);
                        }
                    }
                    else
                    {
                        if (selectionDirection == TextSelectionDirection::Forward)
                        {
                            auto const anchorPosition = selection.end;
                            auto const newCaretPosition = Range<CodePoint>::Clamp(anchorPosition + basePositionOffset, textRange);
                            _inputEditable->SetSelectionRange({selection.begin, newCaretPosition}, selectionDirection);
                        }
                        else
                        {
                            auto const anchorPosition = selection.begin;
                            auto const newCaretPosition = Range<CodePoint>::Clamp(anchorPosition + basePositionOffset, textRange);
                            _inputEditable->SetSelectionRange({newCaretPosition, selection.end}, selectionDirection);
                        }
                    }
                }
                else
                {
                    auto const anchorPosition = (isRtl == isLeftArrow) ? selection.end : selection.begin;
                    auto const positionOffset = selection.IsEmpty() ? basePositionOffset : 0;
                    auto const newCaretPosition = Range<CodePoint>::Clamp(anchorPosition + positionOffset, textRange);
                    _inputEditable->SetSelectionRange({newCaretPosition, newCaretPosition}, TextSelectionDirection::Forward);
                }
            }
        }
        co_return true;
    }
    co_return false;
}

auto TextEdit::ReceiveInputEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<InputEvent>())
    {
        if (event.Is<InputEvent::Attach>() || event.Is<InputEvent::Detach>())
        {
            co_return true;
        }
        else
        {
            InternalInvalidateTextCache();
            InvalidateLayout();
            InvalidateVisual();
        }
    }
    co_return false;
}

auto TextEdit::ReceivePointerEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PointerEvent>())
    {
        if (event.Is<PointerEvent::Motion::Down>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer down");
            RequestFocus(FocusReason::Pointer);
        }
        else if (event.Is<PointerEvent::Motion::Up>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer up");
        }
        else if (event.Is<PointerEvent::Motion::Move>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer move");
        }
        else if (event.Is<PointerEvent::Motion::Enter>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer enter");
        }
        else if (event.Is<PointerEvent::Motion::Leave>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer leave");
        }
        co_return true;
    }
    co_return false;
}

auto TextEdit::ReceiveFocusEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<FocusEvent::FocusIn>())
    {
        InvalidateVisual();
        auto notifyEventParameter = Event<TextEditEvent::FocusChanged>();
        notifyEventParameter->SetFocused(true);
        auto notifyEvent = Event<>(notifyEventParameter);
        co_await SendEvent(notifyEvent);
    }
    else if (event.Is<FocusEvent::FocusOut>())
    {
        InvalidateVisual();
        auto notifyEventParameter = Event<TextEditEvent::FocusChanged>();
        notifyEventParameter->SetFocused(false);
        auto notifyEvent = Event<>(notifyEventParameter);
        co_await SendEvent(notifyEvent);
    }
    co_return false;
}

auto TextEdit::InternalInsertText(String const& text, CodePoint newSelection) -> void
{
    if (_inputEditable)
    {
        _inputEditable->InsertText(text, newSelection);
        InvalidateVisual();
    }
}

auto TextEdit::InternalGetSelectionDirection() const -> TextSelectionDirection
{
    if (_inputEditable)
    {
        return _inputEditable->GetSelectionDirection();
    }
    return TextSelectionDirection::Forward;
}

auto TextEdit::InternalGetSelectionRange() -> Range<CodePoint>
{
    if (_inputEditable)
    {
        return _inputEditable->GetSelectionRange();
    }
    return {};
}
auto TextEdit::InternalSetSelectionRange(Range<CodePoint> const& range, TextSelectionDirection direction) -> void
{
    if (_inputEditable)
    {
        _inputEditable->SetSelectionRange(range, direction);
    }
}

auto TextEdit::InternalGetComposingRange() -> Range<CodePoint>
{
    if (_inputEditable)
    {
        return _inputEditable->GetComposingRange();
    }
    return {};
}

auto TextEdit::InternalGetText() const -> String
{
    if (_inputEditable)
    {
        return _inputEditable->GetString();
    }
    return {};
}

auto TextEdit::InternalGetText(Range<CodePoint> range) -> String
{
    if (_inputEditable)
    {
        return _inputEditable->GetString(range);
    }
    return {};
}

auto TextEdit::InternalSetText(String const& text) -> void
{
    if (_inputEditable)
    {
        _inputEditable->SetText(Text(text));
    }
}

auto TextEdit::InternalGetTextRange() const -> Range<CodePoint>
{
    if (_inputEditable)
    {
        return _inputEditable->GetStringRange();
    }
    return {};
}

auto TextEdit::InternalDeleteBackward() -> void
{
    if (_inputEditable)
    {
        _inputEditable->DeleteSurroundingText(1, 0);
        InvalidateVisual();
    }
}

auto TextEdit::InternalDeleteForward() -> void
{
    if (_inputEditable)
    {
        _inputEditable->DeleteSurroundingText(0, 1);
        InvalidateVisual();
    }
}

auto TextEdit::InternalInvalidateTextCache() -> void
{
    _textCache.Reset();
}

auto TextEdit::InternalUpdateTextCache() -> void
{
    if (!_inputEditable || _textCache)
    {
        return;
    }

    auto const text = _inputEditable->GetText();
    auto const textString = text.GetString();
    auto breakIterator = LineBreakIterator("en-US");
    breakIterator.SetText(textString);

    auto textCache = TextCache();
    textCache.text = text;

    auto prevIndex = breakIterator.GetCurrent();
    while (auto nextIndex = breakIterator.GetNext())
    {
        if (breakIterator.GetCurrentBreakCategory() == LineBreakIterator::BreakCategory::Hard)
        {
            auto const lineText = StringFunction::StripTrailingBreaks(textString, prevIndex, *nextIndex);
            textCache.lines.emplace_back(CodeUnit(prevIndex), lineText);
            prevIndex = *nextIndex;
        }
    }
    auto const lineText = StringFunction::StripTrailingBreaks(textString, prevIndex, textString.GetEnd());
    textCache.lines.emplace_back(CodeUnit(prevIndex), lineText);

    _textCache = std::move(textCache);

    InternalInvalidateMeasureCache();
}

auto TextEdit::InternalInvalidateMeasureCache() -> void
{
    _measureCache.Reset();
}

auto TextEdit::InternalUpdateMeasureCache() -> void
{
    if (!_inputEditable || !_textCache || _measureCache)
    {
        return;
    }

    auto const typeface = InternalGetTypeface();
    auto const fontSize = _fontSize.GetValueOr(16);
    auto const fontMetrics = typeface->GetMetrics(fontSize);
    auto const direction = Graphics::TextShaper::Direction::DefaultLtr;
    auto const bcp47ScriptTag = Graphics::TextShaper::FourByteTag({'L', 'a', 't', 'n'});

    auto measureCache = MeasureCache();

    for (auto const& [textLineOffset, textLine] : _textCache->lines)
    {
        auto const shapedText = _textShaper->ShapeText(Text(textLine), typeface, fontSize, Dp::Infinity(), bcp47ScriptTag, direction);

        auto count = CodePoint(0);
        for (auto const& wrappedLine : shapedText.GetLines())
        {
            auto shapedLine = ShapedLine {
                .start = _textCache->text.GetCodePointIndexByU8Index(textLineOffset) + count,
                .advance = wrappedLine.GetAdvance(),
                .metrics = wrappedLine.GetMetrics(),
                .runs = wrappedLine.GetRuns(),
            };

            auto const& runs = wrappedLine.GetRuns();

            if (std::ssize(runs) == 0)
            {
                shapedLine.metrics = fontMetrics;
            }

            for (auto const& run : runs)
            {
                count += run->GetText().GetCodePointCount();
            }

            measureCache.shapedLines.push_back(std::move(shapedLine));
        }
    }

    auto findSelectionPosition = [&](CodePoint position) -> SelectionPosition {
        if (measureCache.shapedLines.empty())
        {
            return {};
        }

        auto const lineCount = std::ssize(measureCache.shapedLines);
        for (auto i = SInt64(0); i < lineCount - 1; ++i)
        {
            if (position < measureCache.shapedLines[static_cast<size_t>(i + 1)].start)
            {
                return {i, position - measureCache.shapedLines[static_cast<size_t>(i)].start};
            }
        }
        return {lineCount - 1, position - measureCache.shapedLines[static_cast<size_t>(lineCount - 1)].start};
    };
    auto const selectionRange = InternalGetSelectionRange();
    _selectionBegin = findSelectionPosition(selectionRange.begin);
    _selectionEnd = findSelectionPosition(selectionRange.end);
    _measureCache = std::move(measureCache);

    InternalInvalidateArrangeCache();
}

auto TextEdit::InternalInvalidateArrangeCache() -> void
{
    _arrangeCache.Reset();
}

auto TextEdit::InternalUpdateArrangeCache() -> void
{
    if (!_inputEditable || !_measureCache || _arrangeCache)
    {
        return;
    }

    auto const padding = _padding.GetValueOrDefault();

    auto arrangeCache = ArrangeCache();

    auto x = padding.leading;
    auto y = padding.top;
    for (auto const& shapedLine : _measureCache->shapedLines)
    {
        auto arrangedLine = ArrangedLine {
            .position = Point<Dp>(x, y),
            .start = shapedLine.start,
            .advance = shapedLine.advance,
            .metrics = shapedLine.metrics,
            .runs = {},
        };

        if (std::ssize(shapedLine.runs) == 0)
        {
            if (auto const typeface = InternalGetTypeface())
            {
                auto const fontSize = _fontSize.GetValueOr(16);
                auto const metrics = typeface->GetMetrics(fontSize);
                arrangedLine.metrics = metrics;
                arrangedLine.advance = Dp(0);
                y += metrics.ascent + metrics.descent + metrics.leading;
            }
        }
        else
        {
            auto offset = x;
            auto const& lineMetrics = shapedLine.metrics;
            for (auto const& run : shapedLine.runs)
            {
                auto const runMetrics = run->GetMetrics();
                arrangedLine.runs.push_back({
                    .position = Point<Dp>(offset, y + lineMetrics.ascent - runMetrics.ascent + lineMetrics.leading / 2),
                    .run = run,
                });
                offset += run->GetAdvance();
            }
            arrangedLine.advance = offset - x;
            y += lineMetrics.ascent + lineMetrics.descent + lineMetrics.leading;
        }

        arrangeCache.arrangedLines.push_back(std::move(arrangedLine));
    }
    _arrangeCache = std::move(arrangeCache);
}

auto TextEdit::InternalInvalidateTypeface() -> void
{
    _typeface = nullptr;
}

auto TextEdit::InternalUpdateTypeface() -> void
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
    }
}

auto TextEdit::InternalGetBackgroundColor() const -> RGBAColor
{
    auto const color = _backgroundColor.GetValueOrDefault();
    auto const alpha = _backgroundAlpha.GetValueOrDefault();
    return RGBAColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha().GetF64() * alpha.GetF64());
}

auto TextEdit::InternalGetTextColor() const -> RGBAColor
{
    auto const color = _textColor.GetValueOrDefault();
    auto const alpha = _textAlpha.GetValueOrDefault();
    return RGBAColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha().GetF64() * alpha.GetF64());
}

auto TextEdit::InternalGetBorderColor() const -> RGBAColor
{
    auto const color = _borderColor.GetValueOrDefault();
    auto const alpha = _borderAlpha.GetValueOrDefault();
    return RGBAColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha().GetF64() * alpha.GetF64());
}

auto TextEdit::InternalGetTypeface() const -> Shared<Graphics::Typeface>
{
    return _typeface;
}

auto TextEdit::InternalDrawLines(Graphics::Scene& scene, std::vector<ArrangedLine> const& arrangedLines) const -> void
{
    auto const textColor = InternalGetTextColor();
    for (auto const& line : arrangedLines)
    {
        for (auto const& run : line.runs)
        {
            scene.PushTranslate({
                .x = run.position.x,
                .y = run.position.y,
            });
            scene.AddGlyphRun({
                .run = run.run,
                .color = textColor,
            });
            scene.Pop({});
        }
    }
}

auto TextEdit::InternalDrawCaret(Graphics::Scene& scene, SelectionPosition const& selection, std::vector<ArrangedLine> const& arrangedLines) const -> void
{
    if (IsFocused() && selection.line < std::ssize(arrangedLines))
    {
        auto const padding = _padding.GetValueOrDefault();
        auto const caretColor = InternalGetBorderColor(); // TODO: Make this configurable

        auto const& lineInfo = arrangedLines[static_cast<size_t>(selection.line)];
        auto const lineY = lineInfo.position.y;
        auto const lineHeight = lineInfo.metrics.ascent + lineInfo.metrics.descent;

        auto offset = selection.offset;

        auto caretX = padding.leading;
        auto const caretY = lineY;
        auto const caretHeight = lineHeight;

        for (auto const& runInfo : lineInfo.runs)
        {
            auto const& run = runInfo.run;
            auto const& runX = runInfo.position.x;

            if (run->GetText().GetCodePointCount() < offset)
            {
                offset -= run->GetText().GetCodePointCount();
            }
            else
            {
                auto glyphX = Dp(0);
                if (auto const glyphIndex = run->GetGlyphIndex(offset))
                {
                    if (auto const position = run->GetGlyphPosition(*glyphIndex))
                    {
                        glyphX = position->x;
                    }
                }
                else
                {
                    glyphX = run->GetAdvance();
                }
                caretX = runX + glyphX;
                break;
            }
        }

        auto const caretRect = Rect<Dp>::Make(Point<Dp>(caretX, caretY), Size<Dp>(1, caretHeight));
        scene.AddRect({
            .rect = caretRect,
            .color = caretColor,
        });
    }
}

auto TextEdit::InternalDrawSelectionHighlight(Graphics::Scene& scene, SelectionPosition const& selectionBegin, SelectionPosition const& selectionEnd, std::vector<ArrangedLine> const& arrangedLines) const -> void
{
    auto const selectionColor = InternalGetBorderColor().WithAlphaMultiplied(0.25); // TODO: Make this configurable

    auto const lineBegin = SInt64::Max(0, selectionBegin.line);
    auto const lineEnd = SInt64::Min(std::ssize(arrangedLines) - 1, selectionEnd.line);

    for (auto lineIndex = lineBegin; lineIndex <= lineEnd; ++lineIndex)
    {
        auto const& lineInfo = arrangedLines[static_cast<size_t>(lineIndex)];
        auto const lineY = lineInfo.position.y;
        auto const lineHeight = lineInfo.metrics.ascent + lineInfo.metrics.descent;

        auto selectionStartX = lineInfo.position.x;
        if (lineIndex == selectionBegin.line)
        {
            auto offset = selectionBegin.offset;
            for (auto const& runInfo : lineInfo.runs)
            {
                auto const& run = runInfo.run;
                auto const& runX = runInfo.position.x;

                if (run->GetText().GetCodePointCount() < offset)
                {
                    offset -= run->GetText().GetCodePointCount();
                }
                else
                {
                    auto glyphX = Dp(0);
                    if (auto const glyphIndex = run->GetGlyphIndex(offset))
                    {
                        if (auto const position = run->GetGlyphPosition(*glyphIndex))
                        {
                            glyphX = position->x;
                        }
                    }
                    else
                    {
                        glyphX = run->GetAdvance();
                    }
                    selectionStartX = runX + glyphX;
                    break;
                }
            }
        }

        auto selectionEndX = lineInfo.position.x;
        if (lineIndex == selectionEnd.line)
        {
            auto offset = selectionEnd.offset;
            for (auto const& runInfo : lineInfo.runs)
            {
                auto const& run = runInfo.run;
                auto const& runX = runInfo.position.x;

                if (run->GetText().GetCodePointCount() < offset)
                {
                    offset -= run->GetText().GetCodePointCount();
                }
                else
                {
                    auto glyphX = Dp(0);
                    if (auto const glyphIndex = run->GetGlyphIndex(offset))
                    {
                        if (auto const position = run->GetGlyphPosition(*glyphIndex))
                        {
                            glyphX = position->x;
                        }
                    }
                    else
                    {
                        glyphX = run->GetAdvance();
                    }
                    selectionEndX = runX + glyphX;
                    break;
                }
            }
        }
        else
        {
            selectionEndX = lineInfo.position.x + lineInfo.advance;
        }

        auto const selectionRect = Rect<Dp>::Make(Point<Dp>(selectionStartX, lineY), Size<Dp>(selectionEndX - selectionStartX, lineHeight));
        scene.AddRect({
            .rect = selectionRect,
            .color = selectionColor,
        });
    }
}
}
