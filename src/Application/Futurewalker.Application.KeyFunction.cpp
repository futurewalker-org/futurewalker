// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.KeyFunction.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Get modifier key strings.
///
/// @param modifiers modifier key flags.
///
/// @return Array of modifier key strings.
///
auto KeyFunction::GetModifierKeyStrings(Flags<ModifierKeyFlag> const modifiers) -> std::vector<String>
{
    auto modifierString = std::vector<String>();
    if ((modifiers & ModifierKeyFlag::Shift) != ModifierKeyFlag::None)
    {
        modifierString.push_back(String(u8"Shift"));
    }
    if ((modifiers & ModifierKeyFlag::Control) != ModifierKeyFlag::None)
    {
        modifierString.push_back(String(u8"Ctrl"));
    }
    if ((modifiers & ModifierKeyFlag::Alt) != ModifierKeyFlag::None)
    {
        modifierString.push_back(String(u8"Alt"));
    }
    return modifierString;
}
}
