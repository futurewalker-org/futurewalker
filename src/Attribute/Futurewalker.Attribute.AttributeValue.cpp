// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeValue.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief 
///
/// @param other 
///
AttributeValue::AttributeValue(AttributeValue const& other) noexcept
  : _holder {other._holder}
{
}

///
/// @brief
///
/// @param other
///
auto AttributeValue::operator=(AttributeValue const& other) noexcept -> AttributeValue&
{
    _holder = other._holder;
    return *this;
}

///
/// @brief operator==
///
auto AttributeValue::operator==(AttributeValue const& other) const noexcept -> bool
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
