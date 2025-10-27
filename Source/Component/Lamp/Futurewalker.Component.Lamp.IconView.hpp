// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.IconViewType.hpp"
#include "Futurewalker.Component.Lamp.IconViewStyle.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.Icon.hpp"
#include "Futurewalker.Application.IconViewType.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class IconView : public View
{
public:
    static auto Make() -> Shared<IconView>;
    static auto MakeWithIcon(AttributeArg<Icon> const& icon) -> Shared<IconView>;

    auto GetIcon() const -> Icon;
    auto SetIcon(AttributeArg<Icon> const& icon) -> void;

    IconView(PassKey<View> key);

protected:
    auto Initialize() -> void override;

private:
    Shared<::FW_NS::IconView> _iconView;
};
}
}
