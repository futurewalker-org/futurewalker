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

    struct PageInfo
    {
        String title;
        Shared<View> content;
    };
    auto SetPages(std::vector<PageInfo> const& pages) -> void;
    auto SetActivePage(SInt64 const index) -> void;

protected:
    auto Initialize() -> void override;
    auto ReceiveButtonEvent(Event<>& event) -> Async<Bool>;
    auto SetButtonActive(SInt64 const index, Bool const active) -> void;

private:
    SInt64 _index = -1;
    std::vector<PageInfo> _pages;
    Shared<FlexLayout> _buttonColumn;
    Shared<BoxView> _rightPaneContentArea;
};
}
    