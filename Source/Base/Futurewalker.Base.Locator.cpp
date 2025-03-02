// SPDX-License-Identifier: MIT
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Get shared instance.
///
auto Locator::GetSelf() -> Locator&
{
    static auto _locator = Locator();
    return _locator;
}

///
/// @brief Add instance.
///
/// @param[in] id Id.
/// @param[in] instance Instance.
///
void Locator::AddInstance(const Id& id, const Shared<void>& instance)
{
    if (instance)
    {
        _instanceMap.insert_or_assign(id, instance);
    }
}

///
/// @brief Find instance factory and create instance.
///
/// @param[in] id Type id.
///
auto Locator::CreateInstance(const Id& id) -> Shared<void>
{
    const auto it = _factoryMap.find(id);
    if (it != _factoryMap.end())
    {
        if (it->second)
        {
            return it->second();
        }
    }
    return nullptr;
}

///
/// @brief Find instance from instance map.
///
/// @param[in] id Type id.
///
auto Locator::FindInstance(const Id& id) const -> Shared<void>
{
    const auto it = _instanceMap.find(id);
    if (it != _instanceMap.end())
    {
        return it->second.Lock();
    }
    return nullptr;
}
}
