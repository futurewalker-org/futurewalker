// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.PopupFrameView.hpp"

#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.PaddingView.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp"

#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.MaskFilter.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto PopupFrameView::Make() -> Shared<PopupFrameView>
{
    return View::MakeDerived<PopupFrameView>();
}

auto PopupFrameView::MakeWithContent(Shared<View> content) -> Shared<PopupFrameView>
{
    auto popupFrame = Make();
    popupFrame->SetContent(content);
    return popupFrame;
}

PopupFrameView::PopupFrameView(PassKey<View> key)
  : View(key)
{
}

auto PopupFrameView::GetContent() -> Shared<View>
{
    if (_container)
    {
        return _container->GetContent();
    }
    return {};
}

auto PopupFrameView::SetContent(Shared<View> content) -> void
{
    if (_container)
    {
        _container->SetContent(content);
    }
}

auto PopupFrameView::SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void
{
    _cornerRadius.SetAttributeArg(radius);
}

auto PopupFrameView::SetBlurRadius(AttributeArg<Dp> const& radius) -> void
{
    _blurRadius.SetAttributeArg(radius);
}

auto PopupFrameView::SetShadowColor(AttributeArg<RGBAColor> const& color) -> void
{
    _shadowColor.SetAttributeArg(color);
}

auto PopupFrameView::SetShadowAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _shadowAlpha.SetAttributeArg(alpha);
}

auto PopupFrameView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeBlurRadius, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeShadowColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeShadowAlpha, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(EdgeInsets, AttributePadding, [](Dp const d) { return EdgeInsets::MakeUniform(d); }, AttributeBlurRadius);

    _cornerRadius.BindAndConnectAttributeWithDefaultValue(*this, &PopupFrameView::ReceiveAttributeEvent, AttributeCornerRadius, {});
    _blurRadius.BindAndConnectAttributeWithDefaultValue(*this, &PopupFrameView::ReceiveAttributeEvent, AttributeBlurRadius, {0});
    _shadowColor.BindAndConnectAttributeWithDefaultValue(*this, &PopupFrameView::ReceiveAttributeEvent, AttributeShadowColor, {});
    _shadowAlpha.BindAndConnectAttributeWithDefaultValue(*this, &PopupFrameView::ReceiveAttributeEvent, AttributeShadowAlpha, {1});

    _container = ContainerView::Make();
    _container->SetCornerRadius(AttributeCornerRadius);
    _padding = PaddingView::MakeWithContent(_container);
    _padding->SetPadding(AttributePadding);
    AddChildBack(_padding);
}

auto PopupFrameView::Draw(DrawScope& scope) -> void
{
    auto const blurRadius = _blurRadius.GetValueOr({0});
    auto const cornerRadius = _cornerRadius.GetValueOrDefault();
    auto const shadowColor = _shadowColor.GetValueOrDefault();
    auto const shadowAlpha = _shadowAlpha.GetValueOrDefault();
    auto const containerRect = _container->LocalToAncestorRect(_container->GetContentRect(), *this);
    auto const direction = GetLayoutDirection();
    auto const blurStyle = Graphics::BlurStyle::Outer;

    auto& scene = scope.GetScene();
    ViewDrawFunction::DrawRoundRectShadow(scene, containerRect, cornerRadius, shadowColor.WithAlphaMultiplied(shadowAlpha), blurRadius, blurStyle, direction);
}

auto PopupFrameView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateVisual();
    }
    co_return false;
}
}
