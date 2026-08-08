// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.IconView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.ColorFilter.hpp"

#include "Futurewalker.Geometry.Matrix.hpp"

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

auto IconView::SetUseColor(AttributeArg<Bool> const& useColor) -> void
{
    _useColor.SetAttributeArg(useColor);
}

auto IconView::SetColor(AttributeArg<RGBAColor> const& color) -> void
{
    _color.SetAttributeArg(color);
}

auto IconView::SetAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _alpha.SetAttributeArg(alpha);
}

auto IconView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Icon, AttributeIcon, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeSize, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Bool, AttributeUseColor, {false});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeAlpha, {});

    _icon.BindAndConnectAttributeWithDefaultValue(*this, &IconView::ReceiveAttributeEvent, AttributeIcon, {});
    _size.BindAndConnectAttributeWithDefaultValue(*this, &IconView::ReceiveAttributeEvent, AttributeSize, {0});
    _useColor.BindAndConnectAttributeWithDefaultValue(*this, &IconView::ReceiveAttributeEvent, AttributeUseColor, {false});
    _color.BindAndConnectAttributeWithDefaultValue(*this, &IconView::ReceiveAttributeEvent, AttributeColor, {});
    _alpha.BindAndConnectAttributeWithDefaultValue(*this, &IconView::ReceiveAttributeEvent, AttributeAlpha, {});
}

auto IconView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const& constraints = parameter.GetConstraints();
    auto const icon = _icon.GetValueOrDefault();
    auto const iconSize = icon.IsEmpty() ? 0 : _size.GetValueOr(0);
    auto const measuredSize = BoxConstraints::Constrain(constraints, Size<Dp>(iconSize, iconSize));
    scope.SetMeasuredSize(measuredSize);
}

auto IconView::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();
    auto const icon = _icon.GetValueOrDefault();
    auto const useColor = _useColor.GetValueOr(false);
    auto const color = _color.GetValueOrDefault();
    auto const alpha = _alpha.GetValueOrDefault();

    auto const size = GetContentRect().GetSize();

    if (useColor)
    {
        auto const fillColor = RGBAColor(color.GetRGBColor(), color.GetAlpha().GetF64() * alpha.GetF64());
        auto colorMatrix = Matrix5x5<Float64>();
        colorMatrix.m04 = fillColor.GetRed().GetF64();
        colorMatrix.m14 = fillColor.GetGreen().GetF64();
        colorMatrix.m24 = fillColor.GetBlue().GetF64();
        colorMatrix.m33 = fillColor.GetAlpha().GetF64();
        scene.PushLayer({
            .colorFilter = Graphics::ColorFilter::MakeMatrixFilter(colorMatrix, true),
        });
        icon.Draw(scene, size);
        scene.Pop({});
    }
    else
    {
        if (alpha.GetF64() < 1.0)
        {
            scene.PushLayer({.alpha = alpha, .blendMode = Graphics::BlendMode::SrcOver});
            icon.Draw(scene, size);
            scene.Pop({});
        }
        else
        {
            icon.Draw(scene, size);
        }
    }
}

auto IconView::ReceiveAttributeEvent(Event<>& event) -> Bool
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
    return false;
}
}
