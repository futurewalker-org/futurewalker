// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.Prelude.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Customization point of awaitable types.
///
template <class T>
struct AwaitableTraits
{
    ///
    /// @brief Awaiting coroutine will resume on the same thread.
    ///
    static constexpr bool ResumesOnSameThread = false;
};
}
}
