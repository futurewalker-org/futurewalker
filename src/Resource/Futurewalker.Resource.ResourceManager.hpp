// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Resource.ResourceManagerType.hpp"

#include "Futurewalker.Base.PathType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ResourceManager : NonCopyable
{
public:
    ResourceManager();

private:
    std::vector<Path> _resourceFolders;
};

template <>
struct Locator::Resolver<ResourceManager>
{
    using Interface = ResourceManager;
    static auto Resovlve() -> Shared<ResourceManager>;
};
}
}
