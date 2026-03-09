// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.HoverGestureView.hpp"
#include "Futurewalker.Application.HoverGestureViewEvent.hpp"
#include "Futurewalker.Application.HoverGestureRecognizer.hpp"
#include "Futurewalker.Application.HoverGestureEvent.hpp"
#include "Futurewalker.Application.GestureRecognizerView.hpp"

namespace FW_DETAIL_NS
{
auto HoverGestureView::Make() -> Shared<HoverGestureView>
{
    return View::MakeDerived<HoverGestureView>();
}

auto HoverGestureView::MakeWithContent(const Shared<View>& contentView) -> Shared<HoverGestureView>
{
    auto view = Make();
    view->SetContent(contentView);
    return view;
}

HoverGestureView::HoverGestureView(PassKey<View> key)
  : View(key)
{
}

auto HoverGestureView::GetContent() -> Shared<View>
{
    if (_gestureRecognizerView)
    {
        return _gestureRecognizerView->GetContent();
    }
    return {};
}

auto HoverGestureView::SetContent(const Shared<View>& view) -> void
{
    if (_gestureRecognizerView)
    {
        _gestureRecognizerView->SetContent(view);
    }
}

auto HoverGestureView::Initialize() -> void
{
    _gestureRecognizer = Shared<HoverGestureRecognizer>::Make();
    _gestureRecognizerView = GestureRecognizerView::Make();
    _gestureRecognizerView->SetGestureRecognizer(_gestureRecognizer);
    AddChildBack(_gestureRecognizerView);

    EventReceiver::Connect(*_gestureRecognizer, *this, &HoverGestureView::ReceiveEvent);
}

auto HoverGestureView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<HoverGestureEvent>())
    {
        if (event.Is<HoverGestureEvent::Enter>())
        {
            auto viewEvent = Event<>(Event<HoverGestureViewEvent::Enter>());
            co_await SendEvent(viewEvent);
        }
        else if (event.Is<HoverGestureEvent::Leave>())
        {
            auto viewEvent = Event<>(Event<HoverGestureViewEvent::Leave>());
            co_await SendEvent(viewEvent);
        }
    }
    co_return false;
}
}
