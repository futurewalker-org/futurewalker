// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.IconView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

#include "Futurewalker.Graphics.Scene.hpp"

namespace FW_DETAIL_NS
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

IconView::IconView(PassKey<View> key)
  : View(key)
{
}

auto IconView::GetIcon() const -> Icon
{
    return _icon.GetValueOrDefault();
}

auto IconView::SetIcon(AttributeArg<Icon> const& icon) -> void
{
    _icon.SetAttributeArg(icon);
}

auto IconView::SetSize(AttributeArg<Dp> const& size) -> void
{
    _size.SetAttributeArg(size);
}

auto IconView::SetColor(AttributeArg<RGBAColor> const& color) -> void
{
    _color.SetAttributeArg(color);
}

auto IconView::SetAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _alpha.SetAttributeArg(alpha);
}

auto IconView::SetDisabledColor(AttributeArg<RGBAColor> const& color) -> void
{
    _disabledColor.SetAttributeArg(color);
}

auto IconView::SetDisabledAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _disabledAlpha.SetAttributeArg(alpha);
}

auto IconView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Icon, AttributeIcon, Icon());
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeSize, Dp(0.0));
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeDisabledColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeDisabledAlpha, {});

    auto bindAndConnectAttribute = [this]<class Accessor>(Accessor& accessor, StaticAttributeRef<typename Accessor::ValueType> attribute) {
        accessor.BindAttribute(*this, attribute);
        EventReceiver::Connect(accessor, *this, &IconView::ReceiveAttributeEvent);
    };
    bindAndConnectAttribute(_icon, AttributeIcon);
    bindAndConnectAttribute(_size, AttributeSize);
    bindAndConnectAttribute(_color, AttributeColor);
    bindAndConnectAttribute(_alpha, AttributeAlpha);
    bindAndConnectAttribute(_disabledColor, AttributeDisabledColor);
    bindAndConnectAttribute(_disabledAlpha, AttributeDisabledAlpha);
}

auto IconView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const& constraints = parameter.GetConstraints();
    auto const iconSize = _size.GetValueOr(0);
    auto const measuredSize = BoxConstraints::Constrain(constraints, Size<Dp>(iconSize, iconSize));
    scope.SetMeasuredSize(measuredSize);
}

auto IconView::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();
    auto const icon = _icon.GetValueOrDefault();
    auto const color = GetIconColor();
    icon.Draw(scene, GetContentRect(), color);
}

auto IconView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        auto const id = event.As<AttributeEvent::ValueChanged>()->GetId();
        if (id == _size.GetAttributeId())
        {
            InvalidateLayout();
        }
        InvalidateVisual();
    }
    co_return false;
}

auto IconView::GetIconColor() const -> RGBAColor
{
    if (IsEnabledFromRoot())
    {
        auto const color = _color.GetValueOrDefault();
        auto const alpha = _alpha.GetValueOrDefault();
        return RGBAColor(color.GetRed(), color.GetGreen(), color.GetBlue(), alpha.GetF64() * color.GetAlpha().GetF64());
    }
    else
    {
        auto const color = _disabledColor.GetValueOrDefault();
        auto const alpha = _disabledAlpha.GetValueOrDefault();
        return RGBAColor(color.GetRed(), color.GetGreen(), color.GetBlue(), alpha.GetF64() * color.GetAlpha().GetF64());
    }
}
}
