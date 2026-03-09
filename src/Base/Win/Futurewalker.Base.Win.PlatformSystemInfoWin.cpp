// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Win.PlatformSystemInfoWin.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto PlatformSystemInfoWin::IsWindows() const -> Bool
{
    return true;
}

///
/// @brief
///
auto PlatformSystemInfoWin::IsUnix() const -> Bool
{
    return false;
}

///
/// @brief 
///
auto PlatformSystemInfoWin::IsMacOS() const -> Bool
{
    return false;
}

///
/// @brief 
///
auto PlatformSystemInfoWin::IsIOS() const -> Bool
{
    return false;
}

///
/// @brief
///
auto PlatformSystemInfoWin::IsAndroid() const -> Bool
{
    return false;
}

///
/// @brief
///
auto Locator::Resolver<PlatformSystemInfoWin>::Resolve() -> Shared<PlatformSystemInfoWin>
{
    return Shared<PlatformSystemInfoWin>::Make();
}
}
