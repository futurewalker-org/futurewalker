// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.RangeType.hpp"
#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Tagged range class.
///
/// @tparam T Value type
/// @tparam Tag Tag type
///
template <Concepts::Integral T, class Tag>
struct Range<Integer<T, Tag>>
{
    ///
    /// @brief Value type.
    ///
    using ValueType = Integer<T, Tag>;

    ///
    /// @brief Tag type.
    ///
    using TagType = Tag;

    ///
    /// @brief Normalize range.
    ///
    /// @param[in] range Range to normalize
    ///
    /// @return Normalized range (begin <= end).
    ///
    static constexpr auto Normalize(Range const& range) -> Range
    {
        if (range.begin > range.end)
        {
            return Range(range.end, range.begin);
        }
        return range;
    }

    ///
    /// @brief Reverse range.
    ///
    /// @param[in] range Range to reverse
    ///
    /// @return Reversed range (end, begin).
    ///
    static constexpr auto Reverse(Range const& range) -> Range
    {
        return Range(range.end, range.begin);
    }

    ///
    /// @brief Clamp value to range.
    ///
    /// @param[in] range Range.
    /// @param[in] value Value to clamp.
    ///
    /// @return value clamped to range.
    ///
    /// @note This function only works with forward ranges (begin<=end). You may want to normalize the range first to work on backward ranges.
    ///
    static constexpr auto Clamp(ValueType const& value, Range const& range) -> ValueType
    {
        return Utility::Clamp(value, range.begin, range.end);
    }

    ///
    /// @brief Clamp value to normalized range.
    ///
    /// @param[in] range Range.
    /// @param[in] value Value to clamp.
    ///
    /// @return value clamped to normalized range.
    ///
    static constexpr auto NormalizedClamp(ValueType const& value, Range const& range) -> ValueType
    {
        auto const normalized = Normalize(range);
        return Clamp(value, normalized);
    }

    ///
    /// @brief Get length of range.
    ///
    /// @return end - begin
    ///
    inline constexpr auto GetLength() const noexcept -> ValueType
    {
        return end - begin;
    }

    ///
    /// @brief Check if the range is empty.
    ///
    /// @return GetLength() == 0
    ///
    inline constexpr auto IsEmpty() const noexcept -> Bool
    {
        return GetLength() == static_cast<T>(0);
    }

    ///
    /// @brief Check if the range is normalized.
    ///
    /// @return begin <= end
    ///
    inline constexpr auto IsNormalized() const noexcept -> Bool
    {
        return begin <= end;
    }

    friend inline constexpr bool operator==(Range const& l, Range const& r) noexcept
    {
        return (l.begin == r.begin) && (l.end == r.end);
    }

    friend inline constexpr bool operator!=(Range const& l, Range const& r) noexcept
    {
        return (l.begin != r.begin) || (l.end != r.end);
    }

    template <template <class> class U>
    auto As() const -> U<ValueType>
    {
        return U<ValueType>(begin, end);
    }

    ValueType begin = static_cast<T>(0);
    ValueType end = static_cast<T>(0);
};
}
}
