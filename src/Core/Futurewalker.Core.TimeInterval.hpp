// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.TimeIntervalType.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief 
///
class TimeInterval
{
public:
    static inline constexpr auto MakeFromNanoseconds(SInt64 const& nanoseconds) noexcept -> TimeInterval
    {
        return TimeInterval(nanoseconds);
    }

    static inline constexpr auto MakeFromMicroseconds(SInt64 const& microseconds) noexcept -> TimeInterval
    {
        return TimeInterval(microseconds * 1'000);
    }

    static inline constexpr auto MakeFromMilliseconds(SInt64 const& milliseconds) noexcept -> TimeInterval
    {
        return TimeInterval(milliseconds * 1'000'000);
    }

    static inline constexpr auto MakeFromSeconds(SInt64 const& seconds) noexcept -> TimeInterval
    {
        return TimeInterval(seconds * 1'000'000'000);
    }

public:
    TimeInterval() = default;

    inline constexpr auto GetIntSeconds() const noexcept -> SInt64
    {
        return _nanoseconds / 1'000'000'000;
    }

    inline constexpr auto GetIntMilliseconds() const noexcept -> SInt64
    {
        return _nanoseconds / 1'000'000;
    }

    inline constexpr auto GetIntMicroseconds() const noexcept -> SInt64
    {
        return _nanoseconds / 1'000;
    }

    inline constexpr auto GetIntNanoseconds() const noexcept -> SInt64
    {
        return _nanoseconds;
    }

    inline constexpr auto GetSeconds() const noexcept -> Float64
    {
        return static_cast<Float64>(_nanoseconds) / 1'000'000'000;
    }

    inline constexpr auto GetMilliseconds() const noexcept -> Float64
    {
        return static_cast<Float64>(_nanoseconds) / 1'000'000;
    }

    inline constexpr auto GetMicroseconds() const noexcept -> Float64
    {
        return static_cast<Float64>(_nanoseconds) / 1'000;
    }

    inline constexpr auto GetNanoseconds() const noexcept -> Float64
    {
        return static_cast<Float64>(_nanoseconds);
    }

public:
    inline constexpr TimeInterval& operator+=(TimeInterval const& x) noexcept
    {
        _nanoseconds += x._nanoseconds;
        return *this;
    }

    inline constexpr TimeInterval& operator-=(TimeInterval const& x) noexcept
    {
        _nanoseconds -= x._nanoseconds;
        return *this;
    }

    inline constexpr TimeInterval& operator*=(TimeInterval const& x) noexcept
    {
        _nanoseconds *= x._nanoseconds;
        return *this;
    }

    inline constexpr TimeInterval& operator/=(TimeInterval const& x) noexcept
    {
        _nanoseconds /= x._nanoseconds;
        return *this;
    }

    inline constexpr TimeInterval& operator%=(TimeInterval const& x) noexcept
    {
        _nanoseconds %= x._nanoseconds;
        return *this;
    }

    inline constexpr auto operator-() const noexcept -> TimeInterval
    {
        return TimeInterval(-_nanoseconds);
    }

    inline constexpr auto operator+() const noexcept -> TimeInterval
    {
        return TimeInterval(+_nanoseconds);
    }

    friend inline constexpr auto operator+(TimeInterval const& l, TimeInterval const& r) noexcept
    {
        auto tmp = l;
        tmp+= r;
        return tmp;
    }

    friend inline constexpr auto operator-(TimeInterval const& l, TimeInterval const& r) noexcept
    {
        auto tmp = l;
        tmp-= r;
        return tmp;
    }

    friend inline constexpr auto operator*(TimeInterval const& l, TimeInterval const& r) noexcept
    {
        auto tmp = l;
        tmp*= r;
        return tmp;
    }

    friend inline constexpr auto operator/(TimeInterval const& l, TimeInterval const& r) noexcept
    {
        auto tmp = l;
        tmp/= r;
        return tmp;
    }

    friend inline constexpr auto operator%(TimeInterval const& l, TimeInterval const& r) noexcept
    {
        auto tmp = l;
        tmp%= r;
        return tmp;
    }

    friend constexpr auto operator<=>(TimeInterval const& l, TimeInterval const& r) noexcept -> std::strong_ordering = default;
    friend constexpr auto operator==(TimeInterval const& l, TimeInterval const& r) noexcept -> bool = default;

private:
    explicit inline constexpr TimeInterval(SInt64 const& nanoseconds) noexcept
      : _nanoseconds(nanoseconds)
    {
    }
    SInt64 _nanoseconds = 0;
};
}
}
