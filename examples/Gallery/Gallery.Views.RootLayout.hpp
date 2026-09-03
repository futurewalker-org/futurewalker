// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.ContainerViewType.hpp>
#include <Futurewalker.Application.FlexLayoutType.hpp>
#include <Futurewalker.Application.BoxViewType.hpp>

namespace Futurewalker::Gallery
{
class RootLayout : public View
{
public:
    static auto Make() -> Shared<RootLayout>;

    RootLayout(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveButtonEvent(Event<>& event) -> Bool;
    auto SetButtonSelected(Shared<View> const& button, Bool const active) -> void;

private:
    Weak<View> _selected;
    Shared<FlexLayout> _buttonColumn;
    Shared<BoxView> _rightPaneContentArea;
};
}
    