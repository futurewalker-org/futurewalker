// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Popup.hpp"
#include "Futurewalker.Application.Window.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Create instance of popup.
///
/// @param options Popup options.
/// @param sourceView View which owns the popup.
///
/// @note When source view is not attached to a window, the behavior is unspecified.
///
auto Popup::Make(PopupOptions const& options, Shared<View> const& sourceView) -> Shared<Popup>
{
    auto popup = Shared<Popup>::Make(PassKey<Popup>(), options, sourceView);
    popup->Initialize();
    return popup;
}

///
/// @brief Constructor.
///
Popup::Popup(PassKey<Popup>, PopupOptions const& options, Shared<View> const& sourceView)
  : _options(options)
  , _sourceView(sourceView)
{
}


///
/// @brief Get visibility of popup.
///
auto Popup::IsVisible() const -> Bool
{
    if (_window)
    {
        return _window->IsVisible();
    }
    return false;
}

///
/// @brief Set visibility of popup.
///
auto Popup::SetVisible(Bool const visible) -> void
{
    if (auto window = GetWindow())
    {
        window->SetVisible(visible);
    }
}

///
/// @brief Get closed state of popup.
///
auto Popup::IsClosed() const -> Bool
{
    if (_window)
    {
        return _window->IsClosed();
    }
    return true;
}

///
/// @brief Close popup.
///
auto Popup::Close() -> Async<void>
{
    if (_window)
    {
        co_await _window->RequestClose();
    }
}

///
/// @brief Get content view.
///
auto Popup::GetContent() -> Shared<View>
{
    return _contentView;
}

///
/// @brief Get content view.
///
auto Popup::GetContent() const -> Shared<View const>
{
    return _contentView;
}

///
/// @brief Set content view.
///
/// @param contentView 
///
auto Popup::SetContent(Shared<View> const& contentView) -> void
{
    if (_contentView != contentView)
    {
        _contentView = contentView;

        if (_window)
        {
            _window->SetContent(contentView);
        }
    }
}

///
/// @brief Get desired size of popup under given constraints.
///
/// @param constraints Size constraints.
///
auto Popup::Measure(BoxConstraints const& constraints) -> Size<Dp>
{
    if (auto window = GetWindow())
    {
        return window->MeasureSize(constraints);
    }
    return {};
}

///
/// @brief Set frame rectangle of popup.
///
/// @param rect New frame rectangle of the popup.
///
auto Popup::SetFrameRect(Rect<Vp> const& rect) -> void
{
    if (auto window = GetWindow())
    {
        window->SetFrameRect(rect);
    }
}

///
/// @brief Set background color of popup.
///
/// @param backgroundColor New background color of the popup.
///
auto Popup::SetBackgroundColor(AttributeArg<RGBAColor> const& backgroundColor) -> void
{
    if (auto window = GetWindow())
    {
        window->SetBackgroundColor(backgroundColor);
    }
}

///
/// @brief
///
auto Popup::GetAttributeNode() -> AttributeNode&
{
    return _window->GetAttributeNode();
}

///
/// @brief
///
auto Popup::GetAttributeNode() const -> AttributeNode const&
{
    return _window->GetAttributeNode();
}

///
/// @brief
///
auto Popup::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

///
/// @brief
///
auto Popup::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

///
/// @brief
///
auto Popup::GetTracker() -> Tracker&
{
    return GetEventReceiver().GetTracker();
}

///
/// @brief
///
auto Popup::GetTracker() const -> Tracker const&
{
    return GetEventReceiver().GetTracker();
}

///
/// @brief
///
auto Popup::Initialize() -> void
{
    _eventReceiver = EventReceiver::Make();
}

///
/// @brief Get or create window for the popup.
///
auto Popup::GetWindow() -> Shared<Window>
{
    if (_window)
    {
        return _window;
    }

    auto const windowOptions = WindowOptions {
        .behavior = WindowBehavior::Popup,
        .backgroundStyle = WindowBackgroundStyle::Transparent,
        .closable = false,
        .resizable = false,
        .minimizable = false,
        .hasFrame = _options.hasFrame,
        .hasTitleBar = false,
        .allowActiveOwnerPopup = _options.allowActiveOwnerPopup,
    };

    if (auto source = _sourceView.Lock())
    {
        _window = source->MakeOwnedWindow(windowOptions);
    }
    else
    {
        _window = Window::Make(windowOptions);
    }
    _window->SetContent(_contentView);
    EventReceiver::Connect(*_window, *this, &Popup::ReceiveEvent);
    return _window;
}

///
/// @brief
///
auto Popup::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<WindowEvent::Closed>())
    {
        _window = nullptr;
        auto closedEvent = Event<>(Event<PopupEvent::Closed>());
        co_await GetEventReceiver().SendEvent(closedEvent);
    }
    co_return false;
}
}
