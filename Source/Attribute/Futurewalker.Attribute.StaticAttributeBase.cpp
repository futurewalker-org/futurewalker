// SPDX-License-Identifier: MIT

#include "Futurewalker.Attribute.StaticAttributeBase.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
/// @param value
///
StaticAttributeBase::StaticAttributeBase(const AttributeValue& value)
  : _default {value}
{
}

///
/// @brief
///
/// @param reference
///
StaticAttributeBase::StaticAttributeBase(StaticReference<const StaticAttributeBase> reference)
  : _default {reference}
{
}

///
/// @brief
///
/// @return
///
auto StaticAttributeBase::GetId() const noexcept -> AttributeId
{
    return static_cast<AttributeId>(_uniqueId);
}

///
/// @brief
///
/// @return
///
auto StaticAttributeBase::GetDefaultValue() const -> Optional<AttributeValue>
{
    if (const auto ptr = std::get_if<AttributeValue>(&_default))
    {
        return *ptr;
    }
    return {};
}

///
/// @brief
///
/// @return
///
auto StaticAttributeBase::GetDefaultReference() const -> Optional<StaticReference<const StaticAttributeBase>>
{
    if (const auto ptr = std::get_if<StaticReference<const StaticAttributeBase>>(&_default))
    {
        return *ptr;
    }
    return {};
}
}
