// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.PropertyStoreType.hpp"
#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.HashMap.hpp"

#include <any>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Key-value pair map for dynamic properties.
///
class PropertyStore final
{
public:
    static auto Make() -> Unique<PropertyStore>;

    PropertyStore() = default;
    PropertyStore(PropertyStore const&) = default;
    PropertyStore(PropertyStore&&) noexcept = default;

    auto operator=(PropertyStore const&) -> PropertyStore& = default;
    auto operator=(PropertyStore&&) noexcept -> PropertyStore& = default;

    auto GetPropertyStore() -> PropertyStore&;
    auto GetPropertyStore() const -> PropertyStore const&;

    template <class Owner, class T>
    static auto SetValue(Owner& owner, Id const key, T const& value) -> void;

    template <class T, class Owner>
    static auto GetValue(Owner const& owner, Id const key) -> Optional<T>;

private:
    HashMap<Id, std::any> _map;
};

///
/// @brief Set property value.
///
template <class Owner, class T>
auto PropertyStore::SetValue(Owner& owner, Id const key, T const& value) -> void
{
    auto& map = owner.GetPropertyStore()._map;
    map.insert_or_assign(key, value);
}

///
/// @brief Get property value.
///
template <class T, class Owner>
auto PropertyStore::GetValue(Owner const& owner, Id const key) -> Optional<T>
{
    auto const& map = owner.GetPropertyStore()._map;
    if (auto const it = map.find(key); it != map.end())
    {
        if (auto const ptr = std::any_cast<T const>(&it->second))
        {
            return *ptr;
        }
    }
    return {};
}
}
}
