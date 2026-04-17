// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformKeyboardLayoutWin.hpp"
#include "Futurewalker.Application.Key.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
namespace
{
auto IsControlChar(WCHAR c) -> Bool
{
    return c < 0x20;
}
}

///
/// @brief Constructor.
///
PlatformKeyboardLayoutWin::PlatformKeyboardLayoutWin()
{
    UpdateLayout();
}

///
/// @brief Update mappings to current keyboard layout.
///
auto PlatformKeyboardLayoutWin::UpdateLayout() -> void
{
    auto const hkl = ::GetKeyboardLayout(0);
    if (_hkl == hkl)
    {
        return;
    }

    _hkl = hkl;
    _map = {};

    auto mappedCount = SInt32(0);

    for (auto i = 0; i <= std::to_underlying(PlatformModifierFlagWin::All); ++i)
    {
        auto const modifiers = static_cast<PlatformModifierFlagWin>(i);

        _map[modifiers] = std::vector<std::wstring>(256);

        auto keyboardState = std::array<BYTE, 256>();

        if ((Flags(modifiers) & PlatformModifierFlagWin::Shift) != PlatformModifierFlagWin::None)
        {
            keyboardState[VK_SHIFT] |= 0x80;
        }

        if ((Flags(modifiers) & PlatformModifierFlagWin::Control) != PlatformModifierFlagWin::None)
        {
            keyboardState[VK_CONTROL] |= 0x80;
        }

        if ((Flags(modifiers) & PlatformModifierFlagWin::LAlt) != PlatformModifierFlagWin::None)
        {
            keyboardState[VK_LMENU] |= 0x80;
        }

        if ((Flags(modifiers) & PlatformModifierFlagWin::RAlt) != PlatformModifierFlagWin::None)
        {
            keyboardState[VK_RMENU] |= 0x80;
        }

        for (auto vkc = 0; vkc < 256; ++vkc)
        {
            // NOTE: ToUnicodeEx() sometimes returns non-printable ASCII control characters.
            // We ignore them so conversions can fallback to search printable variant by dropping some modifiers.
            auto chars = std::array<WCHAR, 5>();
            auto const r = ::ToUnicodeEx(vkc, 0, keyboardState.data(), chars.data(), static_cast<int>(chars.size()), 0, _hkl);
            if (r < 0)
            {
                // Dead key was written to chars.
                // FIXME: Is it OK to assume length is 1?
                if (!IsControlChar(chars[0]))
                {
                    _map[modifiers][vkc] = std::wstring(&chars[0], 1z);
                }
            }
            else if (r > 0)
            {
                if (!IsControlChar(chars[0]))
                {
                    _map[modifiers][vkc] = std::wstring(&chars[0], static_cast<size_t>(r));
                }
            }

            if (r != 0)
            {
                ++mappedCount;
            }
        }
    }

    FW_DEBUG_LOG_INFO("PlatformKeyboardLayoutWin: UpdateLayout() Mapped {} virtual key codes", static_cast<int>(mappedCount));
}

///
/// @brief Get key name.
///
/// @note Does not consume WM_(SYS)CHAR events in message loop.
///
auto PlatformKeyboardLayoutWin::GetKey(DWORD virtualKeyCode, Flags<PlatformModifierFlagWin> modifiers) const -> String
{
    if (auto namedKey = MapVirtualKeyCodeToNamedKey(virtualKeyCode))
    {
        return *namedKey;
    }

    // Search printable representation by dropping modifiers except Shift or AltGr.
    // This behavior follows W3C UI Events KeyEvent specification.

    auto const shift = Flags(PlatformModifierFlagWin::Shift);
    auto const altGr = Flags(PlatformModifierFlagWin::Control) | PlatformModifierFlagWin::RAlt;

    auto modifierCombinations = std::vector<Flags<PlatformModifierFlagWin>>();

    modifierCombinations.push_back(modifiers);

    if ((modifiers & (shift | altGr)) != PlatformModifierFlagWin::None)
    {
        modifierCombinations.push_back(shift | altGr);
    }

    if ((modifiers & shift) != PlatformModifierFlagWin::None)
    {
        modifierCombinations.push_back(shift);
    }

    if ((modifiers & altGr) != PlatformModifierFlagWin::None)
    {
        modifierCombinations.push_back(altGr);
    }

    modifierCombinations.push_back(PlatformModifierFlagWin::None);

    for (auto const& fallbackModifiers : modifierCombinations)
    {
        if (auto const c = MapVirtualKeyCodeToChars(virtualKeyCode, fallbackModifiers))
        {
            return PlatformStringFunctionWin::WideToUtf8(*c);
        }
    }
    return Key::Unidentified;
}

