// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class>
struct Rect2;

///
/// @brief A two-dimensional rectangle type.
///
/// @tparam T Value type.
///
template <class T>  
using Rect = Rect2<T>;

///
/// @brief Array of two-dimensional rectangle type.
///
template <class T>
using RectArray = std::vector<Rect<T>>;
}
}
