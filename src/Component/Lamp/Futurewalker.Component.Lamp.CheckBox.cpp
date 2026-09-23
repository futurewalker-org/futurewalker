// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.CheckBox.hpp"
#include "Futurewalker.Component.Lamp.CheckBoxRenderView.hpp"
#include "Futurewalker.Component.Lamp.TextView.hpp"

#include "Futurewalker.Application.TapGestureView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp"
#include "Futurewalker.Application.FocusEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.Key.hpp"
#include "Futurewalker.Application.FlexLayout.hpp"
#include "Futurewalker.Application.PaddingView.hpp" 
#include "Futurewalker.Application.SizedView.hpp"

#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.SvgDocument.hpp"
#include "Futurewalker.Graphics.ColorFilter.hpp"

#include "Futurewalker.Resource.ResourceManager.hpp"
#include "Futurewalker.Resource.Resource.hpp"

#include "Resource/Futurewalker.Component.Lamp.hpp"
#include "Resource/Futurewalker.Component.Lamp.Icon.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto CheckBox::Make() -> Shared<CheckBox>
{
    return View::MakeDerived<CheckBox>();
}

auto CheckBox::MakeWithState(CheckBoxState const state) -> Shared<CheckBox>
{
    auto checkbox = Make();
    checkbox->SetState(state);
    return checkbox;
}

auto CheckBox::MakeWithStateAndLabel(CheckBoxState const state, String const& text) -> Shared<CheckBox>
{
    // TOOD: Apply font size at container level.
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(Graphics::FontSize, AttributeFontSize, [](Dp const& s) { return Graphics::FontSize(static_cast<float64_t>(s)); }, CheckBoxStyle::Size);
    auto textView = TextView::MakeWithText(text);
    AttributeNode::SetValue<TextViewStyle::VerticalTrim>(*textView, TextViewVerticalTrim::CapHeight);
    AttributeNode::SetReference<TextViewStyle::FontSize>(*textView, AttributeFontSize);
    auto checkbox = Make();
    checkbox->SetState(state);
    checkbox->SetLabelView(textView);
    return checkbox;
}

auto CheckBox::GetState() const -> CheckBoxState
{
    return _state;
}

auto CheckBox::SetState(CheckBoxState const state) -> void
{
    if (_state != state)
    {
        _state = state;
        _renderView->SetState(_state);
    }
}

auto CheckBox::GetLabelView() -> Shared<View>
{
    return _labelViewPadding->GetContent();
}

auto CheckBox::SetLabelView(Shared<View> const& labelView) -> void
{
    _labelViewPadding->SetContent(labelView);
    _labelViewPadding->SetVisible(labelView != nullptr);
}

CheckBox::CheckBox(PassKey<View> key)
  : View(key)
{
}

