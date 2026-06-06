// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Resource.ResourceType.hpp"
#include "Futurewalker.Resource.ResourceBundleType.hpp"

#include "Futurewalker.Base.Locale.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Uuid.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Provides access for specific resource module.
///
class Resource : NonCopyable
{
public:
    Resource(Weak<ResourceBundle> const& bundle, SInt64 const& moduleIndex);

    auto GetString(ResourceId const id) -> String;

private:
    Weak<ResourceBundle> _bundle;
    SInt64 _moduleIndex = 0;
};
}
}
