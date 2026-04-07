// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class>
struct Radius2;

///
/// @brief A two-dimensional radius type.
///
/// @tparam T Value type.
///
template <class T>
using Radius = Radius2<T>;
}
}
