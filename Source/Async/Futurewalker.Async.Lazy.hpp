// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.LazyTask.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Single-threaded task type for unstructured concurrency. 
///
/// @tparam T Result type
///
template <class T>
using Lazy = LazyTask<T, true>;
}
}
