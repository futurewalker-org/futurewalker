// SPDX-License-Identifier: MIT
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
    PropertyStore(const PropertyStore&) = default;
    PropertyStore& operator=(const PropertyStore&) = default;
    PropertyStore(PropertyStore&&) noexcept = default;
    PropertyStore& operator=(PropertyStore&&) noexcept = default;

    auto GetPropertyStore() -> PropertyStore&;
    auto GetPropertyStore() const -> const PropertyStore&;

    template <class Owner, class T>
    static auto SetValue(Owner& owner, const Id key, const T& value) -> void;

    template <class T, class Owner>
    static auto GetValue(const Owner& owner, const Id key) -> Optional<T>;

private:
    HashMap<Id, std::any> _map;
};

///
/// @brief Set property value.
///
template <class Owner, class T>
auto PropertyStore::SetValue(Owner& owner, const Id key, const T& value) -> void
{
    auto& map = owner.GetPropertyStore()._map;
    map.emplace(key, value);
}

///
/// @brief Get property value.
///
template <class T, class Owner>
auto PropertyStore::GetValue(const Owner& owner, const Id key) -> Optional<T>
{
    const auto& map = owner.GetPropertyStore()._map;
    if (const auto it = map.find(key); it != map.end())
    {
        if (const auto ptr = std::any_cast<const T>(&it->second))
        {
            return *ptr;
        }
    }
    return {};
}
}
}
