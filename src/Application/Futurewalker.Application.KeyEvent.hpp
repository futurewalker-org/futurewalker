// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.KeyEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.MonotonicTime.hpp"
#include "Futurewalker.Core.Flags.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Key event.
///
class KeyEvent : public EventParameter
{
public:
    auto GetKey() const -> String;
    auto SetKey(String const& key) -> void;

    auto GetUnmodifiedKey() const -> String;
    auto SetUnmodifiedKey(String const& unmodifiedKey) -> void;

    auto GetTimestamp() const -> MonotonicTime;
    auto SetTimestamp(MonotonicTime const timestamp) -> void;

    auto IsComposing() const -> Bool;
    auto SetComposing(Bool const composing) -> void;

public:
    class Down;
    class Up;

private:
    String _key;
    String _unmodifiedKey;
    MonotonicTime _timestamp;
    Bool _composing = false;
};

///
/// @brief Key down event.
///
class KeyEvent::Down final : public KeyEvent
{
public:
    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

    auto GetModifiers() const -> Flags<ModifierKeyFlag>;
    auto SetModifiers(Flags<ModifierKeyFlag> const modifiers) -> void;

    auto IsRepeat() const -> Bool;
    auto SetRepeat(Bool const repeat) -> void;

private:
    String _text;
    Flags<ModifierKeyFlag> _modifiers = ModifierKeyFlag::None;
    Bool _repeat = false;
};

///
/// @brief Key up event.
///
class KeyEvent::Up final : public KeyEvent
{
};
}
}
