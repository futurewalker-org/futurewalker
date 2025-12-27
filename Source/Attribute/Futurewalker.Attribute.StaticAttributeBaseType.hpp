// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Core.StaticReference.hpp"
#include "Futurewalker.Core.Function.hpp"

#include <variant>
#include <span>

namespace FW_DETAIL_NS
{
class StaticAttributeBase;
using StaticAttributeBaseRef = StaticReference<StaticAttributeBase const>;
using StaticAttributeComputeFunction = Function<AttributeValue(std::span<AttributeValue const> args)>;
}
