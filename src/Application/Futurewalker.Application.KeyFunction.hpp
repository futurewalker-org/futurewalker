// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.KeyEventType.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

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
    static auto GetModifierKeyStrings(ModifierKeyFlags const modifiers) -> std::vector<String>;
};
}
}
