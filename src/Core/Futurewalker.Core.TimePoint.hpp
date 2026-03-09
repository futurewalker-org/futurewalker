// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.TimePointType.hpp"

#include <boost/operators.hpp>

#include <chrono>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Time point class.
///
/// @tparam Clock Clock type.
///
template <class Clock>
class TimePoint
{
public:
    constexpr TimePoint() = default;

    ///
    /// @brief Construct from float value.
    ///
    /// @param[in] value An float value.
    ///
    template <class U>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, Float64>) TimePoint(U const& value) noexcept
      : _seconds {static_cast<Float64>(value)}
    {
    }

    auto operator<=>(TimePoint const&) const = default;

    friend inline constexpr auto operator==(TimePoint const& l, TimePoint const& r) noexcept -> bool
    {
        return l._seconds == r._seconds;
    }

    ///
    /// @brief Get time interval since epoch.
    ///
    constexpr auto GetIntervalSinceEpoch() const noexcept -> TimeInterval
    {
        return static_cast<float64_t>(_seconds);
    }

public:
    ///
    /// @brief Add interval to time point.
    ///
    inline constexpr auto operator+=(TimeInterval const& x) noexcept -> TimePoint&
    {
        _seconds += static_cast<Float64>(x);
        return *this;
    }

    ///
    /// @brief Subtract time interval from time point.
    ///
    inline constexpr auto operator-=(TimeInterval const& x) noexcept -> TimePoint&
    {
        _seconds -= static_cast<Float64>(x);
        return *this;
    }

    ///
    /// @brief Add interval to time point.
    ///
    friend inline constexpr auto operator+(TimePoint const& l, TimeInterval const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    ///
    /// @brief Subtract time interval from time point.
    ///
    friend inline constexpr auto operator-(TimePoint const& l, TimeInterval const& r) noexcept
    {
        auto tmp = l;
        tmp -= r;
        return tmp;
    }

    ///
    /// @brief Calculate interval between two time points.
    ///
    friend inline constexpr auto operator-(TimePoint const& l, TimePoint const& r) noexcept
    {
        return TimeInterval(static_cast<float64_t>(l._seconds - r._seconds));
    }

private:
    Float64 _seconds = 0;
};
}
}
