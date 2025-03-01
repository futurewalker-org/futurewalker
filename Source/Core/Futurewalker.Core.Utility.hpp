// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.NonConstructible.hpp"

#include <utility>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Utility functions.
///
class Utility : NonConstructible
{
public:
    ///
    /// @brief Retrun ordered pair of (min, max) from two values.
    ///
    /// @tparam T Type of values
    ///
    /// @param a Comparing value
    /// @param b Comparing value
    ///
    /// @return (min, max) pair
    ///
    template <class T>
    static inline constexpr auto MinMax(T a, T b) -> std::pair<T, T>
    {
        return (a > b) ? std::make_pair(std::move(b), std::move(a)) : std::make_pair(std::move(a), std::move(b));
    }

    ///
    /// @brief Return larger value from two values.
    ///
    /// @tparam T Type of values
    ///
    /// @param a Comparing value
    /// @param b Comparing value
    ///
    /// @return Larger value
    ///
    template <class T>
    static inline constexpr auto Max(T a, T b) -> T
    {
        return (a > b) ? std::move(a) : std::move(b);
    }

    ///
    /// @brief Return smaller value from two values.
    ///
    /// @tparam T Type of values
    ///
    /// @param a Comparing value
    /// @param b Comparing value
    ///
    /// @return Smaller value
    ///
    template <class T>
    static inline constexpr auto Min(T a, T b) -> T
    {
        return (a > b) ? std::move(b) : std::move(a);
    }

    ///
    /// @brief Clamp value between min and max.
    ///
    /// @tparam T Type of values
    ///
    /// @param val Value to clamp
    /// @param min Minimum value
    /// @param max Maximum value
    ///
    /// @return Clamped value
    ///
    template <class T>
    static inline constexpr auto Clamp(T val, T min, T max) -> T
    {
        return Max(std::move(min), Min(std::move(val), std::move(max)));
    }
};
}
}
