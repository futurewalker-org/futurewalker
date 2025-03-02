// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.PlatformDebugType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform debug utilities.
///
class PlatformDebug : NonCopyable
{
public:
    virtual ~PlatformDebug() = 0;

    ///
    /// @brief Prints debug message to debugger's console.
    ///
    /// @param message Debug message to print.
    ///
    /// @note This function does not check current build option.
    ///
    virtual auto Print(const StringView message) -> void = 0;

    ///
    /// @brief Break current program's execution.
    ///
    virtual auto Break() -> void = 0;
};
}
}
