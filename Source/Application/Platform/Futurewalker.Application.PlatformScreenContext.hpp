// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformScreenContextType.hpp"
#include "Futurewalker.Application.PlatformScreenType.hpp"
#include "Futurewalker.Application.PlatformWindowType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Screen context.
///
class PlatformScreenContext : NonCopyable
{
public:
    virtual ~PlatformScreenContext() = 0;
    virtual auto GetScreens() -> PlatformScreenArray = 0;
    virtual auto GetPrimaryScreen() -> Shared<PlatformScreen> = 0;
    virtual auto GetScreenFromWindow(Shared<PlatformWindow> window) -> Shared<PlatformScreen> = 0;
    virtual auto GetScreenFromRect(Rect<Vp> const& rect) -> Shared<PlatformScreen> = 0;
    virtual auto Refresh() -> void = 0;
};
}
}
