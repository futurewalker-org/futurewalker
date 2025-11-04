// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.TextViewType.hpp"
#include "Futurewalker.Component.Lamp.TextViewStyle.hpp"

#include "Futurewalker.Application.TextView.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class TextView : public View
{
public:
    static auto Make() -> Shared<TextView>;
    static auto MakeWithText(AttributeArg<String> const& text) -> Shared<TextView>;

    auto GetText() const -> String;
    auto SetText(AttributeArg<String> const& text) -> void;

    TextView(PassKey<View> key);

protected:
    auto Initialize() -> void override;

private:
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto UpdateStyle() -> void;

private:
    Shared<::FW_NS::TextView> _textView;
};
}
}
