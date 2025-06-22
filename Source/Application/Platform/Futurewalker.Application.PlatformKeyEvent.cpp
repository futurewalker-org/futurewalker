// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformKeyEvent.hpp"

namespace FW_DETAIL_NS
{
auto PlatformKeyEvent::GetTimestamp() const -> MonotonicTime
{
    return _timestamp;
}

auto PlatformKeyEvent::SetTimestamp(MonotonicTime const timestamp) -> void
{
    _timestamp = timestamp;
}

auto PlatformKeyEvent::Down::GetKey() const -> String
{
    return _key;
}

auto PlatformKeyEvent::Down::SetKey(String const& key) -> void
{
    _key = key;
}

auto PlatformKeyEvent::Down::GetUnmodifiedKey() const -> String
{
    return _unmodifiedKey;
}

auto PlatformKeyEvent::Down::SetUnmodifiedKey(String const& unmodifiedKey) -> void
{
    _unmodifiedKey = unmodifiedKey;
}

auto PlatformKeyEvent::Down::GetText() const -> String
{
    return _text;
}

auto PlatformKeyEvent::Down::SetText(String const& text) -> void
{
    _text = text;
}

auto PlatformKeyEvent::Down::GetModifiers() const -> ModifierKeyFlags
{
    return _modifiers;
}

auto PlatformKeyEvent::Down::SetModifiers(ModifierKeyFlags const modifiers) -> void
{
    _modifiers = modifiers;
}

auto PlatformKeyEvent::Down::IsRepeat() const -> Bool
{
    return _repeat;
}

auto PlatformKeyEvent::Down::SetRepeat(Bool const repeat) -> void
{
    _repeat = repeat;
}
}
