// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class>
struct Size2;

///
/// @brief A two-dimensional size type.
///
/// @tparam T Value type.
///
template <class T>
using Size = Size2<T>;
}
}
