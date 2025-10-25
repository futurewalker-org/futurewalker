// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.IconViewType.hpp"
#include "Futurewalker.Application.Icon.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Icon view.
///
class IconView : public View
{
public:
    static auto Make() -> Shared<IconView>;
    static auto MakeWithIcon(AttributeArg<Icon> const& icon) -> Shared<IconView>;

    IconView(PassKey<View> key);

    auto GetIcon() const -> Icon;
    auto SetIcon(AttributeArg<Icon> const& icon) -> void;

    auto SetSize(AttributeArg<Dp> const& size) -> void;
    auto SetColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetDisabledColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetDisabledAlpha(AttributeArg<Channel> const& alpha) -> void;

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Draw(DrawScope& scope) -> void override;

private:
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;
    auto GetIconColor() const -> RGBAColor;

private:
    AttributeAccessor<Icon> _icon;
    AttributeAccessor<Dp> _size;
    AttributeAccessor<RGBAColor> _color;
    AttributeAccessor<Channel> _alpha;
    AttributeAccessor<RGBAColor> _disabledColor;
    AttributeAccessor<Channel> _disabledAlpha;
};
}
}
