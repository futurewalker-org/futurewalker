// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Screen.hpp"
#include "Futurewalker.Application.PlatformScreen.hpp"
#include "Futurewalker.Application.PlatformScreenContext.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include <mutex>
#include <vector>
#include <algorithm>
#include <ranges>

namespace FW_DETAIL_NS
{
///
/// @brief Get list of connected screens.
///
auto Screen::GetScreens() -> ScreenArray
{
    if (auto const context = Locator::GetInstance<PlatformScreenContext>())
    {
        auto const screens = context->GetScreens();
        return screens | std::ranges::views::transform([](auto&& s) { return Shared<Screen>::Make(s); }) | std::ranges::to<std::vector>();
    }
    return {};
}

///
/// @brief Get primary screen.
///
auto Screen::GetPrimaryScreen() -> Shared<Screen>
{
    if (auto const context = Locator::GetInstance<PlatformScreenContext>())
    {
        if (auto const platformObject = context->GetPrimaryScreen())
        {
            return Shared<Screen>::Make(platformObject);
        }
    }
    return {};
}

///
/// @brief Get screen from rectangle.
///
/// @param[in] rect Rectangle in virtual screen coordinate.
///
/// @return The closest screen to the given rectangle.
///
auto Screen::GetScreenFromRect(Rect<Vp> const& rect) -> Shared<Screen>
{
    if (auto const context = Locator::GetInstance<PlatformScreenContext>())
    {
        if (auto const platformObject = context->GetScreenFromRect(rect))
        {
            return Shared<Screen>::Make(platformObject);
        }
    }
    FW_DEBUG_LOG_ERROR("Screen::GetScreenFromRect(): Failed to obtain PlatformScreen object");
    return {};
}

///
/// @brief Equality comparison.
///
auto Screen::IsEqual(const Screen& lhs, const Screen& rhs) -> Bool
{
    if (lhs._platformObject && rhs._platformObject)
    {
        return lhs._platformObject == rhs._platformObject;
    }
    return false;
}

///
/// @brief Get information of the screen.
///
auto Screen::GetInfo() const -> Optional<ScreenInfo>
{
    if (_platformObject)
    {
        return _platformObject->GetInfo();
    }
    return {};
}

///
/// @brief Constructor.
///
/// @param platformObject
///
Screen::Screen(Shared<PlatformScreen> platformObject)
  : _platformObject {std::move(platformObject)}
{
}
}
