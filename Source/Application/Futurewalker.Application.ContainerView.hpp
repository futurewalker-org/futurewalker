// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"
#include "Futurewalker.Application.ClipViewType.hpp"
#include "Futurewalker.Application.BoxViewType.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"
#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Color.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Container view.
///
class ContainerView : public View
{
public:
    static auto Make() -> Shared<ContainerView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<ContainerView>;

    ContainerView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto GetContent() const -> Shared<View const>;
    auto SetContent(Shared<View> const& view) -> void;

    auto SetBackgroundColor(AttributeArg<RGBAColor> color) -> void;
    auto SetBackgroundAlpha(AttributeArg<Channel> alpha) -> void;
    auto SetBorderColor(AttributeArg<RGBAColor> color) -> void;
    auto SetBorderAlpha(AttributeArg<Channel> alpha) -> void;
    auto SetBorderWidth(AttributeArg<Dp> width) -> void;
    auto SetCornerRadius(AttributeArg<CornerRadius> radius) -> void;

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;

private:
    Shared<BoxView> _backgroundBox;
    Shared<ClipView> _clipView;
    Shared<BoxView> _borderBox;
    AttributeAccessor<RGBAColor> _backgroundColor;
    AttributeAccessor<Channel> _backgroundAlpha;
    AttributeAccessor<RGBAColor> _borderColor;
    AttributeAccessor<Channel> _borderAlpha;
    AttributeAccessor<Dp> _borderWidth;
    AttributeAccessor<CornerRadius> _cornerRadius;
};
}
}
