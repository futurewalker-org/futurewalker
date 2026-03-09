// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.TextButtonType.hpp"
#include "Futurewalker.Component.Lamp.TextButtonEvent.hpp"
#include "Futurewalker.Component.Lamp.TextButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.TextButtonTheme.hpp"
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
///
/// @brief Text button class.
///
class TextButton : public View
{
public:
    static auto Make() -> Shared<TextButton>;
    static auto MakeWithContent(Shared<View> content) -> Shared<TextButton>;
    static auto MakeWithText(AttributeArg<String> const& text) -> Shared<TextButton>;
    static auto MakeWithTextAndIcon(AttributeArg<String> const& text, AttributeArg<Icon> const& icon) -> Shared<TextButton>;

    auto SetContent(Shared<View> content) -> void;
    auto GetContent() -> Shared<View>;

    TextButton(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<ButtonView> _buttonView;
    Shared<PaddingView> _paddingView;
    AttributeAccessor<RGBAColor> _textColor;
    AttributeAccessor<RGBAColor> _disabledTextColor;
    AttributeAccessor<Graphics::FontSize> _textFontSize;
    AttributeAccessor<Graphics::FontWeight> _textFontWeight;
    AttributeAccessor<Graphics::FontWidth> _textFontWidth;
    AttributeAccessor<Graphics::FontSlant> _textFontSlant;
    AttributeAccessor<Graphics::FontFamily> _textFontFamily;
    AttributeAccessor<TextViewHorizontalAlignment> _textHorizontalAlignment;
    AttributeAccessor<TextViewVerticalAlignment> _textVerticalAlignment;
};
}
}
