// SPDX-License-Identifier: MIT
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
class TimePoint : boost::additive2<TimePoint<Clock>, TimeInterval>
{
public:
    constexpr TimePoint() = default;

    template <class U>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, Float64>) TimePoint(const U& value) noexcept;

    auto operator<=>(const TimePoint&) const = default;

    friend inline constexpr auto operator==(const TimePoint& l, const TimePoint& r) noexcept -> bool
    {
        return l._seconds == r._seconds;
    }

    constexpr auto GetIntervalSinceEpoch() const noexcept -> TimeInterval;

public:
    inline constexpr auto operator+=(const TimeInterval& x) noexcept -> TimePoint&;
    inline constexpr auto operator-=(const TimeInterval& x) noexcept -> TimePoint&;

private:
    Float64 _seconds = 0;
};

///
/// @brief Construct from float value.
///
/// @param[in] value An float value.
///
template <class Clock>
template <class U>
inline constexpr TimePoint<Clock>::TimePoint(const U& value) noexcept
  : _seconds {static_cast<Float64>(value)}
{
}

///
/// @brief Get time interval since epoch. 
///
template <class Clock>
inline constexpr auto TimePoint<Clock>::GetIntervalSinceEpoch() const noexcept -> TimeInterval
{
    return static_cast<float64_t>(_seconds);
}

///
/// @brief Add interval to time point.
///
template <class Clock>
inline constexpr auto TimePoint<Clock>::operator+=(const TimeInterval& x) noexcept -> TimePoint&
{
    _seconds += static_cast<Float64>(x);
    return *this;
}

///
/// @brief Subtract time interval from time point.
///
template <class Clock>
inline constexpr auto TimePoint<Clock>::operator-=(const TimeInterval& x) noexcept -> TimePoint&
{
    _seconds -= static_cast<Float64>(x);
    return *this;
}
}
}
