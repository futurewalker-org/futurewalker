// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Mac.PlatformSystemInfoMac.hpp"

namespace FW_DETAIL_NS
{
auto PlatformSystemInfoMac::IsWindows() const -> Bool
{
    return false;
}

auto PlatformSystemInfoMac::IsUnix() const -> Bool
{
    return false; // FIXME: Should this return true for macOS?
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

auto Locator::Resolver<PlatformSystemInfoMac>::Resolve() -> Shared<PlatformSystemInfoMac>
{
    return Shared<PlatformSystemInfoMac>::Make();
}
}
