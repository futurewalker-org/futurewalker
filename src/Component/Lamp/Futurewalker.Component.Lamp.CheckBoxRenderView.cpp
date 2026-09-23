// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.CheckBoxRenderView.hpp"

#include "Futurewalker.Application.ViewDrawFunction.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

#include "Futurewalker.Resource.ResourceManager.hpp"
#include "Futurewalker.Resource.Resource.hpp"

#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.SvgDocument.hpp"
#include "Futurewalker.Graphics.ColorFilter.hpp"

#include "Resource/Futurewalker.Component.Lamp.hpp"
#include "Resource/Futurewalker.Component.Lamp.Icon.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto CheckBoxRenderView::Make() -> Shared<CheckBoxRenderView>
{
    return View::MakeDerived<CheckBoxRenderView>();
}

CheckBoxRenderView::CheckBoxRenderView(PassKey<View> key)
  : View(key)
{
}

auto CheckBoxRenderView::SetState(CheckBoxState const state) -> void
{
    if (_state != state)
    {
        _state = state;
        InvalidateVisual();
    }
}

auto CheckBoxRenderView::SetDown(Bool const down) -> void
{
    if (_down != down)
    {
        _down = down;
        InvalidateVisual();
    }
}

auto CheckBoxRenderView::SetEnter(Bool const enter) -> void
{
    if (_enter != enter)
    {
        _enter = enter;
        InvalidateVisual();
    }
}

auto CheckBoxRenderView::SetFocused(Bool const focused, FocusReason const reason) -> void
{
    if (_focused != focused || _focusReason != reason)
    {
        _focused = focused;
        _focusReason = reason;
        InvalidateVisual();
    }
}

auto CheckBoxRenderView::SetEnabled(Bool const enabled) -> void
{
    if (_enabled != enabled)
    {
        _enabled = enabled;
        InvalidateVisual();
    }
}

auto CheckBoxRenderView::SetCornerRadius(AttributeArg<CornerRadius> const& cornerRadius) -> void
{
    _cornerRadius.SetAttributeArg(cornerRadius);
}

auto CheckBoxRenderView::SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    _backgroundColor.SetAttributeArg(color);
}

auto CheckBoxRenderView::SetBackgroundColorDisabled(AttributeArg<RGBAColor> const& color) -> void
{
    _backgroundColorDisabled.SetAttributeArg(color);
}

auto CheckBoxRenderView::SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _backgroundAlpha.SetAttributeArg(alpha);
}

auto CheckBoxRenderView::SetBackgroundAlphaDisabled(AttributeArg<Channel> const& alpha) -> void
{
    _backgroundAlphaDisabled.SetAttributeArg(alpha);
}

auto CheckBoxRenderView::SetBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    _borderColor.SetAttributeArg(color);
}

auto CheckBoxRenderView::SetBorderColorDisabled(AttributeArg<RGBAColor> const& color) -> void
{
    _borderColorDisabled.SetAttributeArg(color);
}

auto CheckBoxRenderView::SetBorderColorFocused(AttributeArg<RGBAColor> const& color) -> void
{
    _borderColorFocused.SetAttributeArg(color);
}

auto CheckBoxRenderView::SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _borderAlpha.SetAttributeArg(alpha);
}

auto CheckBoxRenderView::SetBorderAlphaDisabled(AttributeArg<Channel> const& alpha) -> void
{
    _borderAlphaDisabled.SetAttributeArg(alpha);
}

auto CheckBoxRenderView::SetBorderAlphaFocused(AttributeArg<Channel> const& alpha) -> void
{
    _borderAlphaFocused.SetAttributeArg(alpha);
}

auto CheckBoxRenderView::SetCheckMarkColor(AttributeArg<RGBAColor> const& color) -> void
{
    _checkMarkColor.SetAttributeArg(color);
}

auto CheckBoxRenderView::SetCheckMarkColorDisabled(AttributeArg<RGBAColor> const& color) -> void
{
    _checkMarkColorDisabled.SetAttributeArg(color);
}

auto CheckBoxRenderView::SetCheckMarkAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _checkMarkAlpha.SetAttributeArg(alpha);
}

auto CheckBoxRenderView::SetCheckMarkAlphaDisabled(AttributeArg<Channel> const& alpha) -> void
{
    _checkMarkAlphaDisabled.SetAttributeArg(alpha);
}

auto CheckBoxRenderView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBackgroundColorDisabled, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBackgroundAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBackgroundAlphaDisabled, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBorderColorDisabled, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBorderColorFocused, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBorderAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBorderAlphaDisabled, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBorderAlphaFocused, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeCheckMarkColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeCheckMarkColorDisabled, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeCheckMarkAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeCheckMarkAlphaDisabled, {});

    _cornerRadius.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeCornerRadius);
    _backgroundColor.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBackgroundColor);
    _backgroundColorDisabled.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBackgroundColorDisabled);
    _backgroundAlpha.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBackgroundAlpha);
    _backgroundAlphaDisabled.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBackgroundAlphaDisabled);
    _borderColor.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBorderColor);
    _borderColorDisabled.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBorderColorDisabled);
    _borderColorFocused.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBorderColorFocused);
    _borderAlpha.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBorderAlpha);
    _borderAlphaDisabled.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBorderAlphaDisabled);
    _borderAlphaFocused.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeBorderAlphaFocused);
    _checkMarkColor.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeCheckMarkColor);
    _checkMarkColorDisabled.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeCheckMarkColorDisabled);
    _checkMarkAlpha.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeCheckMarkAlpha);
    _checkMarkAlphaDisabled.BindAndConnectAttribute(*this, &CheckBoxRenderView::ReceiveAttributeEvent, AttributeCheckMarkAlphaDisabled);

    auto resource = ResourceManager::GetResource(M::Futurewalker::Component::Lamp);
    auto loadIcon = [&](auto const iconId) {
        auto file = resource->LoadFile(iconId);
        auto svg = Graphics::SvgDocument::LoadFromStream(file);
        return Icon::MakeFromSvgDocument(std::move(svg));
    };
    _checkMarkIcon = loadIcon(R::Futurewalker::Component::Lamp::Icon::Check);
    _indeterminateIcon = loadIcon(R::Futurewalker::Component::Lamp::Icon::Minus);
}

