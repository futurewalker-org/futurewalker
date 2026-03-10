// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.TimePointType.hpp"
#include "Futurewalker.Core.TimeInterval.hpp"

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
    ///
    /// @brief Create time point from nanoseconds since epoch.
    ///
    /// @param nanoseconds Time interval since epoch in nanoseconds.
    ///
    static auto MakeFromNanoseconds(SInt64 const& nanoseconds) noexcept -> TimePoint
    {
        return TimePoint(nanoseconds);
    }

    ///
    /// @brief Create time point from microseconds since epoch.
    ///
    /// @param microseconds Time interval since epoch in microseconds.
    ///
    static auto MakeFromMicroseconds(SInt64 const& microseconds) noexcept -> TimePoint
    {
        return TimePoint(microseconds * 1'000);
    }

    ///
    /// @brief Create time point from milliseconds since epoch.
    ///
    /// @param milliseconds Time interval since epoch in milliseconds.
    ///
    static auto MakeFromMilliseconds(SInt64 const& milliseconds) noexcept -> TimePoint
    {
        return TimePoint(milliseconds * 1'000'000);
    }

    ///
    /// @brief Create time point from seconds since epoch.
    ///
    /// @param seconds Time interval since epoch in seconds.
    ///
    static auto MakeFromSeconds(SInt64 const& seconds) noexcept -> TimePoint
    {
        return TimePoint(seconds * 1'000'000'000);
    }

public:
    ///
    /// @brief Default constructor.
    ///
    constexpr TimePoint() = default;

    ///
    /// @brief Compare two time points.
    ///
    friend inline constexpr auto operator<=>(TimePoint const& l, TimePoint const& r) = default;

    ///
    /// @return
    ///
    friend inline constexpr auto operator==(TimePoint const& l, TimePoint const& r) noexcept -> bool = default;

    ///
    /// @brief Get time interval since epoch.
    ///
    constexpr auto GetIntervalSinceEpoch() const noexcept -> TimeInterval
    {
        return TimeInterval::MakeFromNanoseconds(_nanoseconds);
    }

public:
    ///
    /// @brief Add interval to time point.
    ///
    inline constexpr auto operator+=(TimeInterval const& x) noexcept -> TimePoint&
    {
        _nanoseconds += x.GetIntNanoseconds();
        return *this;
    }

    ///
    /// @brief Subtract time interval from time point.
    ///
    inline constexpr auto operator-=(TimeInterval const& x) noexcept -> TimePoint&
    {
        _nanoseconds -= x.GetIntNanoseconds();
        return *this;
    }

    ///
    /// @brief Add interval to time point.
    ///
    friend inline constexpr auto operator+(TimePoint const& l, TimeInterval const& r) noexcept -> TimePoint
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    ///
    /// @brief Subtract time interval from time point.
    ///
    friend inline constexpr auto operator-(TimePoint const& l, TimeInterval const& r) noexcept -> TimePoint
    {
        auto tmp = l;
        tmp -= r;
        return tmp;
    }

    ///
    /// @brief Calculate interval between two time points.
    ///
    friend inline constexpr auto operator-(TimePoint const& l, TimePoint const& r) noexcept -> TimeInterval
    {
        return TimeInterval::MakeFromNanoseconds(l._nanoseconds - r._nanoseconds);
    }

private:
    explicit constexpr TimePoint(SInt64 const& nanoseconds) noexcept
      : _nanoseconds(nanoseconds)
    {
    }

private:
    SInt64 _nanoseconds = 0;
};
}
}
