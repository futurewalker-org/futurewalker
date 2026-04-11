// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.ExceptionType.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Exception-related utility functions.
///
class ExceptionFunction : NonConstructible
{
public:
    [[noreturn]] static auto ThrowDynamicCastException() -> void;
};
}
