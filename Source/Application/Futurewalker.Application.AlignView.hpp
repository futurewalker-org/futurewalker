// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.AlignViewType.hpp"
#include "Futurewalker.Application.Alignment.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.View.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Align layout.
///
class AlignView final : public View
{
public:
    static auto Make() -> Shared<AlignView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<AlignView>;

    auto GetContent() const -> Shared<View>;
    auto SetContent(Shared<View> const& content) -> void;

    auto GetAlignment() const -> Alignment;
    auto SetAlignment(Alignment const& alignment) -> void;

    AlignView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;

private:
    Shared<ContainerView> _container;
    Alignment _alignment = Alignment::Center();
};
}
}
