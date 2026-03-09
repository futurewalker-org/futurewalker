// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Geometry.SizeType.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Size class.
///
/// @tparam T Floating point type
/// @tparam Tag Tag type
///
template <Concepts::FloatingPoint T, class Tag>
class Size2<Float<T, Tag>>
{
public:
    ///
    /// @brief Value type
    ///
    using ValueType = Float<T, Tag>;

    ///
    /// @brief Normalize size.
    ///
    /// @param size Size to normalize
    ///
    /// @return Normalized size
    ///
    static inline constexpr auto Normalize(Size2 const& size) -> Size2
    {
        return Size2(ValueType::Max(ValueType(0), size._w), ValueType::Max(ValueType(0), size._h));
    }

    ///
    /// @brief Get minimum components.
    ///
    /// @param lhs Size to compare
    /// @param rhs Size to compare
    ///
    /// @return Size of minimum components
    ///
    static inline constexpr auto Min(Size2 const& lhs, Size2 const& rhs) -> Size2
    {
        return Size2(ValueType::Min(lhs._w, rhs._w), ValueType::Min(lhs._h, rhs._h));
    }

    ///
    /// @brief Get maximum components.
    ///
    /// @param lhs Size to compare
    /// @param rhs Size to compare
    ///
    /// @return Size of maximum components
    ///
    static inline constexpr auto Max(Size2 const& lhs, Size2 const& rhs) -> Size2
    {
        return Size2(ValueType::Max(lhs._w, rhs._w), ValueType::Max(lhs._h, rhs._h));
    }

    ///
    /// @brief Round components.
    ///
    /// @param size Size to round
    ///
    /// @return Rounded size
    ///
    static inline constexpr auto Round(Size2 const& size) -> Size2
    {
        return Size2(ValueType::Round(size._w), ValueType::Round(size._h));
    }

    ///
    /// @brief Floor components.
    ///
    /// @param size Size to floor
    ///
    /// @return Floored size
    ///
    static inline constexpr auto Floor(Size2 const& size) -> Size2
    {
        return Size2(ValueType::Floor(size._w), ValueType::Floor(size._h));
    }

    ///
    /// @brief Ceil components.
    ///
    /// @param size Size to ceil
    ///
    /// @return Ceiled size
    ///
    static inline constexpr auto Ceil(Size2 const& size) -> Size2
    {
        return Size2(ValueType::Ceil(size._w), ValueType::Ceil(size._h));
    }

public:
    ///
    /// @brief Default constructor.
    ///
    /// Initializes both Width and Height values to zero.
    ///
    inline constexpr Size2() = default;

    ///
    /// @brief Copy constructor.
    ///
    inline constexpr Size2(Size2 const&) = default;

    ///
    /// @brief Construct from components
    ///
    /// @param width Width
    /// @param height Height
    ///
    inline constexpr Size2(ValueType const& width, ValueType const& height)
      : _w {width}
      , _h {height}
    {
    }

    ///
    /// @brief Construct square size
    ///
    /// @param extent Extent of width and height
    ///
    inline constexpr explicit Size2(ValueType const& extent)
      : _w {extent}
      , _h {extent}
    {
    }

    ///
    /// @brief Copy assignment operator.
    ///
    inline constexpr auto operator=(Size2 const&) -> Size2& = default;

    ///
    /// @brief Get width.
    ///
    inline constexpr auto GetWidth() const noexcept -> ValueType const&
    {
        return _w;
    }

    ///
    /// @brief Set width.
    ///
    inline constexpr void SetWidth(ValueType const& width) noexcept
    {
        _w = width;
    }

    ///
    /// @brief Get height.
    ///
    inline constexpr auto GetHeight() const noexcept -> ValueType const&
    {
        return _h;
    }

    ///
    /// @brief Set height.
    ///
    inline constexpr void SetHeight(ValueType const& height) noexcept
    {
        _h = height;
    }

    ///
    /// @brief Returns true if the size is empty
    ///
    inline constexpr auto IsEmpty() const noexcept -> Bool
    {
        return _w <= ValueType(0) || _h <= ValueType(0);
    }

    ///
    /// @brief Returns true if the size is finite
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(_w) && ValueType::IsFinite(_h);
    }

    friend inline constexpr bool operator==(Size2 const&, Size2 const&) = default;

    friend inline constexpr bool operator!=(Size2 const&, Size2 const&) = default;

private:
    ValueType _w = static_cast<T>(0);
    ValueType _h = static_cast<T>(0);
};
}
}
