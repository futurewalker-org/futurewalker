// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.IconButtonType.hpp"
#include "Futurewalker.Component.Lamp.IconButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.IconButtonEvent.hpp"
#include "Futurewalker.Component.Lamp.ButtonViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.IconType.hpp"
#include "Futurewalker.Application.PaddingViewType.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"
#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class IconButton : public View 
{
public:
    static auto Make() -> Shared<IconButton>;
    static auto MakeWithContent(Shared<View> content) -> Shared<IconButton>;
    static auto MakeWithIcon(AttributeArg<Icon> const& icon) -> Shared<IconButton>;

    auto SetContent(Shared<View> content) -> void;
    auto GetContent() -> Shared<View>;

    IconButton(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto ReceiveAttributeEvent(Event<>& event) -> Bool;

private:
    Shared<PaddingView> _paddingView;
    Shared<ButtonView> _buttonView;
    AttributeAccessor<Dp> _iconSize;
    AttributeAccessor<RGBAColor> _iconColor;
    AttributeAccessor<Channel> _iconAlpha;
    AttributeAccessor<RGBAColor> _iconDisabledColor;
    AttributeAccessor<Channel> _iconDisabledAlpha;
};
}
}
