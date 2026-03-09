// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ClipViewType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A view that clips its content to bounds.
///
/// ### Layout behavior
///
/// ClipView sizes itself to wrap its content.
///
class ClipView : public View
{
public:
    static auto Make() -> Shared<ClipView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<ClipView>;

    ClipView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto GetContent() const -> Shared<View const>;
    auto SetContent(Shared<View> const& content) -> void;

    auto SetCornerRadius(AttributeArg<CornerRadius> radius) -> void;

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    AttributeAccessor<CornerRadius> _cornerRadius;
};
}
}
