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
auto KeyFunction::GetModifierKeyStrings(ModifierKeyFlags const modifiers) -> std::vector<String>
{
    auto modifierString = std::vector<String>();
    if ((modifiers & ModifierKeyFlags::Shift) != ModifierKeyFlags::None)
    {
        modifierString.push_back(String(u8"Shift"));
    }
    if ((modifiers & ModifierKeyFlags::Control) != ModifierKeyFlags::None)
    {
        modifierString.push_back(String(u8"Ctrl"));
    }
    if ((modifiers & ModifierKeyFlags::Alt) != ModifierKeyFlags::None)
    {
        modifierString.push_back(String(u8"Alt"));
    }
    return modifierString;
}
}
