// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.KeyEventType.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.Flags.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Key related functions.
///
struct KeyFunction : NonConstructible
{
    static auto GetModifierKeyStrings(Flags<ModifierKeyFlag> const modifiers) -> std::vector<String>;
};
}
}
