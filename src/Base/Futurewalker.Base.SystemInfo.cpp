// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.SystemInfo.hpp"
#include "Futurewalker.Base.PlatformSystemInfo.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Returns true if the system is Windows.
///
auto SystemInfo::IsWindows() -> Bool
{
    if (auto const platform = Locator::GetInstance<PlatformSystemInfo>())
    {
        return platform->IsWindows();
    }
    FW_DEBUG_ASSERT(false);
    return false;
}

///
/// @brief Returns true if the system is Unix.
///
auto SystemInfo::IsUnix() -> Bool
{
    if (auto const platform = Locator::GetInstance<PlatformSystemInfo>())
    {
        return platform->IsUnix();
    }
    FW_DEBUG_ASSERT(false);
    return false;
}

///
/// @brief Returns true if the system is Unix.
///
auto SystemInfo::IsMacOS() -> Bool
{
    if (auto const platform = Locator::GetInstance<PlatformSystemInfo>())
    {
        return platform->IsMacOS();
    }
    FW_DEBUG_ASSERT(false);
    return false;
}

///
/// @brief Returns true if the system is iOS.
///
auto SystemInfo::IsIOS() -> Bool
{
    if (auto const platform = Locator::GetInstance<PlatformSystemInfo>())
    {
        return platform->IsIOS();
    }
    FW_DEBUG_ASSERT(false);
    return false;
}

///
/// @brief Returns true if the system is Android.
///
auto SystemInfo::IsAndroid() -> Bool
{
    if (auto const platform = Locator::GetInstance<PlatformSystemInfo>())
    {
        return platform->IsAndroid();
    }
    FW_DEBUG_ASSERT(false);
    return false;
}
}
