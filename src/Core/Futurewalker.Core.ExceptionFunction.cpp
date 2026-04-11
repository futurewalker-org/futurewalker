// SPDX-License-Identifier: MPL-2.0
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
}
