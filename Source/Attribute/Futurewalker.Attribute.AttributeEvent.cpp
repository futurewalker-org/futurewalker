// SPDX-License-Identifier: MIT

#include "Futurewalker.Attribute.AttributeEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto AttributeEvent::ValueChanged::GetId() const -> AttributeId
{
    return _id;
}

///
/// @brief
///
/// @param id
///
auto AttributeEvent::ValueChanged::SetId(const AttributeId& id) -> void
{
    _id = id;
}

///
/// @brief
///
auto AttributeEvent::ValueChanged::GetValue() const -> Optional<AttributeValue>
{
    return _value;
}

///
/// @brief 
///
/// @param value 
///
auto AttributeEvent::ValueChanged::SetValue(const AttributeValue& value) -> void
{
    _value = value;
}
}
