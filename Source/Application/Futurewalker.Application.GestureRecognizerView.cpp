// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.GestureRecognizerView.hpp"
#include "Futurewalker.Application.GestureRecognizer.hpp"
#include "Futurewalker.Application.ContainerView.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make instance.
///
auto GestureRecognizerView::Make() -> Shared<GestureRecognizerView>
{
    return View::MakeDerived<GestureRecognizerView>();
}

///
/// @brief Make instance with content.
///
/// @param content Content view.
///
auto GestureRecognizerView::MakeWithContent(Shared<View> const& content) -> Shared<GestureRecognizerView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

///
/// @brief Constructor.
///
GestureRecognizerView::GestureRecognizerView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief
///
auto GestureRecognizerView::SetGestureRecognizer(Shared<GestureRecognizer> const& gestureRecognizer) -> void
{
    if (_gestureRecognizer != gestureRecognizer)
    {
        // TODO: Send cancel event to old recognizer.
        _gestureRecognizer = gestureRecognizer;
    }
}

///
/// @brief
///
auto GestureRecognizerView::GetGestureRecognizer() -> Shared<GestureRecognizer>
{
    return _gestureRecognizer;
}

///
/// @brief
///
auto GestureRecognizerView::GetGestureRecognizer() const -> Shared<GestureRecognizer const>
{
    return _gestureRecognizer;
}

///
/// @brief
///
auto GestureRecognizerView::SetContent(Shared<View> const& view) -> void
{
    if (_containerView)
    {
        _containerView->SetContent(view);
    }
}

///
/// @brief
///
auto GestureRecognizerView::GetContent() -> Shared<View>
{
    if (_containerView)
    {
        return _containerView->GetContent();
    }
    return {};
}

///
/// @brief
///
auto GestureRecognizerView::GetContent() const -> Shared<View const>
{
    if (_containerView)
    {
        return _containerView->GetContent();
    }
    return {};
}

///
/// @brief
///
auto GestureRecognizerView::Initialize() -> void
{
    SetPointerTrackingFlags(ViewPointerTrackingFlags::All);

    _containerView = ContainerView::Make();
    AddChildBack(_containerView);

    EventReceiver::Connect(*this, *this, &GestureRecognizerView::ReceiveEvent);
}

///
/// @brief Receive event.
///
auto GestureRecognizerView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewEvent::Attached>())
    {
        if (IsEnabledFromRoot())
        {
            _gestureRecognizer->Start(
              GestureRecognizer::Delegate {
                  .capturePointer = [&](auto id) { CapturePointer(id); },
                  .releasePointer = [&](auto id) { ReleasePointer(id); },
              });
        }
    }
    else if (event.Is<ViewEvent::Detached>())
    {
        if (IsEnabledFromRoot())
        {
            _gestureRecognizer->Stop();
        }
    }
    else if (event.Is<ViewEvent::EnabledChanged>())
    {
        if (IsAttached())
        {
            if (IsEnabledFromRoot())
            {
                _gestureRecognizer->Start(
                  GestureRecognizer::Delegate {
                      .capturePointer = [&](auto id) { CapturePointer(id); },
                      .releasePointer = [&](auto id) { ReleasePointer(id); },
                  });
            }
            else
            {
                _gestureRecognizer->Stop();
            }
        }
    }
    else if (event.Is<PointerEvent>())
    {
        if (IsEnabledFromRoot() && IsAttached())
        {
            co_return _gestureRecognizer->Recognize(event.As<PointerEvent>(), GetContentRect());
        }
        co_return false;
    }
    co_return false;
}
}
