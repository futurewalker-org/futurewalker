#pragma once

#include "SplitPaneView.hpp"

#include <Futurewalker.Application.ViewController.hpp>

namespace Futurewalker::ComponentGallery
{
class SplitPaneController : public ViewController<SplitPaneView>
{
public:
    static auto Make() -> Shared<SplitPaneController>;

    SplitPaneController(PassKey<ViewController<SplitPaneView>> key);

protected:
    auto MakeView() -> Shared<SplitPaneView> override;

    auto ReceiveViewEvent(Event<>& event) -> Async<Bool>;
};
}
