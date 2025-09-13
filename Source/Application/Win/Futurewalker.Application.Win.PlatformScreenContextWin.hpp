// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformScreenContextWinType.hpp"
#include "Futurewalker.Application.PlatformScreenContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Screen context.
///
class PlatformScreenContextWin : public PlatformScreenContext
{
public:
    auto GetScreens() -> PlatformScreenArray override;
    auto GetPrimaryScreen() -> Shared<PlatformScreen> override;
    auto GetScreenFromWindow(Shared<PlatformWindow> window) -> Shared<PlatformScreen> override;
    auto GetScreenFromRect(Rect<Vp> const& rect) -> Shared<PlatformScreen> override;
    auto Refresh() -> void override;

private:
    auto Add(const PlatformScreenArray& screens) -> void;
    auto Remove(const PlatformScreenArray& screens) -> void;
    auto Update(const PlatformScreenArray& screens) -> void;

private:
    PlatformScreenArray _screens;
};

template <>
struct Locator::Resolver<PlatformScreenContextWin>
{
    using Interface = PlatformScreenContext;
    static auto Resolve() -> Shared<PlatformScreenContextWin>;
};
}
}