auto CheckBox::Initialize() -> void
{
    SetFocusTrackingFlags(ViewFocusTrackingFlag::All);

    _size.BindAndConnectAttribute(*this, &CheckBox::ReceiveAttributeEvent, CheckBoxStyle::Size);

    _renderView = CheckBoxRenderView::Make();
    _renderView->SetState(_state);
    _renderView->SetEnabled(true);
    _renderView->SetCornerRadius(CheckBoxStyle::CornerRadius);
    _renderView->SetBackgroundColor(CheckBoxStyle::BackgroundColor);
    _renderView->SetBackgroundColorDisabled(CheckBoxStyle::BackgroundColorDisabled);
    _renderView->SetBackgroundAlpha(CheckBoxStyle::BackgroundAlpha);
    _renderView->SetBackgroundAlphaDisabled(CheckBoxStyle::BackgroundAlphaDisabled);
    _renderView->SetBorderColor(CheckBoxStyle::BorderColor);
    _renderView->SetBorderColorDisabled(CheckBoxStyle::BorderColorDisabled);
    _renderView->SetBorderColorFocused(CheckBoxStyle::BorderColorFocused);
    _renderView->SetBorderAlpha(CheckBoxStyle::BorderAlpha);
    _renderView->SetBorderAlphaDisabled(CheckBoxStyle::BorderAlphaDisabled);
    _renderView->SetBorderAlphaFocused(CheckBoxStyle::BorderAlphaFocused);
    _renderView->SetCheckMarkColor(CheckBoxStyle::CheckMarkColor);
    _renderView->SetCheckMarkColorDisabled(CheckBoxStyle::CheckMarkColorDisabled);
    _renderView->SetCheckMarkAlpha(CheckBoxStyle::CheckMarkAlpha);
    _renderView->SetCheckMarkAlphaDisabled(CheckBoxStyle::CheckMarkAlphaDisabled);

    auto sized = SizedView::MakeWithContent(_renderView);
    sized->SetWidth(CheckBoxStyle::Size);
    sized->SetHeight(CheckBoxStyle::Size);

    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(EdgeInsets, AttributeLabelGap, [](Dp const& s) { return EdgeInsets(s, 0, 0, 0); }, CheckBoxStyle::LabelGap);
    _labelViewPadding = PaddingView::Make();
    _labelViewPadding->SetPadding(AttributeLabelGap);
    _labelViewPadding->SetVisible(false);

    auto row = FlexLayout::Make();
    row->SetDirection(FlexLayoutDirection::Row);
    row->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    row->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
    row->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Center);
    row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
    row->AddChild(sized);
    row->AddChild(_labelViewPadding);

    auto padding = PaddingView::MakeWithContent(row);
    padding->SetPadding(CheckBoxStyle::Padding);

    _tapGestureView = TapGestureView::MakeWithContent(padding);
    AddChildBack(_tapGestureView);

    EventReceiver::Connect(*this, *this, &CheckBox::ReceiveEvent);
}

auto CheckBox::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<ViewEvent::EnabledChanged>())
    {
        _renderView->SetEnabled(IsEnabledFromRoot());
    }
    else if (event.Is<FocusEvent::FocusIn>() || event.Is<FocusEvent::FocusOut>())
    {
        if (_keyDown)
        {
            _keyDown = false;
            _renderView->SetDown(false);
        }
        _renderView->SetFocused(IsFocused(), GetFocusedReason());
    }
    else if (event.Is<KeyEvent::Down>())
    {
        auto const key = event.As<KeyEvent::Down>()->GetUnmodifiedKey();
        if ((key != Key::Process) && (key == Key::Space || key == Key::Enter))
        {
            if (!_keyDown && !_tapDown)
            {
                _keyDown = true;
                _renderView->SetDown(true);
            }
            return true;
        }
    }
    else if (event.Is<KeyEvent::Up>())
    {
        auto const key = event.As<KeyEvent::Up>()->GetUnmodifiedKey();
        if ((key != Key::Process) && (key == Key::Space || key == Key::Enter))
        {
            if (_keyDown)
            {
                _keyDown = false;
                _renderView->SetDown(false);
                InternalSendPressedEvent();
            }
            return true;
        }
    }
    else if (event.Is<TapGestureViewEvent::Begin>())
    {
        if (!_tapDown && !_keyDown)
        {
            _tapDown = true;
            _renderView->SetDown(true);
        }
        RequestFocus(FocusReason::Pointer);
        return true;
    }
    else if (event.Is<TapGestureViewEvent::Tap>())
    {
        if (_tapDown)
        {
            _tapDown = false;
            _renderView->SetDown(false);
            InternalSendPressedEvent();
        }
        return true;
    }
    else if (event.Is<TapGestureViewEvent::Cancel>())
    {
        if (_tapDown)
        {
            _tapDown = false;
            _renderView->SetDown(false);
        }
        return true;
    }
    return false;
}

auto CheckBox::ReceiveAttributeEvent(Event<>& event) -> Bool
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateVisual();
    }
    return false;
}

auto CheckBox::InternalSendPressedEvent() -> void
{
    auto event = Event<>(Event<CheckBoxEvent::Pressed>());
    SendEvent(event);
}
}
