// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.CheckBoxType.hpp"
#include "Futurewalker.Component.Lamp.CheckBoxEvent.hpp"
#include "Futurewalker.Component.Lamp.CheckBoxStyle.hpp"
#include "Futurewalker.Component.Lamp.CheckBoxRenderViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.PaddingViewType.hpp"
#include "Futurewalker.Application.TapGestureViewType.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class CheckBox : public View
{
public:
    static auto Make() -> Shared<CheckBox>;
    static auto MakeWithState(CheckBoxState const state) -> Shared<CheckBox>;
    static auto MakeWithStateAndLabel(CheckBoxState const state, String const& text) -> Shared<CheckBox>;

    auto GetState() const -> CheckBoxState;
    auto SetState(CheckBoxState const state) -> void;

    auto GetLabelView() -> Shared<View>;
    auto SetLabelView(Shared<View> const& labelView) -> void;

    CheckBox(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto ReceiveAttributeEvent(Event<>& event) -> Bool;

private:
    auto InternalSendPressedEvent() -> void;

private:
    Bool _tapDown = false;
    Bool _keyDown = false;
    CheckBoxState _state = CheckBoxState::Unchecked;
    AttributeAccessor<Dp> _size;
    Shared<CheckBoxRenderView> _renderView;
    Shared<TapGestureView> _tapGestureView;
    Shared<PaddingView> _labelViewPadding;
};
}
}
