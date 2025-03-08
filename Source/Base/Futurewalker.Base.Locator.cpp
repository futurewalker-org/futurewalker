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
void Locator::AddInstance(Id const& id, Shared<void> const& instance)
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
auto Locator::CreateInstance(Id const& id) -> Shared<void>
{
    auto const it = _factoryMap.find(id);
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
auto Locator::FindInstance(Id const& id) const -> Shared<void>
{
    auto const it = _instanceMap.find(id);
    if (it != _instanceMap.end())
    {
        return it->second.Lock();
    }
    return nullptr;
}
}
