// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

#include <functional>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Wrapper for function type.
///
/// TODO: Use `std::copyable_function` in C++26.
/// https://en.cppreference.com/w/cpp/utility/functional/copyable_function
///
template <class F>
using Function = std::function<F>;
}
}
