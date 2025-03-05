// SPDX-License-Identifier: MIT

#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief 
///
/// @param other 
///
AttributeValue::AttributeValue(const AttributeValue& other) noexcept
  : _holder {other._holder}
{
}

///
/// @brief
///
/// @param other
///
auto AttributeValue::operator=(const AttributeValue& other) noexcept -> AttributeValue&
{
    _holder = other._holder;
    return *this;
}

///
/// @brief operator==
///
auto AttributeValue::operator==(const AttributeValue& other) const noexcept -> bool
{
    if (_holder == other._holder)
    {
        return true;
    }

    if (_holder && other._holder)
    {
        return _holder->EqualsTo(other._holder);
    }
    return false;
}
}