auto CheckBoxRenderView::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    auto const state = _state;
    auto const rect = GetContentRect();
    auto const cornerRadius = _cornerRadius.GetValueOrDefault();
    auto const backgroundColor = InternalGetBackgroundColor();
    auto const borderColor = InternalGetBorderColor();
    auto const borderColorFocused = InternalGetBorderColorFocused();
    auto const checkMarkColor = InternalGetCheckMarkColor();
    auto const layoutDirection = GetLayoutDirection();
    auto const borderWidth = 2.0;

    if (state == CheckBoxState::Unchecked)
    {
        ViewDrawFunction::DrawRoundRectBorder(scene, rect, cornerRadius, borderColor, borderWidth, layoutDirection);
    }
    else if (state == CheckBoxState::Checked)
    {
        ViewDrawFunction::DrawRoundRect(scene, rect, cornerRadius, backgroundColor, layoutDirection);
        auto colorMatrix = Matrix5x5<Float64>();
        colorMatrix.m04 = checkMarkColor.GetRed().GetF64();
        colorMatrix.m14 = checkMarkColor.GetGreen().GetF64();
        colorMatrix.m24 = checkMarkColor.GetBlue().GetF64();
        colorMatrix.m33 = checkMarkColor.GetAlpha().GetF64();
        scene.PushLayer({
            .colorFilter = Graphics::ColorFilter::MakeMatrixFilter(colorMatrix, true),
        });
        _checkMarkIcon.Draw(scene, rect.GetSize());
        scene.Pop({});
    }
    else if (state == CheckBoxState::Indeterminate)
    {
        ViewDrawFunction::DrawRoundRect(scene, rect, cornerRadius, backgroundColor, layoutDirection);
        auto colorMatrix = Matrix5x5<Float64>();
        colorMatrix.m04 = checkMarkColor.GetRed().GetF64();
        colorMatrix.m14 = checkMarkColor.GetGreen().GetF64();
        colorMatrix.m24 = checkMarkColor.GetBlue().GetF64();
        colorMatrix.m33 = checkMarkColor.GetAlpha().GetF64();
        scene.PushLayer({
            .colorFilter = Graphics::ColorFilter::MakeMatrixFilter(colorMatrix, true),
        });
        _indeterminateIcon.Draw(scene, rect.GetSize());
        scene.Pop({});
    }

    if (_focused && _focusReason == FocusReason::Keyboard)
    {
        auto const offset = borderWidth / 2.0 + 2.0;
        auto const offsetRadius = CornerRadius::Offset(cornerRadius, offset);
        ViewDrawFunction::DrawRoundRectBorder(scene, Rect<Dp>::Inflate(rect, offset, offset), offsetRadius, borderColorFocused, borderWidth, layoutDirection);
    }
}

auto CheckBoxRenderView::ReceiveAttributeEvent(Event<>& event) -> Bool
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateVisual();
    }
    return false;
}

auto CheckBoxRenderView::InternalGetBackgroundColor() const -> RGBAColor
{
    if (_enabled)
    {
        return _backgroundColor.GetValueOrDefault().WithAlphaMultiplied(_backgroundAlpha.GetValueOrDefault());
    }
    return _backgroundColorDisabled.GetValueOrDefault().WithAlphaMultiplied(_backgroundAlphaDisabled.GetValueOrDefault());
}

auto CheckBoxRenderView::InternalGetBorderColor() const -> RGBAColor
{
    if (_enabled)
    {
        return _borderColor.GetValueOrDefault().WithAlphaMultiplied(_borderAlpha.GetValueOrDefault());
    }
    return _borderColorDisabled.GetValueOrDefault().WithAlphaMultiplied(_borderAlphaDisabled.GetValueOrDefault());
}

auto CheckBoxRenderView::InternalGetCheckMarkColor() const -> RGBAColor
{
    if (_enabled)
    {
        return _checkMarkColor.GetValueOrDefault().WithAlphaMultiplied(_checkMarkAlpha.GetValueOrDefault());
    }
    return _checkMarkColorDisabled.GetValueOrDefault().WithAlphaMultiplied(_checkMarkAlphaDisabled.GetValueOrDefault());
}

auto CheckBoxRenderView::InternalGetBorderColorFocused() const -> RGBAColor
{
    return _borderColorFocused.GetValueOrDefault().WithAlphaMultiplied(_borderAlphaFocused.GetValueOrDefault());
}
}
