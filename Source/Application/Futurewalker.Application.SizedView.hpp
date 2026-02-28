// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.SizedViewType.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A layout view which forces fixed width and height to its child.
///
/// ### Layout behavior
///
/// If width/height is set to a finite number greater than or equal to zero, the SizedView will force its content to have the specified width/height.  
/// If width/height is set to positive infinity, the SizedView will fill the available space in that axis.  
/// Otherwise, the SizedView will size itself to wrap its content in that axis.  
///
class SizedView final : public View
{
public:
    static auto Make() -> Shared<SizedView>;
    static auto Make(AttributeArg<Dp> const& width, AttributeArg<Dp> const& height) -> Shared<SizedView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<SizedView>;
    static auto MakeWithContent(AttributeArg<Dp> const& width, AttributeArg<Dp> const& height, Shared<View> const& content) -> Shared<SizedView>;

    auto SetWidth(AttributeArg<Dp> const& width) -> void;
    auto GetWidth() const -> Dp;

    auto SetHeight(AttributeArg<Dp> const& height) -> void;
    auto GetHeight() const -> Dp;

    auto SetContent(Shared<View> const& content) -> void;
    auto GetContent() -> Shared<View>;

    SizedView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    auto MeasureAxis(const AxisConstraints& c, Optional<Dp> const& v) const -> Optional<Dp>;
    auto GetLayoutWidth() const -> Optional<Dp>;
    auto GetLayoutHeight() const -> Optional<Dp>;

private:
    Shared<ContainerView> _container;
    AttributeAccessor<Dp> _width;
    AttributeAccessor<Dp> _height;
};
}
}
