// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ButtonViewType.hpp"
#include "Futurewalker.Application.ButtonViewEvent.hpp"
#include "Futurewalker.Application.TapGestureViewType.hpp"
#include "Futurewalker.Application.HoverGestureViewType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"

#include "Futurewalker.Color.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A container view that enables button behavior.
///
class ButtonView : public View
{
public:
    static auto Make() -> Shared<ButtonView>;
    static auto MakeWithContent(Shared<View> contentView) -> Shared<ButtonView>;

    ButtonView(PassKey<View> key);

    auto SetContent(Shared<View> content) -> void;
    auto GetContent() -> Shared<View>;

    auto SetActionFlags(ButtonViewActionFlags const flags) -> void;
    auto GetActionFlags() const -> ButtonViewActionFlags;

protected:
    auto Initialize() -> void override;

private:
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto SetDown(const Bool down) -> void;
    auto SetEnter(const Bool enter) -> void;

private:
    Shared<TapGestureView> _tapGestureView;
    Shared<HoverGestureView> _hoverGestureView;
    ButtonViewActionFlags _actionFlags = ButtonViewActionFlags::None;
    Bool _down = false;
    Bool _enter = false;
};
}
}
