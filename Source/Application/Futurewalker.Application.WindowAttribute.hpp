// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"
#include "Futurewalker.Application.WindowAreaManagerType.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Window attribute.
///
class WindowAttribute : NonConstructible
{
public:
    FW_STATIC_ATTRIBUTE(Shared<WindowAreaManager>, AreaManager);
};
}
}
