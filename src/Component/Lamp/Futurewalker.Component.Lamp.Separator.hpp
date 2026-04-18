// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.SeparatorType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.SizedViewType.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A view that draws a separator line.
///
class Separator : public View
{
public:
    static auto Make() -> Shared<Separator>;

    Separator(PassKey<View> key);

    auto GetOrientation() const -> SeparatorOrientation;
    auto SetOrientation(AttributeArg<SeparatorOrientation> const& orientation) -> void;

    auto GetLayoutThickness() const -> Dp;
    auto SetLayoutThickness(AttributeArg<Dp> const& thickness) -> void;

    auto GetDrawingThickness() const -> Dp;
    auto SetDrawingThickness(AttributeArg<Dp> const& thickness) -> void;

    auto GetStartMargin() const -> Dp;
    auto SetStartMargin(AttributeArg<Dp> const& margin) -> void;

    auto GetEndMargin() const -> Dp;
    auto SetEndMargin(AttributeArg<Dp> const& margin) -> void;

    auto GetColor() const -> RGBAColor;
    auto SetColor(AttributeArg<RGBAColor> const& color) -> void;

    auto GetAlpha() const -> Channel;
    auto SetAlpha(AttributeArg<Channel> const& alpha) -> void;

    auto GetCornerRadius() const -> CornerRadius;
    auto SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void;

protected:
    auto Draw(DrawScope& scope) -> void override;
    auto Initialize() -> void override;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    auto InternalUpdateLayout() -> void;

private:
    AttributeAccessor<SeparatorOrientation> _orientation;
    AttributeAccessor<Dp> _layoutThickness;
    AttributeAccessor<Dp> _drawingThickness;
    AttributeAccessor<RGBAColor> _color;
    AttributeAccessor<Channel> _alpha;
    AttributeAccessor<Dp> _startMargin;
    AttributeAccessor<Dp> _endMargin;
    AttributeAccessor<CornerRadius> _cornerRadius;
    Shared<SizedView> _sizedView;
};
}
}
