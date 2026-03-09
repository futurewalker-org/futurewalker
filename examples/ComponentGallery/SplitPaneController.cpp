#include "SplitPaneController.hpp"
#include "ColorPageView.hpp"
#include "HomePageView.hpp"
#include "TypographyPageView.hpp"
#include "ButtonPageView.hpp"
#include "TextEditPageView.hpp"

#include <Futurewalker.Component.Lamp.TextView.hpp>

namespace Futurewalker::ComponentGallery
{
auto SplitPaneController::Make() -> Shared<SplitPaneController>
{
    return ViewController<SplitPaneView>::MakeDerived<SplitPaneController>();
}

SplitPaneController::SplitPaneController(PassKey<ViewController<SplitPaneView>> key)
  : ViewController<SplitPaneView>(key)
{
}

auto SplitPaneController::MakeView() -> Shared<SplitPaneView>
{
    auto splitPane = SplitPaneView::Make();
    splitPane->SetPages({
        {u8"Home", HomePageView::Make()},
        {u8"Color", ColorPageView::Make()},
        {u8"Typography", TypographyPageView::Make()},
        {u8"Buttons", ButtonPageView::Make()},
        {u8"Text fields", TextEditPageView::Make()},
    });
    EventReceiver::Connect(*splitPane, *this, &SplitPaneController::ReceiveViewEvent);
    return splitPane;
}

auto SplitPaneController::ReceiveViewEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<SplitPaneViewEvent>())
    {
        auto const index = event.As<SplitPaneViewEvent>()->GetIndex();
        GetView()->SetActivePage(index);
        co_return true;
    }
    co_return false;
}
}
