// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.FocusEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Focus event.
///
class FocusEvent : public EventParameter
{
public:
    class FocusIn;
    class FocusOut;
    class FocusWillChange;
    class FocusDidChange;
};

///
/// @brief Gained focus.
///
class FocusEvent::FocusIn final : public FocusEvent
{
};

///
/// @brief Lost focus.
///
class FocusEvent::FocusOut final : public FocusEvent
{
};

///
/// @brief Focused node will change.
///
/// @note Currently only sent to RootFocusNode.
///
class FocusEvent::FocusWillChange final : public FocusEvent
{
};

///
/// @brief Focused node did change.
///
/// @note Currently only sent to RootFocusNode.
///
class FocusEvent::FocusDidChange final : public FocusEvent
{
};
}
}
