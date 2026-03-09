// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"
#include "Futurewalker.Application.ViewAreaManagerType.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
struct ViewAttribute : NonConstructible
{
    FW_STATIC_ATTRIBUTE(Shared<ViewAreaManager>, AreaManager);
    FW_STATIC_ATTRIBUTE(Float64, HorizontalScrollFactor);
    FW_STATIC_ATTRIBUTE(Float64, VerticalScrollFactor);
};
}
}
