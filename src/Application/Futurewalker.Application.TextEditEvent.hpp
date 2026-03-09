// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TextEditEvent : public ViewEvent::Notify
{
public:
    class FocusChanged;
};

class TextEditEvent::FocusChanged final : public TextEditEvent
{
public:
    auto IsFocused() const -> Bool;
    auto SetFocused(Bool focused) -> void;

private:
    Bool _focused = false;
};
}
}
