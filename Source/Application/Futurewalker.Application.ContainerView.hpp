// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.View.hpp"

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

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto ReceiveEvent(Event& event) -> Async<Bool>;

private:
    Weak<View> _content;
    AttributeAccessor<RGBAColor> _backgroundColor;
};
}
}
