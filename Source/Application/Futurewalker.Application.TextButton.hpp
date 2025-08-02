// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TextButtonType.hpp"
#include "Futurewalker.Application.TextButtonStyle.hpp"
#include "Futurewalker.Application.ButtonViewType.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"
#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_DETAIL_NS
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
    static auto MakeWithText(AttributeArg<String> const& arg) -> Shared<TextButton>;

    auto SetContent(Shared<View> content) -> void;
    auto GetContent() -> Shared<View>;

    TextButton(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<ButtonView> _buttonView;
    AttributeAccessor<RGBAColor> _backgroundColor;
    AttributeAccessor<RGBAColor> _highlightColor;
    AttributeAccessor<RGBAColor> _borderColor;
    AttributeAccessor<Channel> _hoverHighlightAlpha;
    AttributeAccessor<Channel> _pressHighlightAlpha;
    AttributeAccessor<CornerRadius> _cornerRadius;
    AttributeAccessor<Dp> _borderWidth;
    AttributeAccessor<RGBAColor> _textColor;
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
