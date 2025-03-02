// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Concepts.hpp"

#include <boost/functional/hash.hpp>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Tagged Identifier type.
///
/// @tparam Value Value type of identifier
/// @tparam Tag Tag type of identifier
///
template <class Value, class Tag = void>
class Identifier
{
public:
    ///
    /// @brief Underlying type of Identifier.
    ///
    using TagType = Tag;

    ///
    /// @brief Tag type of Identifier.
    ///
    using ValueType = Value;

public:
    Identifier() = delete;
    Identifier(const Identifier&) = default;
    Identifier& operator=(const Identifier&) = default;

    ///
    /// @brief Construct Identifier from value.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U, ValueType>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, ValueType>) Identifier(const U& value) noexcept
      : _value {static_cast<ValueType>(value)}
    {
    }

    ///
    /// @brief Convert Identifier to underlying type.
    ///
    inline explicit constexpr operator ValueType() const noexcept
    {
        return _value;
    }

    ///
    /// @brief Convert to different identifier.
    ///
    template <class OtherValue, class OtherTag>
    requires Concepts::ImplicitlyConvertibleTo<ValueType, OtherValue>
    inline explicit constexpr operator Identifier<OtherValue, OtherTag>() const noexcept
    {
        return Identifier<OtherValue, OtherTag>(_value);
    }

    constexpr auto operator<=>(const Identifier&) const = default;

private:
    ValueType _value;
};

///
/// @brief Generic ID type.
///
using Id = Identifier<UInt64, void>;
}
}

template <class Value, class Tag>
struct std::hash<FW_NS::Identifier<Value, Tag>>
{
    constexpr auto operator()(const FW_NS::Identifier<Value, Tag>& v) const noexcept -> size_t
    {
        return std::hash<Value>()(static_cast<Value>(v));
    }
};

template <class Value, class Tag>
struct boost::hash<FW_NS::Identifier<Value, Tag>>
{
    constexpr auto operator()(const FW_NS::Identifier<Value, Tag>& v) const noexcept -> size_t
    {
        return boost::hash<Value>()(static_cast<Value>(v));
    }
};