///
/// @brief Get unmodified key name.
///
auto PlatformKeyboardLayoutWin::GetUnmodifiedKey(DWORD virtualKeyCode) const -> String
{
    if (auto const chars = MapVirtualKeyCodeToChars(virtualKeyCode, PlatformModifierFlagWin::None))
    {
        if (auto namedKey = MapControlCharsToNamedKey(*chars))
        {
            return *namedKey;
        }
        return PlatformStringFunctionWin::WideToUtf8(*chars);
    }
    else
    {
        if (auto namedKey = MapVirtualKeyCodeToNamedKey(virtualKeyCode))
        {
            return *namedKey;
        }
    }
    return Key::Unidentified;
}

///
/// @brief Get key name and text value for key down event.
///
/// @note Consumes WM_(SYS)CHAR events in message loop.
///
auto PlatformKeyboardLayoutWin::GetKeyAndText(HWND hWnd, DWORD virtualKeyCode, Flags<PlatformModifierFlagWin> modifiers) const -> std::pair<String, String>
{
    // Collect WM_CHAR and WM_DEACHAR posted by TranslateMessage().
    auto chars = std::wstring();
    {
        auto msg = MSG();
        while (::PeekMessageW(&msg, hWnd, WM_CHAR, WM_CHAR, PM_REMOVE))
        {
            if (msg.message == WM_CHAR)
            {
                FW_DEBUG_LOG_INFO("PlatformKeyboardLayoutWin WM_CHAR {:x}", msg.wParam);
                chars.push_back(static_cast<WCHAR>(msg.wParam));
            }
        }
        while (::PeekMessageW(&msg, hWnd, WM_SYSCHAR, WM_CHAR, PM_REMOVE))
        {
            if (msg.message == WM_SYSCHAR)
            {
                FW_DEBUG_LOG_INFO("PlatformKeyboardLayoutWin WM_SYSCHAR {:x}", msg.wParam);
                chars.push_back(static_cast<WCHAR>(msg.wParam));
            }
        }
    }
    auto deadChars = std::wstring();
    {
        auto msg = MSG();
        while (::PeekMessageW(&msg, hWnd, WM_DEADCHAR, WM_DEADCHAR, PM_REMOVE))
        {
            if (msg.message == WM_DEADCHAR)
            {
                FW_DEBUG_LOG_INFO("PlatformKeyboardLayoutWin WM_DEADCHAR {:x}", msg.wParam);
                deadChars.push_back(static_cast<WCHAR>(msg.wParam));
            }
        }
        while (::PeekMessageW(&msg, hWnd, WM_SYSDEADCHAR, WM_SYSDEADCHAR, PM_REMOVE))
        {
            if (msg.message == WM_SYSDEADCHAR)
            {
                FW_DEBUG_LOG_INFO("PlatformKeyboardLayoutWin WM_SYSDEADCHAR {:x}", msg.wParam);
                deadChars.push_back(static_cast<WCHAR>(msg.wParam));
            }
        }
    }

    auto text = String();
    if (!chars.empty() && !IsControlChar(chars[0])) 
    {
        text = PlatformStringFunctionWin::WideToUtf8(chars);
    }
    else if (!deadChars.empty() && !IsControlChar(deadChars[0]))
    {
        text = PlatformStringFunctionWin::WideToUtf8(deadChars);
    }

    if (auto namedKey = MapControlCharsToNamedKey(chars))
    {
        return {*namedKey, text};
    }

    // We have control char but could not map it to named key.
    // Give up chars and search for printable variant by using VKC. This includes handling of 0x00.
    if (chars.size() == 1 && IsControlChar(chars[0]))
    {
        chars.clear();
    }

    if (chars.empty())
    {
        if (deadChars.empty())
        {
            return {GetKey(virtualKeyCode, modifiers), text};
        }
        else
        {
            return {Key::Dead, text};
        }
    }
    else
    {
        return {PlatformStringFunctionWin::WideToUtf8(chars), text};
    }
}

