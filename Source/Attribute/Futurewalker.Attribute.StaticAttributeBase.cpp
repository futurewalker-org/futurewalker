// SPDX-License-Identifier: MIT

#include "Futurewalker.Attribute.StaticAttributeBase.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
/// @param value
///
StaticAttributeBase::StaticAttributeBase(AttributeValue const& value)
  : _default {value}
{
}

///
/// @brief
///
/// @param reference
///
StaticAttributeBase::StaticAttributeBase(StaticAttributeBaseRef reference)
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
    if (auto const ptr = std::get_if<AttributeValue>(&_default))
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
auto StaticAttributeBase::GetDefaultReference() const -> Optional<StaticAttributeBaseRef>
{
    if (auto const ptr = std::get_if<StaticAttributeBaseRef>(&_default))
    {
        return *ptr;
    }
    return {};
}
}
