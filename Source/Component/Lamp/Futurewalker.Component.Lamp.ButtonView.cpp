// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.ButtonView.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderView.hpp"

#include "Futurewalker.Application.ButtonView.hpp"
#include "Futurewalker.Application.ButtonViewEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
auto ButtonView::Make() -> Shared<ButtonView>
{
    return View::MakeDerived<ButtonView>();
}

///
/// @brief
///
/// @param contentView
///
auto ButtonView::MakeWithContent(Shared<View> contentView) -> Shared<ButtonView>
{
    auto view = Make();
    view->SetContent(contentView);
    return view;
}

///
/// @brief
///
auto ButtonView::GetContent() -> Shared<View>
{
    if (_buttonView)
    {
        return _buttonView->GetContent();
    }
    return {};
}

///
/// @brief
///
/// @param content
///
auto ButtonView::SetContent(Shared<View> content) -> void
{
    if (_buttonView)
    {
        _buttonView->SetContent(content);
    }
}

///
/// @brief
///
/// @param flags
///
auto ButtonView::SetActionFlags(ButtonViewActionFlags const flags) -> void
{
    if (_buttonView)
    {
        _buttonView->SetActionFlags(flags);
    }
}

///
/// @brief
///
/// @return
///
auto ButtonView::GetActionFlags() const -> ButtonViewActionFlags
{
    if (_buttonView)
    {
        return _buttonView->GetActionFlags();
    }
    return {};
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetBackgroundColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetBackgroundAlpha(alpha);
    }
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetDisabledBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetDisabledBackgroundColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetDisabledBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetDisabledBackgroundAlpha(alpha);
    }
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetBorderColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetBorderAlpha(alpha);
    }
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetDisabledBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetDisabledBorderColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetDisabledBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetDisabledBorderAlpha(alpha);
    }
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetHighlightColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetHighlightColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetHoverHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetHoverHighlightAlpha(alpha);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetPressHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetPressHighlightAlpha(alpha);
    }
}

///
/// @brief
///
/// @param radius
///
auto ButtonView::SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void
{
    if (_renderView)
    {
        _renderView->SetCornerRadius(radius);
    }
}

///
/// @brief
///
/// @param width
///
auto ButtonView::SetBorderWidth(AttributeArg<Dp> const& width) -> void
{
    if (_renderView)
    {
        _renderView->SetBorderWidth(width);
    }
}

///
/// @brief
///
ButtonView::ButtonView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief
///
auto ButtonView::Initialize() -> void
{
    _buttonView = ::FW_NS::ButtonView::Make();
    _renderView = ButtonRenderView::MakeWithContent(_buttonView);
    AddChildBack(_renderView);

    EventReceiver::Connect(*_buttonView, *this, &ButtonView::ReceiveEvent);
}

///
/// @brief
///
/// @param event
///
auto ButtonView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ButtonViewEvent>())
    {
        if (event.Is<ButtonViewEvent::Enter>())
        {
            SetEnter(true);
        }
        else if (event.Is<ButtonViewEvent::Leave>())
        {
            SetEnter(false);
        }
        else if (event.Is<ButtonViewEvent::Down>())
        {
            SetDown(true);
        }
        else if (event.Is<ButtonViewEvent::Up>())
        {
            SetDown(false);
        }
        co_await SendEvent(event);
        co_return true;
    }
    co_return false;
}

///
/// @brief
///
/// @param down
///
auto ButtonView::SetDown(const Bool down) -> void
{
    if (_renderView)
    {
        _renderView->SetDown(down);
    }
}

///
/// @brief
///
/// @param enter
///
auto ButtonView::SetEnter(const Bool enter) -> void
{
    if (_renderView)
    {
        _renderView->SetEnter(enter);
    }
}
}
}
