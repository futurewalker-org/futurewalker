// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.TextButtonThemeType.hpp"

#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.Theme.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.ReferenceArg.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class TextButtonTheme final : public Theme
{
public:
    enum class Type
    {
        Filled,
        Tonal,
        Outlined,
        Text,
    };

    explicit TextButtonTheme(Type const type);

    auto Apply(AttributeNode& target) -> void override;

private:
    Type _type = Type::Filled;
};
}
}
