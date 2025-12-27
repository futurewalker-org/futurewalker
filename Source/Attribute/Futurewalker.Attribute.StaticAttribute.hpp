// SPDX-License-Identifier: MPL-2.0
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
    /// @brief Value type.
    ///
    using ValueType = T;

    ///
    /// @brief Create new attribute with default value.
    ///
    /// @param value Initial value of attribute. 
    ///
    /// @return Reference to unique attribute object.
    ///
    static auto MakeWithDefaultValue(
#if FW_ENABLE_DEBUG
      Pointer<char const> name,
#endif
      T const& value) -> StaticAttribute<T>
    {
        return StaticAttribute<T>(
#if FW_ENABLE_DEBUG
          name,
#endif
          [value](auto const&) -> AttributeValue { return AttributeValue(value); },
          {});
    }

    ///
    /// @brief Create new attribute with default reference.
    ///
    /// @param reference Initial reference value of attribute.
    ///
    /// @return Reference to unique attribute object.
    ///
    template <const auto* Reference>
    static auto MakeWithDefaultReference(
#if FW_ENABLE_DEBUG
      Pointer<char const> name
#endif
      ) -> StaticAttribute<T>
    {
        auto const reference = StaticAttributeBaseRef(*Reference);
        return StaticAttribute<T>(
#if FW_ENABLE_DEBUG
          name,
#endif
          [](auto const& references) -> AttributeValue { return references[0]; },
          {&reference, 1});
    }

private:
    StaticAttribute() = delete;

    StaticAttribute(
#if FW_ENABLE_DEBUG
      Pointer<char const> name,
#endif
      StaticAttributeComputeFunction const& computeFunction,
      std::span<StaticAttributeBaseRef const> const references)
      : StaticAttributeBase {
#if FW_ENABLE_DEBUG
            name,
#endif
            computeFunction,
            references}
    {
    }
};
}
}
