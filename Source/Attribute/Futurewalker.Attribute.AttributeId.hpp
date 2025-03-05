// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.AttributeValueType.hpp"

#include "Futurewalker.Base.UniqueIdentifierType.hpp" 

#include "Futurewalker.Core.Identifier.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
using AttributeId = Identifier<UInt64, AttributeValue>;
using UniqueAttributeId = UniqueIdentifier<AttributeValue>;
}
}
