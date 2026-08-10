#pragma once

#include "SplitPaneViewEvent.hpp" 

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.ContainerViewType.hpp>
#include <Futurewalker.Application.FlexLayoutType.hpp>
#include <Futurewalker.Application.BoxViewType.hpp>

namespace Futurewalker::ComponentGallery
{
class SplitPaneView : public View
{
public:
    static auto Make() -> Shared<SplitPaneView>;

    SplitPaneView(PassKey<View> key);

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
    