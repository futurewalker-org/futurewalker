// SPDX-License-Identifier: MIT
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
    [[noreturn]] static auto ThrowNullDereferenceException() -> void;
};
}
