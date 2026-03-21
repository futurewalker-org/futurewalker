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
    auto const textColor = InternalGetTextColor();
    auto const borderColor = InternalGetBorderColor();
    auto const borderWidth = _borderWidth.GetValueOr(0);
    auto const cornerRadius = _cornerRadius.GetValueOrDefault();
    auto const padding = _padding.GetValueOrDefault();

    auto& scene = scope.GetScene();

    ViewDrawFunction::DrawRoundRect(scene, GetContentRect(), cornerRadius, backgroundColor, GetLayoutDirection());

    auto x = padding.GetLeading();
    auto y = padding.GetTop();
    for (auto const& shapedLine : _shapedLines)
    {
        for (auto const& line : shapedLine.GetLines())
        {
            if (line.GetRunCount() == 0)
            {
                if (auto const typeface = InternalGetTypeface())
                {
                    auto const fontSize = _fontSize.GetValueOr(16);
                    auto const metrics = typeface->GetMetrics(fontSize);
                    y += metrics.ascent + metrics.descent + metrics.leading;
                }
            }
            else
            {
                auto offset = x;
                auto const lineMetrics = line.GetMetrics();
                for (auto const& run : line.GetRuns())
                {
                    auto const runMetrics = run->GetMetrics();
                    scene.PushTranslate({
                        .x = offset,
                        .y = y + lineMetrics.ascent - runMetrics.ascent + lineMetrics.leading / 2,
                    });
                    scene.AddGlyphRun({
                        .run = run,
                        .color = textColor,
                    });
                    scene.Pop({});
                    offset += run->GetAdvance();
                }
                y += lineMetrics.ascent + lineMetrics.descent + lineMetrics.leading;
            }
        }
    }

    ViewDrawFunction::DrawRoundRectBorder(scene, GetContentRect(), cornerRadius, borderColor, borderWidth, GetLayoutDirection());
}

auto TextEdit::Measure(MeasureScope& scope) -> void
{
    InternalUpdateTypeface();
    InternalUpdateLayoutCache();

    auto height = Dp(0);
    for (auto const& shapedLine : _shapedLines)
    {
        for (auto const line : shapedLine.GetLines())
        {
            if (line.GetRunCount() == 0)
            {
                if (auto const typeface = InternalGetTypeface())
                {
                    auto const fontSize = _fontSize.GetValueOr(16);
                    auto const metrics = typeface->GetMetrics(fontSize);
                    height += metrics.ascent + metrics.descent + metrics.leading;
                }
            }
            else
            {
                auto const metrics = line.GetMetrics();
                height += metrics.ascent + metrics.descent + metrics.leading;
            }
        }
    }

    auto const padding = _padding.GetValueOrDefault();
    height += padding.GetTotalHeight();

    auto const widthConstraints = scope.GetParameter().GetWidthConstraints();
    auto const heightConstraints = scope.GetParameter().GetHeightConstraints();
    auto const measuredWidth = widthConstraints.IsBounded() ? widthConstraints.GetMax() : widthConstraints.GetMin();
    auto const measuredHeight = AxisConstraints::Constrain(heightConstraints, height);
    scope.SetMeasuredSize(measuredWidth, measuredHeight);
}

auto TextEdit::Arrange(ArrangeScope& scope) -> void
{
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
        InternalInvalidateLayoutCache();
        InvalidateLayout();
        InvalidateVisual();
    }
    co_return false;
}

auto TextEdit::ReceiveKeyEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<KeyEvent::Down>())
    {
        auto const key = event.As<KeyEvent::Down>()->GetKey();
        co_return false;
    }
    else if (event.Is<KeyEvent::Up>())
    {
        co_return false;
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
            InternalInvalidateLayoutCache();
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

auto TextEdit::InternalGetSelectedRange() -> Range<CodePoint>
{
    if (_inputEditable)
    {
        return _inputEditable->GetSelectedRange();
    }
    return {};
}

auto TextEdit::InternalSetSelectedRange(Range<CodePoint> range) -> void
{
    if (_inputEditable)
    {
        _inputEditable->SetSelectedRange(range);
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

auto TextEdit::InternalSetComposingRange(Range<CodePoint> range) -> void
{
    if (_inputEditable)
    {
        _inputEditable->SetComposingRange(range);
    }
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

auto TextEdit::InternalInvalidateLayoutCache() -> void
{
    _shapedLines.clear();
}

auto TextEdit::InternalUpdateLayoutCache() -> void
{
    if (!_inputEditable || !_textShaper)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (!_shapedLines.empty())
    {
        return;
    }

    auto const text = _inputEditable->GetText().GetString();
    auto breakIterator = LineBreakIterator("en-US");
    breakIterator.SetText(text);

    auto const typeface = InternalGetTypeface();
    auto const fontSize = _fontSize.GetValueOr(16);
    auto const direction = Graphics::TextShaper::Direction::DefaultLtr;
    auto const bcp47ScriptTag = Graphics::TextShaper::FourByteTag({'L', 'a', 't', 'n'});

    auto prevIndex = breakIterator.GetCurrent();
    while (auto nextIndex = breakIterator.GetNext())
    {
        if (breakIterator.GetCurrentBreakCategory() == LineBreakIterator::BreakCategory::Hard)
        {
            auto const lineText = StringFunction::StripTrailingBreakAndSpace(text, prevIndex, *nextIndex);
            auto const shapedLine = _textShaper->ShapeText(Text(lineText), typeface, fontSize, Dp::Infinity(), bcp47ScriptTag, direction);
            _shapedLines.push_back(shapedLine);
            prevIndex = *nextIndex;
        }
    }
    auto const lineText = StringFunction::StripTrailingBreakAndSpace(text, prevIndex, text.GetEnd());
    auto const shapedLine = _textShaper->ShapeText(Text(lineText), typeface, fontSize, Dp::Infinity(), bcp47ScriptTag, direction);
    _shapedLines.push_back(shapedLine);
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
}
