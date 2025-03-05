// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.Prelude.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.StaticReference.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class T>
class StaticAttribute;

template <class T>
using StaticAttributeRef = StaticReference<const StaticAttribute<T>>;
}
}
