// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.Prelude.hpp"

#include "Futurewalker.Attribute.StaticAttributeType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Argument wrapper for attributes.
///
template <class T>
class AttributeArg
{
public:
    ///
    /// @brief Construct from StaticAttributeRef.
    ///
    AttributeArg(StaticAttributeRef<T> const& reference)
      : _reference {reference}
    {
    }

    ///
    /// @brief Construct from 
    ///
    template <class U, auto = [] {}>
    AttributeArg(U const& value)
      : _reference {StaticAttribute<T>::MakeWithDefaultValue(value)}
    {
    }

    auto GetDescription() const -> StaticAttributeRef<T>
    {
        return _reference;
    }

private:
    StaticAttributeRef<T> _reference;
};
}
}
