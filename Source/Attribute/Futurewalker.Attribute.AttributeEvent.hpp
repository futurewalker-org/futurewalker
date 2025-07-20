// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeId.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.Optional.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Attribute event.
///
class AttributeEvent : public EventParameter 
{
public:
    class ValueChanged;
};

///
/// @brief Value of attribute changed.
///
class AttributeEvent::ValueChanged final : public AttributeEvent
{
public:
    auto GetId() const -> AttributeId;
    auto SetId(AttributeId const& id) -> void;

private:
    AttributeId _id = 0U;
};
}
}
