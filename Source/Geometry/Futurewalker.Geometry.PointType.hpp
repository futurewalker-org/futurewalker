// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class>
class Point2;

///
/// @brief A two-dimensional point type.
///
/// @tparam T Value type.
///
template <class T>
using Point = Point2<T>;
}
}
