// SPDX-License-Identifier: MIT
#include "Futurewalker.Core.ExceptionFunction.hpp"
#include "Futurewalker.Core.Exception.hpp"

#include <cassert>

namespace FW_DETAIL_NS
{
[[noreturn]] auto ExceptionFunction::ThrowDynamicCastException() -> void
{
    assert(false);
    throw Exception(ErrorCode::Failure, "Dynamic cast failed");
}

[[noreturn]] auto ExceptionFunction::ThrowNullDereferenceException() -> void
{
    assert(false);
    throw Exception(ErrorCode::InvalidOperation, "Null pointer dereference.");
}
}
