// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.StaticAttributeType.hpp"
#include "Futurewalker.Attribute.StaticAttributeBase.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.StaticReference.hpp"

#include <variant>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Attribute description.
///
/// @tparam T Value type of attribute.
///
template <class T>
class StaticAttribute : public StaticAttributeBase
{
public:
    ///
    /// @brief Create new attribute with default value.
    ///
    /// @param value Initial value of attribute. 
    ///
    /// @return Reference to unique attribute object.
    ///
    template <auto = [] {}>
    static auto MakeWithDefaultValue(T const& value) -> StaticAttributeRef<T>
    {
        static auto attribute = StaticAttribute<T>(AttributeValue(value));
        return attribute;
    }

    ///
    /// @brief Create new attribute with default reference.
    ///
    /// @param reference Initial reference value of attribute.  
    ///
    /// @return Reference to unique attribute object.
    ///
    template <auto = [] {}>
    static auto MakeWithDefaultReference(StaticAttributeRef<T> reference) -> StaticAttributeRef<T>
    {
        static auto attribute = StaticAttribute<T>(reference);
        return attribute;
    }

private:
    StaticAttribute() = delete;

    StaticAttribute(AttributeValue const& value)
      : StaticAttributeBase {value}
    {
    }

    StaticAttribute(StaticAttributeRef<T> reference)
      : StaticAttributeBase {reference}
    {
    }
};
}
}
