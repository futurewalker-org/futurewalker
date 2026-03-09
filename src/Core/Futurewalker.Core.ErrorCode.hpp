// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Error code enum.
///
/// Common error codes which can be useful for diagnostic purposes.
///
enum class ErrorCode
{
    Success = 0,         // Success.
    Failure,             // Generic failure.
    InvalidArgument,     // Arguments provided were invalid.
    InvalidOperation,    // Invalid operation.
    ResourceUnavailable, // Resource unavailable.
    OperationAborted,    // Operation aborted.
};
}
}
