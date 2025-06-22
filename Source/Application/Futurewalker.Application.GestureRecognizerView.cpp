// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.GestureRecognizerView.hpp"
#include "Futurewalker.Application.GestureRecognizer.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.PointerScope.hpp"

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
}

///
/// @brief
///
auto GestureRecognizerView::PointerIntercept(PointerScope& scope) -> void
{
    if (_gestureRecognizer)
    {
        auto const delegate = GestureRecognizer::Delegate {
          .capturePointer = [&](auto id) { CapturePointer(id); },
          .releasePointer = [&](auto id) { ReleasePointer(id); },
        };
        auto const& event = scope.GetParameter().GetPointerEvent();
        auto const area = GetContentRect();
        scope.SetResult(_gestureRecognizer->PointerIntercept(delegate, event, area));
        return;
    }
    View::PointerIntercept(scope);
}

///
/// @brief
///
auto GestureRecognizerView::Pointer(PointerScope& scope) -> void
{
    if (_gestureRecognizer)
    {
        auto const delegate = GestureRecognizer::Delegate {
          .capturePointer = [&](auto id) { CapturePointer(id); },
          .releasePointer = [&](auto id) { ReleasePointer(id); },
        };
        auto const& event = scope.GetParameter().GetPointerEvent();
        auto const area = GetContentRect();
        scope.SetResult(_gestureRecognizer->Pointer(delegate, event, area));
        return;
    }
    View::Pointer(scope);
}
}
