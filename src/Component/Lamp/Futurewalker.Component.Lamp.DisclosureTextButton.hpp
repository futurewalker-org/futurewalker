// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.DisclosureTextButtonType.hpp"
#include "Futurewalker.Component.Lamp.DisclosureViewType.hpp"
#include "Futurewalker.Component.Lamp.TextViewType.hpp"
#include "Futurewalker.Component.Lamp.TextButtonType.hpp"
#include "Futurewalker.Component.Lamp.IconViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.Icon.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"
#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Animation.ValueAnimator.hpp" 

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class DisclosureTextButton : public View
{
public:
    static auto Make() -> Shared<DisclosureTextButton>;
    static auto MakeWithText(AttributeArg<String> const& text) -> Shared<DisclosureTextButton>; 
    static auto MakeWithTextAndIcon(AttributeArg<String> const& text, AttributeArg<Icon> const& icon) -> Shared<DisclosureTextButton>;

    DisclosureTextButton(PassKey<View> key);

    auto SetText(AttributeArg<String> const& text) -> void;
    auto SetIcon(AttributeArg<Icon> const& icon) -> void;

    auto SetContent(Shared<View> content) -> void;

private:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;

private:
    Shared<TextView> _buttonText;
    Shared<IconView> _buttonIcon;
    Shared<TextButton> _button;
    Shared<DisclosureView> _disclosure;
    Bool _open = false;
    ValueAnimator<Float64> _openRatioAnimator;
};
}
}
