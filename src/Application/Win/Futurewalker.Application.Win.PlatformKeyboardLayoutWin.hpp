// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformKeyboardLayoutWinType.hpp"
#include "Futurewalker.Application.KeyEventType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.Flags.hpp"

#include <unordered_map>
#include <string>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Manages keyboard layout information.
///
class PlatformKeyboardLayoutWin : NonCopyable
{
public:
    PlatformKeyboardLayoutWin();

    auto UpdateLayout() -> void;

    auto GetKey(DWORD virtualKeyCode, Flags<PlatformModifierFlagWin> modifiers) const -> String;
    auto GetUnmodifiedKey(DWORD virtualKeyCode) const -> String;
    auto GetKeyAndText(HWND hWnd, DWORD virtualKeyCode, Flags<PlatformModifierFlagWin> modifiers) const -> std::pair<String, String>;
    auto GetModifierState() const -> Flags<ModifierKeyFlag>;
    auto GetPlatformModifierState() const -> Flags<PlatformModifierFlagWin>;

private:
    auto MapVirtualKeyCodeToChars(DWORD virtualKeyCode, Flags<PlatformModifierFlagWin> modifiers) const -> Optional<std::wstring>;
    auto MapVirtualKeyCodeToNamedKey(DWORD virtualKeyCode) const -> Optional<String>;
    auto MapControlCharsToNamedKey(std::wstring_view chars) const -> Optional<String>;

private:
    HKL _hkl = NULL;
    std::unordered_map<PlatformModifierFlagWin, std::vector<std::wstring>> _map;
};
}
}
