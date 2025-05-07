// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformScreenType.hpp"
#include "Futurewalker.Application.ScreenType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Optional.hpp" 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Screen.
///
class PlatformScreen : NonCopyable
{
public:
    virtual ~PlatformScreen() = 0;
    virtual auto GetInfo() const -> Optional<ScreenInfo> = 0;
};
}
}