///
/// @brief Get current state of modifier keys.
///
auto PlatformKeyboardLayoutWin::GetModifierState() const -> Flags<ModifierKeyFlag>
{
    auto flags = Flags(ModifierKeyFlag::None);
    auto testAndSet = [&](auto vkc, auto flag) {
        if (::GetKeyState(vkc) & 0x8000)
        {
            flags |= flag;
        }
    };
    testAndSet(VK_SHIFT, ModifierKeyFlag::Shift);
    testAndSet(VK_CONTROL, ModifierKeyFlag::Control);
    testAndSet(VK_MENU, ModifierKeyFlag::Alt);
    return flags;
}

///
/// @brief Get current state of modifier keys.
///
auto PlatformKeyboardLayoutWin::GetPlatformModifierState() const -> Flags<PlatformModifierFlagWin>
{
    auto flags = Flags(PlatformModifierFlagWin::None);
    auto testAndSet = [&](auto vkc, auto flag) {
        if (::GetKeyState(vkc) & 0x8000)
        {
            flags |= flag;
        }
    };
    testAndSet(VK_SHIFT, PlatformModifierFlagWin::Shift);
    testAndSet(VK_CONTROL, PlatformModifierFlagWin::Control);
    testAndSet(VK_LMENU, PlatformModifierFlagWin::LAlt);
    testAndSet(VK_RMENU, PlatformModifierFlagWin::RAlt);
    return flags;
}

///
/// @brief Map virtual key code to chars.
///
auto PlatformKeyboardLayoutWin::MapVirtualKeyCodeToChars(DWORD virtualKeyCode, Flags<PlatformModifierFlagWin> modifiers) const -> Optional<std::wstring>
{
    if (0 <= virtualKeyCode && virtualKeyCode < 256)
    {
        auto const it = _map.find(static_cast<PlatformModifierFlagWin>(modifiers));
        if (it != _map.end())
        {
            if (virtualKeyCode < std::ssize(it->second))
            {
                if (!it->second[virtualKeyCode].empty())
                {
                    return it->second[virtualKeyCode];
                }
            }
            else
            {
                FW_DEBUG_ASSERT(false);
            }
        }
    }
    return {};
}

///
/// @brief Map virtual key code to named key.
///
auto PlatformKeyboardLayoutWin::MapVirtualKeyCodeToNamedKey(DWORD virtualKeyCode) const -> Optional<String>
{
    switch (virtualKeyCode)
    {
        case VK_BACK:
            return Key::Backspace;
        case VK_TAB:
            return Key::Tab;
        case VK_RETURN:
            return Key::Enter;
        case VK_CAPITAL:
            return Key::CapsLock;
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            return Key::Shift;
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            return Key::Control;
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            return Key::Alt;
        case VK_ESCAPE:
            return Key::Escape;
        case VK_LEFT:
            return Key::ArrowLeft;
        case VK_UP:
            return Key::ArrowUp;
        case VK_RIGHT:
            return Key::ArrowRight;
        case VK_DOWN:
            return Key::ArrowDown;
        case VK_INSERT:
            return Key::Insert;
        case VK_DELETE:
            return Key::Delete;
        case VK_PROCESSKEY:
            return Key::Process;
    }
    return {};
}

///
/// @brief Map control chars to named keys.
///
auto PlatformKeyboardLayoutWin::MapControlCharsToNamedKey(std::wstring_view chars) const -> Optional<String>
{
    if (chars.size() == 1)
    {
        // Map some control characters into named keys.
        switch (chars[0])
        {
            case '\x08':
                return Key::Backspace;
            case '\x09':
                return Key::Tab;
            case '\x0D':
                return Key::Enter;
            case '\x1B':
                return Key::Escape;
            case '\x7F':
                return Key::Delete;
        }
    }
    return {};
}
}
}
