// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.StaticAttribute.hpp"
#include "Futurewalker.Attribute.AttributeNode.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Parameter wrapper for attributes.
///
template <class T>
class AttributeParam
{
public:
    constexpr AttributeParam()
      : _variant {std::monostate {}}
    {
    }

    ///
    /// @brief Construct from StaticAttributeRef.
    ///
    /// @param[in] reference Attribute reference.
    ///
    consteval AttributeParam(StaticAttribute<T> const& reference)
      : _variant {StaticAttributeRef(reference)}
    {
    }

    ///
    /// @brief Construct from value.
    ///
    /// @param[in] value Value of the attribute
    ///
    template <class U>
    AttributeParam(U const& value)
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

    ///
    /// @brief Apply attribute or value.
    ///
    template <class Owner>
    auto Apply(Owner& owner) const -> void
    {
        if (auto const value = GetValue())
        {
            AttributeNode::SetValue(owner, *value);
        }
        else if (auto const reference = GetReference())
        {
            AttributeNode::SetReference(owner, *reference);
        }
    }

private:
    std::variant<StaticAttributeRef<T>, T, std::monostate> _variant;
};
}
}
