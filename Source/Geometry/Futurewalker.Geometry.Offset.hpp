// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Geometry.OffsetType.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Offset class
///
/// @tparam T Floating point type
/// @tparam Tag Tag type
///
template <Concepts::FloatingPoint T, class Tag>
class Offset2<Float<T, Tag>>
{
public:
    ///
    /// @brief Value type.
    ///
    using ValueType = Float<T, Tag>;

    ///
    /// @brief Get minimum components.
    ///
    /// @param lhs Offset to compare
    /// @param rhs Offset to compare
    ///
    /// @return Offset of minimum components
    ///
    static inline constexpr auto Min(Offset2 const& lhs, Offset2 const& rhs) -> Offset2
    {
        return Offset2(ValueType::Min(lhs._dx, rhs._dx), ValueType::Min(lhs._dy, rhs._dy));
    }

    ///
    /// @brief Get maximum components.
    ///
    /// @param lhs Offset to compare
    /// @param rhs Offset to compare
    ///
    /// @return Offset of maximum components
    ///
    static inline constexpr auto Max(Offset2 const& lhs, Offset2 const& rhs) -> Offset2
    {
        return Offset2(ValueType::Max(lhs._dx, rhs._dx), ValueType::Max(lhs._dy, rhs._dy));
    }

    ///
    /// @brief Round components.
    ///
    /// @param offset Offset to round
    ///
    /// @return Rounded offset
    ///
    static inline constexpr auto Round(Offset2 const& offset) -> Offset2
    {
        return Offset2(ValueType::Round(offset._dx), ValueType::Round(offset._dy));
    }

    ///
    /// @brief Floor components.
    ///
    /// @param offset Offset to floor
    ///
    /// @return Floored offset
    ///
    static inline constexpr auto Floor(Offset2 const& offset) -> Offset2
    {
        return Offset2(ValueType::Floor(offset._dx), ValueType::Floor(offset._dy));
    }

    ///
    /// @brief Ceil components.
    ///
    /// @param offset Offset to ceil
    ///
    /// @return Ceiled offset
    ///
    static inline constexpr auto Ceil(Offset2 const& offset) -> Offset2
    {
        return Offset2(ValueType::Ceil(offset._dx), ValueType::Ceil(offset._dy));
    }

public:
    ///
    /// @brief Create zero offset.
    ///
    inline constexpr Offset2() = default;

    ///
    /// @brief Copy constructor.
    ///
    inline constexpr Offset2(Offset2 const&) = default;

    ///
    /// @brief Construct from (dx, dy).
    ///
    /// @param dx dx
    /// @param dy dy
    ///
    inline constexpr Offset2(ValueType const& dx, ValueType const& dy) noexcept
      : _dx {dx}
      , _dy {dy}
    {
    }

    ///
    /// @brief Assignment operator.
    ///
    inline constexpr auto operator=(Offset2 const&) -> Offset2& = default;

    ///
    /// @brief Get dx.
    ///
    inline constexpr auto GetDeltaX() const noexcept -> ValueType const&
    {
        return _dx;
    }

    ///
    /// @brief Set dx.
    ///
    inline constexpr auto SetDeltaX(ValueType const& dx) noexcept -> void
    {
        _dx = dx;
    }

    ///
    /// @brief Get dy.
    ///
    inline constexpr auto GetDeltaY() const noexcept -> ValueType const&
    {
        return _dy;
    }

    ///
    /// @brief Set dy.
    ///
    inline constexpr auto SetDeltaY(ValueType const& dy) noexcept -> void
    {
        _dy = dy;
    }

    ///
    /// @brief Returns true if the offset is finite.
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(_dx) && ValueType::IsFinite(_dy);
    }

    ///
    /// @brief operator==
    ///
    friend inline constexpr bool operator==(Offset2 const& lhs, Offset2 const& rhs) noexcept
    {
        return (lhs._dx == rhs._dx) && (lhs._dy == rhs._dy);
    }

    ///
    /// @brief operator!=
    ///
    friend inline constexpr bool operator!=(Offset2 const& lhs, Offset2 const& rhs) noexcept
    {
        return (lhs._dx != rhs._dx) || (lhs._dy != rhs._dy);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+=(Offset2 const& x) noexcept -> Offset2&
    {
        _dx += x._dx;
        _dy += x._dy;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-=(Offset2 const& x) noexcept -> Offset2&
    {
        _dx -= x._dx;
        _dy -= x._dy;
        return *this;
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator-() const noexcept -> Offset2
    {
        return Offset2(-_dx, -_dy);
    }

    ///
    /// @brief
    ///
    inline constexpr auto operator+() const noexcept -> Offset2
    {
        return Offset2(+_dx, +_dy);
    }

    friend inline constexpr auto operator+(Offset2 const& l, Offset2 const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Offset2 const& l, Offset2 const& r) noexcept
    {
        auto tmp = l;
        tmp -= r;
        return tmp;
    }

    ///
    /// @brief Convert Point to other geometry types.
    ///
    template <template <class> class U>
    auto As() const -> U<ValueType>
    {
        return U<ValueType>(_dx, _dy);
    }

private:
    ValueType _dx = static_cast<T>(0);
    ValueType _dy = static_cast<T>(0);
};
}
}
