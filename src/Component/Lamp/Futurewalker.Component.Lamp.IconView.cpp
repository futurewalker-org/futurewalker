// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.IconView.hpp"

#include "Futurewalker.Application.IconView.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto IconView::Make() -> Shared<IconView>
{
    return View::MakeDerived<IconView>();
}

auto IconView::MakeWithIcon(AttributeArg<Icon> const& icon) -> Shared<IconView>
{
    auto iconView = IconView::Make();
    iconView->SetIcon(icon);
    return iconView;
}

auto IconView::GetIcon() const -> Icon
{
    if (_iconView)
    {
        return _iconView->GetIcon();
    }
    return {};
}

auto IconView::SetIcon(AttributeArg<Icon> const& icon) -> void
{
    if (_iconView)
    {
        _iconView->SetIcon(icon);
    }
}

IconView::IconView(PassKey<View> key)
  : View(key)
{
}

auto IconView::Initialize() -> void
{
    _iconView = ::FW_NS::IconView::Make();
    AddChildBack(_iconView);
    UpdateStyle();
    EventReceiver::Connect(*this, *this, &IconView::ReceiveEvent);
}

auto IconView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewEvent::EnabledChanged>())
    {
        UpdateStyle();
    }
    co_return false;
}

auto IconView::UpdateStyle() -> void
{
    _iconView->SetSize(IconViewStyle::Size);

    if (IsEnabledFromRoot())
    {
        _iconView->SetColor(IconViewStyle::Color);
        _iconView->SetAlpha(IconViewStyle::Alpha);
    }
    else
    {
        _iconView->SetColor(IconViewStyle::DisabledColor);
        _iconView->SetAlpha(IconViewStyle::DisabledAlpha);
    }
}
}
