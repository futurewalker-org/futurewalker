// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.ScreenType.hpp"
#include "Futurewalker.Application.PlatformScreenType.hpp"
#include "Futurewalker.Application.PlatformWindowType.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Screen.
///
class Screen : NonCopyable
{
public:
    static auto GetScreens() -> ScreenArray;
    static auto GetPrimaryScreen() -> Shared<Screen>;
    static auto GetScreenFromWindow(Shared<PlatformWindow> window) -> Shared<Screen>;

    static auto IsEqual(const Screen& lhs, const Screen& rhs) -> Bool;

    auto GetInfo() const -> Optional<ScreenInfo>;

    Screen(PassKey<Screen>, Shared<PlatformScreen> platformObject);

private:
    Shared<PlatformScreen> _platformObject;
};
}
}
