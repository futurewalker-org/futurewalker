// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextEdit.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.InputEvent.hpp"
#include "Futurewalker.Application.FocusEvent.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.Key.hpp"
#include "Futurewalker.Application.InputMethod.hpp"
#include "Futurewalker.Application.InputMethodEditable.hpp"

#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.TextShaper.hpp"
#include "Futurewalker.Graphics.GlyphRun.hpp"
#include "Futurewalker.Graphics.FontManager.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
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

auto TextEdit::SetDisabledBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    _disabledBackgroundColor.SetAttributeArg(color);
}

auto TextEdit::SetDisabledBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _disabledBackgroundAlpha.SetAttributeArg(alpha);
}

auto TextEdit::SetTextColor(AttributeArg<RGBAColor> const& color) -> void
{
    _textColor.SetAttributeArg(color);
}

auto TextEdit::SetTextAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _textAlpha.SetAttributeArg(alpha);
}

auto TextEdit::SetDisabledTextColor(AttributeArg<RGBAColor> const& color) -> void
{
    _disabledTextColor.SetAttributeArg(color);
}

auto TextEdit::SetDisabledTextAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _disabledTextAlpha.SetAttributeArg(alpha);
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
    _inputMethodEditable = InputMethodEditable::Make();

    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBackgroundAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeDisabledBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeDisabledBackgroundAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeTextColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeTextAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeDisabledTextColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeDisabledTextAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontSize, AttributeFontSize, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontWeight, AttributeFontWeight, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontWidth, AttributeFontWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontSlant, AttributeFontSlant, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Graphics::FontFamily, AttributeFontFamily, {});

    auto bindAndConnectAttribute = [this]<class Accessor>(Accessor& accessor, StaticAttributeRef<typename Accessor::ValueType> attribute, typename Accessor::ValueType const& value) {
        accessor.BindAttributeWithDefaultValue(*this, attribute, value);
        EventReceiver::Connect(accessor, *this, &TextEdit::ReceiveAttributeEvent);
    };
    bindAndConnectAttribute(_backgroundColor, AttributeBackgroundColor, {});
    bindAndConnectAttribute(_backgroundAlpha, AttributeBackgroundAlpha, {});
    bindAndConnectAttribute(_disabledBackgroundColor, AttributeDisabledBackgroundColor, {});
    bindAndConnectAttribute(_disabledBackgroundAlpha, AttributeDisabledBackgroundAlpha, {});
    bindAndConnectAttribute(_textColor, AttributeTextColor, {});
    bindAndConnectAttribute(_textAlpha, AttributeTextAlpha, {});
    bindAndConnectAttribute(_disabledTextColor, AttributeDisabledTextColor, {});
    bindAndConnectAttribute(_disabledTextAlpha, AttributeDisabledTextAlpha, {});
    bindAndConnectAttribute(_fontSize, AttributeFontSize, {0});
    bindAndConnectAttribute(_fontWeight, AttributeFontWeight, {0});
    bindAndConnectAttribute(_fontWidth, AttributeFontWidth, {0});
    bindAndConnectAttribute(_fontSlant, AttributeFontSlant, {});
    bindAndConnectAttribute(_fontFamily, AttributeFontFamily, {});

    EventReceiver::Connect(*_inputMethodEditable, *this, &TextEdit::ReceiveInputEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveInputEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveKeyEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveFocusEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceivePointerEvent);
}

auto TextEdit::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    auto const backgroundColor = _backgroundColor.GetValueOrDefault();
    auto const textColor = _textColor.GetValueOrDefault();

    scene.AddRect({
        .rect = GetContentRect(),
        .color = backgroundColor,
        .drawStyle = Graphics::DrawStyle::Fill,
    });

    scene.AddRect({
        .rect = GetContentRect(),
        .color = IsFocused() ? RGBAColor(1, 0, 0, 1) : RGBAColor(0, 0, 1, 1),
        .drawStyle = Graphics::DrawStyle::Stroke,
        .strokeWidth = IsFocused() ? 2.0 : 1.0,
    });

    if (_shapedText)
    {
        auto offset = Dp(0);
        for (auto const& line : _shapedText->GetLines())
        {
            auto const lineMetrics = line.GetMetrics();
            for (auto const& run : line.GetRuns())
            {
                auto const runMetrics = run->GetMetrics();
                scene.PushTranslate({
                    .x = offset,
                    .y = lineMetrics.GetAscent() - runMetrics.GetAscent(),
                });
                scene.AddGlyphRun({
                    .run = run,
                    .color = textColor,
                });
                scene.Pop({});
                offset += run->GetAdvance();
            }
        }
    }
}

auto TextEdit::Measure(MeasureScope& scope) -> void
{
    InternalUpdateLayoutCache();
    View::Measure(scope);
}

auto TextEdit::Arrange(ArrangeScope& scope) -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->SetLayoutRect(LocalToRootRect(GetContentRect()));
    }
    View::Arrange(scope);
}

