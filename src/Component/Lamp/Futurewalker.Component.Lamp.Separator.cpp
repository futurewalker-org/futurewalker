// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.Separator.hpp"

#include "Futurewalker.Application.SizedView.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

#include "Futurewalker.Graphics.Scene.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto Separator::Make() -> Shared<Separator>
{
    return View::MakeDerived<Separator>();
}

Separator::Separator(PassKey<View> key)
  : View(key)
{
}

auto Separator::GetOrientation() const -> SeparatorOrientation
{
    return _orientation.GetValueOr(SeparatorOrientation::Horizontal);
}

auto Separator::SetOrientation(AttributeArg<SeparatorOrientation> const& orientation) -> void
{
    _orientation.SetAttributeArg(orientation);
}

auto Separator::GetLayoutThickness() const -> Dp
{
    return _layoutThickness.GetValueOr(0);
}

auto Separator::SetLayoutThickness(AttributeArg<Dp> const& thickness) -> void
{
    _layoutThickness.SetAttributeArg(thickness);
}

auto Separator::GetDrawingThickness() const -> Dp
{
    return _drawingThickness.GetValueOr(0);
}

auto Separator::SetDrawingThickness(AttributeArg<Dp> const& thickness) -> void
{
    _drawingThickness.SetAttributeArg(thickness);
}

auto Separator::GetLeadingMargin() const -> Dp
{
    return _leadingMargin.GetValueOr(0);
}

auto Separator::SetLeadingMargin(AttributeArg<Dp> const& margin) -> void
{
    _leadingMargin.SetAttributeArg(margin);
}

auto Separator::GetTrailingMargin() const -> Dp
{
    return _trailingMargin.GetValueOr(0);
}

auto Separator::SetTrailingMargin(AttributeArg<Dp> const& margin) -> void
{
    _trailingMargin.SetAttributeArg(margin);
}

auto Separator::GetColor() const -> RGBAColor
{
    return _color.GetValueOrDefault();
}

auto Separator::SetColor(AttributeArg<RGBAColor> const& color) -> void
{
    _color.SetAttributeArg(color);
}

auto Separator::GetAlpha() const -> Channel
{
    return _alpha.GetValueOrDefault();
}

auto Separator::SetAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _alpha.SetAttributeArg(alpha);
}

auto Separator::GetCornerRadius() const -> CornerRadius
{
    return _cornerRadius.GetValueOrDefault();
}

auto Separator::SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void
{
    _cornerRadius.SetAttributeArg(radius);
}

auto Separator::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(SeparatorOrientation, AttributeOrientation, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeLayoutThickness, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeDrawingThickness, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeLeadingMargin, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeTrailingMargin, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});

    _orientation.BindAndConnectAttributeWithDefaultValue(*this, &Separator::ReceiveAttributeEvent, AttributeOrientation, SeparatorOrientation::Horizontal);
    _layoutThickness.BindAndConnectAttributeWithDefaultValue(*this, &Separator::ReceiveAttributeEvent, AttributeLayoutThickness, {0});
    _drawingThickness.BindAndConnectAttributeWithDefaultValue(*this, &Separator::ReceiveAttributeEvent, AttributeDrawingThickness, {0});
    _color.BindAndConnectAttributeWithDefaultValue(*this, &Separator::ReceiveAttributeEvent, AttributeColor, {});
    _alpha.BindAndConnectAttributeWithDefaultValue(*this, &Separator::ReceiveAttributeEvent, AttributeAlpha, {});
    _leadingMargin.BindAndConnectAttributeWithDefaultValue(*this, &Separator::ReceiveAttributeEvent, AttributeLeadingMargin, {0});
    _trailingMargin.BindAndConnectAttributeWithDefaultValue(*this, &Separator::ReceiveAttributeEvent, AttributeTrailingMargin, {0});
    _cornerRadius.BindAndConnectAttributeWithDefaultValue(*this, &Separator::ReceiveAttributeEvent, AttributeCornerRadius, {});

    _sizedView = SizedView::Make();
    AddChildBack(_sizedView);

    InternalUpdateLayout();
}

auto Separator::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    auto const orientation = GetOrientation();
    auto const thickness = GetDrawingThickness();
    auto const color = GetColor().WithAlphaMultiplied(GetAlpha());
    auto const cornerRadius = GetCornerRadius();
    auto const leadingMargin = GetLeadingMargin();
    auto const trailingMargin = GetTrailingMargin();

    auto const layoutDirection = GetLayoutDirection();
    auto const contentRect = GetContentRect();

    auto const totalMargin = leadingMargin + trailingMargin;

    auto drawingRect = Rect<Dp>();
    if (orientation == SeparatorOrientation::Horizontal)
    {
        auto const x = contentRect.x0 + leadingMargin;
        auto const y = contentRect.y0 + (contentRect.GetHeight() - thickness) / 2;
        drawingRect = Rect<Dp>(x, y, x + contentRect.GetWidth() - totalMargin, y + thickness);
    }
    else
    {
        auto const x = contentRect.x0 + (contentRect.GetWidth() - thickness) / 2;
        auto const y = contentRect.y0 + leadingMargin;
        drawingRect = Rect<Dp>(x, y, x + thickness, y + contentRect.GetHeight() - totalMargin);
    }

    if (!drawingRect.IsEmpty())
    {
        scene.AddRoundRect({
            .roundRect = cornerRadius.GetRoundRect(drawingRect, layoutDirection),
            .color = color,
            .drawStyle = Graphics::DrawStyle::Fill,
            .blendMode = Graphics::BlendMode::SrcOver,
            .antiAlias = true,
        });
    }
}

auto Separator::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        auto const id = event.As<AttributeEvent::ValueChanged>()->GetId();
        if (id == _orientation.GetAttributeId() || id == _layoutThickness.GetAttributeId())
        {
            InternalUpdateLayout();
        }
        InvalidateVisual();
    }
    co_return false;
}

auto Separator::InternalUpdateLayout() -> void
{
    auto const orientation = GetOrientation();
    if (orientation == SeparatorOrientation::Horizontal)
    {
        _sizedView->SetWidth(Dp::Infinity());
        _sizedView->SetHeight(GetLayoutThickness());
    }
    else
    {
        _sizedView->SetWidth(GetLayoutThickness());
        _sizedView->SetHeight(Dp::Infinity());
    }
}
}
