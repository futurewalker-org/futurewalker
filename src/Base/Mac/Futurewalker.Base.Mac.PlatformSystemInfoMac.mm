// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Mac.PlatformSystemInfoMac.hpp"

namespace FW_DETAIL_NS
{
auto PlatformSystemInfoMac::GetPlatformKind() const -> PlatformKind
{
    return PlatformKind::MacOS;
}

auto PlatformSystemInfoMac::IsWindows() const -> Bool
{
    return false;
}

auto PlatformSystemInfoMac::IsUnix() const -> Bool
{
    return true;
}

auto PlatformSystemInfoMac::IsMacOS() const -> Bool
{
    return true;
}

auto PlatformSystemInfoMac::IsIOS() const -> Bool
{
    return false;
}

auto PlatformSystemInfoMac::IsAndroid() const -> Bool
{
    return false;
}

auto PlatformSystemInfoMac::GetSystemVersion() const -> SystemVersion
{
    @autoreleasepool
    {
        auto version = [NSProcessInfo processInfo].operatingSystemVersion;
        return SystemVersion {
          static_cast<int>(version.majorVersion),
          static_cast<int>(version.minorVersion),
          static_cast<int>(version.patchVersion),
        };
    }
}

auto Locator::Resolver<PlatformSystemInfoMac>::Resolve() -> Shared<PlatformSystemInfoMac>
{
    return Shared<PlatformSystemInfoMac>::Make();
}
}
