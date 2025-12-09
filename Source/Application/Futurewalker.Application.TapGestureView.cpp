// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TapGestureView.hpp"
#include "Futurewalker.Application.TapGestureViewEvent.hpp"
#include "Futurewalker.Application.TapGestureRecognizer.hpp"
#include "Futurewalker.Application.TapGestureEvent.hpp"
#include "Futurewalker.Application.GestureRecognizerView.hpp"

namespace FW_DETAIL_NS
{
auto TapGestureView::Make() -> Shared<TapGestureView>
{
    return View::MakeDerived<TapGestureView>();
}

auto TapGestureView::MakeWithContent(const Shared<View>& contentView) -> Shared<TapGestureView>
{
    auto view = Make();
    view->SetContent(contentView);
    return view;
}

TapGestureView::TapGestureView(PassKey<View> key)
  : View(key)
{
}

auto TapGestureView::GetContent() -> Shared<View>
{
    if (_gestureRecognizerView)
    {
        return _gestureRecognizerView->GetContent();
    }
    return {};
}


auto TapGestureView::SetContent(const Shared<View>& view) -> void
{
    if (_gestureRecognizerView)
    {
        _gestureRecognizerView->SetContent(view);
    }
}

auto TapGestureView::Initialize() -> void
{
    _gestureRecognizer = Shared<TapGestureRecognizer>::Make();
    _gestureRecognizerView = GestureRecognizerView::Make();
    _gestureRecognizerView->SetGestureRecognizer(_gestureRecognizer);
    AddChildBack(_gestureRecognizerView);

    EventReceiver::Connect(*_gestureRecognizer, *this, &TapGestureView::ReceiveEvent);
}

auto TapGestureView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<TapGestureEvent>())
    {
        if (event.Is<TapGestureEvent::Down>())
        {
            auto viewEvent = Event<>(Event<TapGestureViewEvent::Down>());
            co_await SendEvent(viewEvent);
        }
        else if (event.Is<TapGestureEvent::Up>())
        {
            auto viewEvent = Event<>(Event<TapGestureViewEvent::Up>());
            co_await SendEvent(viewEvent);
        }
        else if (event.Is<TapGestureEvent::Cancel>())
        {
            auto viewEvent = Event<>(Event<TapGestureViewEvent::Cancel>());
            co_await SendEvent(viewEvent);
        }
        else if (event.Is<TapGestureEvent::Tap>())
        {
            auto viewEvent = Event<>(Event<TapGestureViewEvent::Tap>());
            co_await SendEvent(viewEvent);
        }
    }
    co_return false;
}
}
