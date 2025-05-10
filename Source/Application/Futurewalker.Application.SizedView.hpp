// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.SizedViewType.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.View.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A layout view which forces fixed width and height to its child.
///
class SizedView final : public View
{
public:
    static auto Make() -> Shared<SizedView>;
    static auto Make(Dp const width, Dp const height) -> Shared<SizedView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<SizedView>;
    static auto MakeWithContent(Dp const width, Dp const height, Shared<View> const& content) -> Shared<SizedView>;

    auto SetWidth(Dp const width) -> void;
    auto GetWidth() const -> Dp;

    auto SetHeight(Dp const height) -> void;
    auto GetHeight() const -> Dp;

    auto SetContent(Shared<View> const& content) -> void;
    auto GetContent() -> Shared<View>;

    SizedView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;

private:
    auto MeasureAxis(const AxisConstraints& c, Dp const v) const -> Dp;

private:
    Shared<ContainerView> _container;
    Dp _width = 0;
    Dp _height = 0;
};
}
}
