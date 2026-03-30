// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformScreenMacType.hpp"
#include "Futurewalker.Application.PlatformScreenContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Screen context.
///
class PlatformScreenContextMac : public PlatformScreenContext
{
public:
    auto GetScreens() -> PlatformScreenArray override;
    auto GetPrimaryScreen() -> Shared<PlatformScreen> override;
    auto GetScreenFromWindow(Shared<PlatformWindow> window) -> Shared<PlatformScreen> override;
    auto GetScreenFromRect(Rect<Vp> const& rect) -> Shared<PlatformScreen> override;
    auto Refresh() -> void override;

private:
    auto FindScreenById(CGDirectDisplayID const displayId) -> Shared<PlatformScreenMac>;

private:
    std::vector<Shared<PlatformScreenMac>> _screens;
};

template <>
struct Locator::Resolver<PlatformScreenContextMac>
{
    using Interface = PlatformScreenContext;
    static auto Resolve() -> Shared<PlatformScreenContextMac>;
};
}
}
