// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PaddingViewType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Padding view.
///
class PaddingView : public View
{
public:
    static auto Make() -> Shared<PaddingView>;
    static auto MakeWithPadding(EdgeInsets const& padding) -> Shared<PaddingView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<PaddingView>;
    static auto MakeWithPaddingAndContent(EdgeInsets const& padding, Shared<View> const& content) -> Shared<PaddingView>;

    auto GetPadding() const -> EdgeInsets;
    auto SetPadding(EdgeInsets const& padding) -> void;

    auto GetContent() -> Shared<View>;
    auto GetContent() const -> Shared<View const>;
    auto SetContent(Shared<View> const& content) -> void;

    PaddingView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;

private:
    Shared<ContainerView> _container;
    EdgeInsets _padding;
};
}
}
