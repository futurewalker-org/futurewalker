// SPDX-License-Identifier: MPL-2.0

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
auto AttributeEvent::ValueChanged::SetId(AttributeId const& id) -> void
{
    _id = id;
}
}
