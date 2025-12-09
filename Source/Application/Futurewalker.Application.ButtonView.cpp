// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ButtonView.hpp"
#include "Futurewalker.Application.TapGestureView.hpp"
#include "Futurewalker.Application.HoverGestureView.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make instance.
///
auto ButtonView::Make() -> Shared<ButtonView>
{
    return View::MakeDerived<ButtonView>();
}

///
/// @brief Make instance with content view.
///
/// @param contentView 
///
auto ButtonView::MakeWithContent(Shared<View> contentView) -> Shared<ButtonView>
{
    auto const view = Make();
    view->SetContent(contentView);
    return view;
}

///
/// @brief Constructor.
///
ButtonView::ButtonView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Set content view.
///
/// @param content 
///
auto ButtonView::SetContent(Shared<View> content) -> void
{
    if (_tapGestureView)
    {
        _tapGestureView->SetContent(content);
    }
}

///
/// @brief Get content view.
///
auto ButtonView::GetContent() -> Shared<View>
{
    return _tapGestureView->GetContent();
}

///
/// @brief 
///
/// @param flags 
///
auto ButtonView::SetActionFlags(ButtonViewActionFlags const flags) -> void
{
    _actionFlags = flags;
}

///
/// @brief
///
auto ButtonView::GetActionFlags() const -> ButtonViewActionFlags
{
    return _actionFlags;
}

///
/// @brief Initialize.
///
auto ButtonView::Initialize() -> void
{
    _tapGestureView = TapGestureView::Make();
    _hoverGestureView = HoverGestureView::MakeWithContent(_tapGestureView);

    AddChildBack(_hoverGestureView);

    EventReceiver::Connect(*this, *this, &ButtonView::ReceiveEvent);
}

///
/// @brief Receive view events.
///
auto ButtonView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewEvent::Notify>())
    {
        auto const sender = event.As<ViewEvent::Notify>()->GetSender();
        if (sender == _tapGestureView || sender == _hoverGestureView)
        {
            if (event.Is<TapGestureViewEvent>())
            {
                if (event.Is<TapGestureViewEvent::Down>())
                {
                    SetDown(true);
                }
                else if (event.Is<TapGestureViewEvent::Up>() || event.Is<TapGestureViewEvent::Cancel>())
                {
                    SetDown(false);
                }
                else if (event.Is<TapGestureViewEvent::Tap>())
                {
                    auto buttonEvent = Event<>(Event<ButtonViewEvent::Press>());
                    SendEventDetached(buttonEvent);
                }
                co_return true;
            }
            else if (event.Is<HoverGestureViewEvent>())
            {
                if (event.Is<HoverGestureViewEvent::Enter>())
                {
                    SetEnter(true);
                }
                else if (event.Is<HoverGestureViewEvent::Leave>())
                {
                    SetEnter(false);
                }
                co_return true;
            }
        }
    }
    co_return false;
}

///
/// @brief Set down state.
///
auto ButtonView::SetDown(const Bool down) -> void
{
    if (_down != down)
    {
        _down = down;

        if (_down)
        {
            auto event = Event<>(Event<ButtonViewEvent::Down>());
            SendEventDetached(event);
        }
        else
        {
            auto event = Event<>(Event<ButtonViewEvent::Up>());
            SendEventDetached(event);
        }
    }
}

///
/// @brief Set hover state.
///
auto ButtonView::SetEnter(const Bool enter) -> void
{
    if (_enter != enter)
    {
        _enter = enter;

        if (_enter)
        {
            auto event = Event<>(Event<ButtonViewEvent::Enter>());
            SendEventDetached(event);
        }
        else
        {
            auto event = Event<>(Event<ButtonViewEvent::Leave>());
            SendEventDetached(event);
        }
    }
}
}
