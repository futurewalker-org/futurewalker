// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.AsyncTask.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Single threaded task type for structured concurrency.
///
/// @tparam T Result type
///
template <class T>
using Async = AsyncTask<T, true>;
}
}
