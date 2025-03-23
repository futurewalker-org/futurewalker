// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.PlatformSystemInfo.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Win32 implementation of PlatformSystemInfo.
///
class PlatformSystemInfoWin final : public PlatformSystemInfo
{
public:
    auto IsWindows() const -> Bool override;
    auto IsUnix() const -> Bool override;
    auto IsMacOS() const -> Bool override;
    auto IsIOS() const -> Bool override;
    auto IsAndroid() const -> Bool override;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformSystemInfoWin>
{
    using Interface = PlatformSystemInfo;
    static auto Resolve() -> Shared<PlatformSystemInfoWin>;
};
}
}
