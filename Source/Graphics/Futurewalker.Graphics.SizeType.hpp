// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class>
class Size2;

///
/// @brief A two-dimensional size type.
///
/// @tparam T Value type.
///
template <class T>
using Size = Size2<T>;
}
}
