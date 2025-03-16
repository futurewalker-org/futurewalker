// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class>
class RoundRect2;

///
/// @brief A two-dimensional rounded rectangle type.
///
/// @tparam T Value type.
///
template <class T>
using RoundRect = RoundRect2<T>;
}
}