auto TextEdit::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
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
        FW_DEBUG_LOG_INFO("TextEdit::Down");
        co_return true;
    }
    else if (event.Is<KeyEvent::Up>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::Up");
        co_return true;
    }
    co_return false;
}

auto TextEdit::ReceiveInputEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<InputEvent::Attach>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::ReceiveInputEvent SetContext");
        auto const parameter = event.As<InputEvent::Attach>();
        InternalSetInputMethod(parameter->GetInputMethod());
        co_return true;
    }
    else if (event.Is<InputEvent::Detach>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::ReceiveInputEvent ReleaseContext");
        InternalSetInputMethod(nullptr);
        co_return true;
    }
    else if (event.Is<InputEvent::InsertText>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::ReceiveInputEvent InsertText");
        InternalInvalidateLayoutCache();
        InvalidateLayout();
        InvalidateVisual();
        co_return false;
    }
    else if (event.Is<InputEvent::DeleteSurroundingText>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::ReceiveInputEvent DeleteSurroundgText");
        InternalInvalidateLayoutCache();
        InvalidateLayout();
        InvalidateVisual();
        co_return false;
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
            SetFocused(true);
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
        FW_DEBUG_LOG_INFO("TextEdit: focus in");
        InvalidateVisual();
    }
    else if (event.Is<FocusEvent::FocusOut>())
    {
        FW_DEBUG_LOG_INFO("TextEdit: focus out");
        InvalidateVisual();
    }
    co_return false;
}

auto TextEdit::InternalSetInputMethod(Shared<InputMethod> const& inputMethod) -> void
{
    if (_inputMethod != inputMethod)
    {
        _inputMethod = inputMethod;

        if (_inputMethod)
        {
            _inputMethod->SetEditable(_inputMethodEditable);
        }
    }
}

auto TextEdit::InternalInsertText(String const& text, CodePoint newSelection) -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->InsertText(text, newSelection);
        InvalidateVisual();
    }
}

auto TextEdit::InternalGetSelectedRange() -> Range<CodePoint>
{
    if (_inputMethodEditable)
    {
        return _inputMethodEditable->GetSelectedRange();
    }
    return {};
}

auto TextEdit::InternalSetSelectedRange(Range<CodePoint> range) -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->SetSelectedRange(range);
    }
}

auto TextEdit::InternalGetComposingRange() -> Range<CodePoint>
{
    if (_inputMethodEditable)
    {
        return _inputMethodEditable->GetComposingRange();
    }
    return {};
}

auto TextEdit::InternalSetComposingRange(Range<CodePoint> range) -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->SetComposingRange(range);
    }
}

auto TextEdit::InternalGetText() const -> String
{
    if (_inputMethodEditable)
    {
        return _inputMethodEditable->GetString();
    }
    return {};
}

auto TextEdit::InternalGetText(Range<CodePoint> range) -> String
{
    if (_inputMethodEditable)
    {
        return _inputMethodEditable->GetString(range);
    }
    return {};
}

auto TextEdit::InternalSetText(String const& text) -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->SetText(Text(text));
    }
}

auto TextEdit::InternalGetTextRange() const -> Range<CodePoint>
{
    if (_inputMethodEditable)
    {
        return _inputMethodEditable->GetStringRange();
    }
    return {};
}

auto TextEdit::InternalDeleteBackward() -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->DeleteSurroundingText(1, 0);
        InvalidateVisual();
    }
}

auto TextEdit::InternalDeleteForward() -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->DeleteSurroundingText(0, 1);
        InvalidateVisual();
    }
}

auto TextEdit::InternalInvalidateLayoutCache() -> void
{
    _shapedText = nullptr;
}

auto TextEdit::InternalUpdateLayoutCache() -> void
{
    if (!_inputMethodEditable || !_textShaper)
    {
        FW_DEBUG_ASSERT(false);
        return;
    }

    if (_shapedText)
    {
        return;
    }

    auto const fontSize = _fontSize.GetValueOr(16);
    auto const fontWidth = _fontWidth.GetValueOr(Graphics::FontWidth::Normal());
    auto const fontWeight = _fontWeight.GetValueOr(Graphics::FontWeight::Normal());
    auto const fontSlant = _fontSlant.GetValueOrDefault();
    auto fontStyle = Graphics::FontStyle(fontWeight, fontWidth, fontSlant);

    if (auto const fontManager = Locator::GetInstance<Graphics::FontManager>())
    {
        auto const typeface = fontManager->FindTypefaceByFamilyAndStyle(_fontFamily.GetValueOrDefault(), fontStyle);
        auto const text = _inputMethodEditable->GetText();
        _shapedText = Shared<Graphics::ShapedText>::Make(_textShaper->ShapeText(text, typeface, fontSize, Dp::Infinity()));
    }
}
}
}
