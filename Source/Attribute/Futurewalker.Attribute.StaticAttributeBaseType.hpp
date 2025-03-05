// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Core.StaticReference.hpp"

#include <variant>

namespace FW_DETAIL_NS
{
class StaticAttributeBase;
using StaticAttributeBaseRef = StaticReference<const StaticAttributeBase>;
using StaticAttributeValue = std::variant<AttributeValue, StaticAttributeBaseRef>;
}
