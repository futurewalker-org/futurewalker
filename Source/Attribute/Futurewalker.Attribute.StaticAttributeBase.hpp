// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.StaticAttributeBaseType.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.StaticPointer.hpp"
#include "Futurewalker.Core.Optional.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Base class of attribute description.
///
class StaticAttributeBase : NonCopyable
{
public:
    StaticAttributeBase() = delete;
    StaticAttributeBase(const AttributeValue& value);
    StaticAttributeBase(StaticReference<const StaticAttributeBase> reference);

    auto GetId() const noexcept -> AttributeId;
    auto GetDefaultValue() const -> Optional<AttributeValue>;
    auto GetDefaultReference() const -> Optional<StaticReference<const StaticAttributeBase>>;

private:
    UniqueAttributeId _uniqueId;
    StaticAttributeValue _default;
};
}
