// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Core.StaticReference.hpp"

#include <variant>

namespace FW_DETAIL_NS
{
class StaticAttributeBase;
using StaticAttributeBaseRef = StaticReference<StaticAttributeBase const>;
using StaticAttributeValue = std::variant<AttributeValue, StaticAttributeBaseRef>;
}
