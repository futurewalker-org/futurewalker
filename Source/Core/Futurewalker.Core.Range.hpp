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
class Range<Integer<T, Tag>>
{
public:
    using ValueType = Integer<T, Tag>;
    using TagType = Tag;

    inline constexpr Range() = default;
    inline constexpr Range(Range const&) = default;
    inline constexpr auto operator=(Range const&) -> Range& = default;

    template <class B, class E>
    inline constexpr Range(B const& b, E const& e)
      : _b {b}
      , _e {e}
    {
    }

    inline constexpr auto GetBegin() const noexcept -> ValueType const&
    {
        return _b;
    }

    inline constexpr auto SetBegin(ValueType const& b) noexcept
    {
        _b = b;
    }

    inline constexpr auto GetEnd() const noexcept -> ValueType const&
    {
        return _e;
    }

    inline constexpr auto SetEnd(ValueType const& e) noexcept
    {
        _e = e;
    }

    inline constexpr auto GetLength() const noexcept -> ValueType
    {
        return _e - _b;
    }

    friend inline constexpr bool operator==(Range const& l, Range const& r) noexcept
    {
        return (l._b == r._b) && (l._e == r._e);
    }

    friend inline constexpr bool operator!=(Range const& l, Range const& r) noexcept
    {
        return (l._b != r._b) || (l._e != r._e);
    }

    template <template <class> class U>
    auto As() const -> U<ValueType>
    {
        return U<ValueType>(_b, _e);
    }

private:
    ValueType _b = static_cast<T>(0);
    ValueType _e = static_cast<T>(0);
};
}
}
