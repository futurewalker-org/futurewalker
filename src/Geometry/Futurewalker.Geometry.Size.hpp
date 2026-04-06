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
struct Size2<Float<T, Tag>>
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
        return Size2(ValueType::Max(ValueType(0), size.width), ValueType::Max(ValueType(0), size.height));
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
        return Size2(ValueType::Min(lhs.width, rhs.width), ValueType::Min(lhs.height, rhs.height));
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
        return Size2(ValueType::Max(lhs.width, rhs.width), ValueType::Max(lhs.height, rhs.height));
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
        return Size2(ValueType::Round(size.width), ValueType::Round(size.height));
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
        return Size2(ValueType::Floor(size.width), ValueType::Floor(size.height));
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
        return Size2(ValueType::Ceil(size.width), ValueType::Ceil(size.height));
    }

public:
    ///
    /// @brief Returns true if the size is empty
    ///
    inline constexpr auto IsEmpty() const noexcept -> Bool
    {
        return width <= ValueType(0) || height <= ValueType(0);
    }

    ///
    /// @brief Returns true if the size is finite
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(width) && ValueType::IsFinite(height);
    }

    friend inline constexpr bool operator==(Size2 const&, Size2 const&) = default;

    friend inline constexpr bool operator!=(Size2 const&, Size2 const&) = default;

    ///
    /// @brief Width component.
    ///
    ValueType width = static_cast<T>(0);

    ///
    /// @brief Height component.
    ///
    ValueType height = static_cast<T>(0);
};
}
}
