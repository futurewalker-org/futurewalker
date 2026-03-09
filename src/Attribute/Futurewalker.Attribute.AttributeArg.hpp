// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Argument wrapper for attributes.
///
/// Wraps variant of types which can be assigned to an attribute.
///
/// @note This class is intended to used as functions arguments. Do not use for other purposes.
///
template <class T>
class AttributeArg : NonCopyable
{
public:
    ///
    /// @brief Construct from StaticAttribute.
    ///
    /// @param[in] reference Attribute reference.
    ///
    consteval AttributeArg(StaticAttribute<T> const& reference) noexcept
      : _variant {StaticAttributeRef(reference)}
    {
    }

    ///
    /// @brief Construct from StaticAttributeRef.
    ///
    AttributeArg(StaticAttributeRef<T> const& reference) noexcept
      : _variant {reference}
    {
    }

    ///
    /// @brief Construct from value.
    ///
    /// @param[in] value Value of the attribute
    ///
    template <class U>
    AttributeArg(U const& value)
      : _variant {T(value)}
    {
    }

    ///
    /// @brief Get value.
    ///
    auto GetValue() const -> Pointer<T const>
    {
        return std::get_if<T>(&_variant);
    }

    ///
    /// @brief Get reference.
    ///
    auto GetReference() const -> Pointer<StaticAttributeRef<T> const>
    {
        return std::get_if<StaticAttributeRef<T>>(&_variant);
    }

private:
    std::variant<StaticAttributeRef<T>, T> _variant;
};
}
}
