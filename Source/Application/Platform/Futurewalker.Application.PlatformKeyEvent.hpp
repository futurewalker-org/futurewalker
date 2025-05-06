// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformKeyEventType.hpp"
#include "Futurewalker.Application.KeyEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"
#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformKeyEvent : public EventParameter
{
public:
    auto GetTimestamp() const -> MonotonicTime;
    auto SetTimestamp(MonotonicTime const timestamp) -> void;

public:
    class Down;
    class Up;

private:
    MonotonicTime _timestamp;
};

class PlatformKeyEvent::Down final : public PlatformKeyEvent
{
public:
    auto GetKey() const -> String;
    auto SetKey(String const& key) -> void;

    auto GetUnmodifiedKey() const -> String;
    auto SetUnmodifiedKey(String const& unmodifiedKey) -> void;

    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

    auto GetModifiers() const -> ModifierKeyFlags;
    auto SetModifiers(ModifierKeyFlags const modifiers) -> void;

    auto IsRepeat() const -> Bool;
    auto SetRepeat(Bool const repeat) -> void;

private:
    String _key;
    String _unmodifiedKey;
    String _text;
    ModifierKeyFlags _modifiers = ModifierKeyFlags::None;
    Bool _repeat = false;
};

class PlatformKeyEvent::Up final : public PlatformKeyEvent
{
public:
};
}
}
