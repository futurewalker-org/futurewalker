// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Core.PropertyStore.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make property store.
///
auto PropertyStore::Make() -> Unique<PropertyStore>
{
    return Unique<PropertyStore>::Make();
}

///
/// @brief Get property store.
///
auto PropertyStore::GetPropertyStore() -> PropertyStore&
{
    return *this;
}

///
/// @brief Get property store.
///
auto PropertyStore::GetPropertyStore() const -> PropertyStore const&
{
    return *this;
}
}