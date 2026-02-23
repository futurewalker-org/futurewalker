// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.PopupFrameViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.PaddingViewType.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Popup frame view.
///
/// Draws frame with shadow and corner radius around its content.
///
/// ### Layout behavior
///
/// PopupFrameView sizes itself to wrap its content plus padding for shadow.
///
class PopupFrameView : public View
{
public:
    static auto Make() -> Shared<PopupFrameView>;
    static auto MakeWithContent(Shared<View> content) -> Shared<PopupFrameView>;

    PopupFrameView(PassKey<View>);

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> content) -> void;

    auto SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void;
    auto SetBlurRadius(AttributeArg<Dp> const& radius) -> void;
    auto SetShadowColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetShadowAlpha(AttributeArg<Channel> const& alpha) -> void;

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<ContainerView> _container;
    Shared<PaddingView> _padding;
    AttributeAccessor<CornerRadius> _cornerRadius;
    AttributeAccessor<Dp> _blurRadius;
    AttributeAccessor<RGBAColor> _shadowColor;
    AttributeAccessor<Channel> _shadowAlpha;
};
}
}
