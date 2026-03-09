// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

#include <unordered_map>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Wrapper for hash map.
///
/// TODO: Choose best one from std, boost, abseil, etc.
///
template <class K, class V, class H = std::hash<K>, class P = std::equal_to<K>, class A = std::allocator<std::pair<K const, V>>>
using HashMap = std::unordered_map<K, V, H, P, A>;
}
}
