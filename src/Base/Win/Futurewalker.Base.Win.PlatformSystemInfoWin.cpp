// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Win.PlatformSystemInfoWin.hpp"

#pragma warning(disable : 4996)

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto PlatformSystemInfoWin::GetPlatformKind() const -> PlatformKind
{
    return PlatformKind::Windows;
}

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
auto PlatformSystemInfoWin::GetSystemVersion() const -> SystemVersion
{
    auto osversion = OSVERSIONINFOW {};
    osversion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    if (::GetVersionEx(&osversion))
    {
        return SystemVersion {
            .major = static_cast<SInt32>(osversion.dwMajorVersion),
            .minor = static_cast<SInt32>(osversion.dwMinorVersion),
            .patch = 0,
        };
    }
    return SystemVersion {0, 0, 0};
}

///
/// @brief
///
auto Locator::Resolver<PlatformSystemInfoWin>::Resolve() -> Shared<PlatformSystemInfoWin>
{
    return Shared<PlatformSystemInfoWin>::Make();
}

}
