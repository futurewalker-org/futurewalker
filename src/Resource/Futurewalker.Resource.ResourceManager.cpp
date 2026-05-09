// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Resource.ResourceManager.hpp"

#include "Futurewalker.Base.Path.hpp"

namespace FW_DETAIL_NS
{
ResourceManager::ResourceManager()
{
}

auto Locator::Resolver<ResourceManager>::Resovlve() -> Shared<ResourceManager>
{
    return Shared<ResourceManager>::Make();
}
}
