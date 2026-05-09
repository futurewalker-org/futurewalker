// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Resource.ResourceType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class Resource : NonCopyable
{
public:
    // static auto GetResource(String const& uuid) -> Shared<Resource>;

private:
    Resource();
};
}
}
